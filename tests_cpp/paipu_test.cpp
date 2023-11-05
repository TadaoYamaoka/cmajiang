#include "pch.h"
#include "../src_cpp/paipu.h"
#include "../src_cpp/random.h"

#include <fstream>


void playout(Game& game, std::mt19937_64& mt, const bool random) {
    game.kaiju();

    // 終了まで繰り返す
    while (game.status() != Game::Status::JIEJI) {
        if (random)
            random_game_state(game, 3, mt);

        // 和了/流局まで繰り返す
        while (game.status() != Game::Status::HULE && game.status() != Game::Status::PINGJU) {
            switch (game.status()) {
            case Game::Status::ZIMO:
            case Game::Status::GANGZIMO:
                if (game.allow_hule()) {
                    // 和了
                    game.reply(game.lunban_player_id(), Game::Message::HULE);
                }
                else {
                    auto dapai = game.get_dapai();
                    // 立直
                    for (const auto& p : game.allow_lizhi().second) {
                        dapai.emplace_back(p + '*');
                    }
                    // 暗槓もしくは加槓
                    for (const auto& m : game.get_gang_mianzi()) {
                        dapai.emplace_back(m);
                    }
                    std::uniform_int_distribution<int> dist(0, (int)dapai.size() - 1);
                    const auto& p = dapai[dist(mt)];
                    if (p.size() > 4)
                        game.reply(game.lunban_player_id(), Game::Message::GANG, p);
                    else
                        game.reply(game.lunban_player_id(), Game::Message::DAPAI, p);
                }
                break;
            case Game::Status::DAPAI:
                // 他家の応答 ロン、副露
                for (int player_id = 0; player_id < 4; player_id++) {
                    if (player_id == game.lunban_player_id())
                        continue;
                    const auto player_lunban = game.player_lunban(player_id);
                    if (game.allow_hule(player_lunban)) {
                        // ロン
                        game.reply(player_id, Game::Message::HULE);
                    }
                    else {
                        // 副露
                        std::vector<std::string> mianzi(1);
                        concat(mianzi, game.get_chi_mianzi(player_lunban));
                        concat(mianzi, game.get_peng_mianzi(player_lunban));
                        concat(mianzi, game.get_gang_mianzi(player_lunban));
                        std::uniform_int_distribution<int> dist(0, (int)mianzi.size() - 1);
                        const auto& m = mianzi[dist(mt)];
                        if (!m.empty()) {
                            if (m.size() > 5)
                                game.reply(player_id, Game::Message::GANG, m);
                            else
                                game.reply(player_id, Game::Message::FULOU, m);
                        }
                    }
                }
                break;
            case Game::Status::FULOU:
            {
                const auto& dapai = game.get_dapai();
                std::uniform_int_distribution<int> dist(0, (int)dapai.size() - 1);
                const auto& p = dapai[dist(mt)];
                game.reply(game.lunban_player_id(), Game::Message::DAPAI, p);
                break;
            }
            case Game::Status::GANG:
                for (int player_id = 0; player_id < 4; player_id++) {
                    if (player_id == game.lunban_player_id())
                        continue;
                    const int player_lunban = game.player_lunban(player_id);
                    if (game.allow_hule(player_lunban)) {
                        // ロン(槍槓)
                        game.reply(player_id, Game::Message::HULE);
                    }
                }
                break;
            }
            game.next();
        }
        game.next();
    }
}

TEST(PaipuTest, replay) {
    const auto random_game = [](const size_t seed) {
        std::mt19937_64 mt{ seed };
        set_seed(seed);

        Game game;
        playout(game, mt, false);

        const auto& paipu = game.paipu();
        PaipuReplay replay{ paipu };
        while (replay.status() != Game::Status::JIEJI) {
            replay.next();
        }
        EXPECT_EQ(game.point(), replay.game().point());
        };
    for (size_t seed = 0; seed < 10; seed++) {
        EXPECT_NO_THROW(random_game(seed)) << seed;
    }
}

TEST(PaipuTest, random_replay) {
    const auto random_game = [](const size_t seed) {
        std::mt19937_64 mt{ seed };
        set_seed(seed);

        Game game;
        playout(game, mt, true);

        const auto& paipu = game.paipu();
        PaipuReplay replay{ paipu };
        while (replay.status() != Game::Status::JIEJI) {
            replay.next();
        }
        EXPECT_EQ(game.point(), replay.game().point()) << seed;
        };
    for (size_t seed = 0; seed < 10; seed++) {
        EXPECT_NO_THROW(random_game(seed)) << seed;
    }
}


TEST(PaipuTest, stream) {
    for (size_t seed = 0; seed < 10; seed++) {
        std::mt19937_64 mt{ seed };
        set_seed(seed);

        Game game;
        playout(game, mt, false);

        std::stringstream ss1;
        ss1 << game.paipu();
        std::string str1 = ss1.str();

        Game::Paipu paipu;
        ss1 >> paipu;

        EXPECT_EQ(game.paipu().rounds.size(), paipu.rounds.size()) << seed;

        std::stringstream ss2;
        ss2 << paipu;
        std::string str2 = ss2.str();

        EXPECT_EQ(str1, str2) << seed;
    }
}

TEST(PaipuTest, random_stream) {
    for (size_t seed = 0; seed < 10; seed++) {
        std::mt19937_64 mt{ seed };
        set_seed(seed);

        Game game;
        playout(game, mt, true);

        std::stringstream ss1;
        ss1 << game.paipu();
        std::string str1 = ss1.str();

        Game::Paipu paipu;
        ss1 >> paipu;

        EXPECT_EQ(game.paipu().rounds.size(), paipu.rounds.size()) << seed;

        std::stringstream ss2;
        ss2 << paipu;
        std::string str2 = ss2.str();

        EXPECT_EQ(str1, str2) << seed;
    }
}

TEST(PaipuTest, stream_replay) {
    const auto random_game = [](const size_t seed) {
        std::mt19937_64 mt{ seed };
        set_seed(seed);

        Game game;
        playout(game, mt, false);

        std::stringstream ss;
        ss << game.paipu();

        Game::Paipu paipu;
        ss >> paipu;

        PaipuReplay replay{ paipu };
        while (replay.status() != Game::Status::JIEJI) {
            replay.next();
        }

        EXPECT_EQ(game.point(), replay.game().point()) << seed;
        };
    for (size_t seed = 0; seed < 10; seed++) {
        EXPECT_NO_THROW(random_game(seed)) << seed;
    }
}

TEST(PaipuTest, random_stream_replay) {
    const auto random_game = [](const size_t seed) {
        std::mt19937_64 mt{ seed };
        set_seed(seed);

        Game game;
        playout(game, mt, true);

        std::stringstream ss;
        ss << game.paipu();

        Game::Paipu paipu;
        ss >> paipu;

        PaipuReplay replay{ paipu };
        while (replay.status() != Game::Status::JIEJI) {
            replay.next();
        }

        EXPECT_EQ(game.point(), replay.game().point()) << seed;
        };
    for (size_t seed = 0; seed < 10; seed++) {
        std::cerr << seed << std::endl;
        EXPECT_NO_THROW(random_game(seed)) << seed;
    }
}

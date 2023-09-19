#pragma once

struct Rule {
    /* 点数関連 */
    // 配給原点
    int startingPoints = 25000;
    // 順位点
    int rankPoints[4] = { 20, 10, -10, -20 };
    // 連風牌は2符
    bool doubleWindTileScore = false;

    /* 赤牌有無/クイタンなど */
    // 赤牌
    int redTiles[3] = { 1, 1, 1 };
    // クイタンあり
    bool kuitanAllowed = true;
    // 喰い替え許可レベル
    int kuiChangePermissionLevel = 0; // 0: 喰い替えなし, 1: スジ喰い替えあり,  2: 現物喰い替えもあり

    /* 局数関連 */
    // 場数
    int roundsType = 2; // 0: 一局戦, 1: 東風戦, 2： 東南戦, 4: 一荘戦
    // 途中流局あり
    bool abortiveDrawAllowed = true;
    // 流し満貫あり
    bool nagashiManganAllowed = true;
    // ノーテン宣言あり
    bool notenDeclarationAllowed = false;
    // ノーテン罰あり
    bool notenPenaltyAllowed = true;
    // 最大同時和了数
    int maxSimultaneousWinners = 2; // 1: 頭ハネ, 2: ダブロンあり, 3: トリロンあり
    // 連荘方式
    int dealerContinuationType = 2; // 0: 連荘なし, 1: 和了連荘, 2: テンパイ連荘, 3: ノーテン連荘
    // トビ終了あり
    bool bankruptcyEndAllowed = true;
    // オーラス止めあり
    bool lastRoundStopAllowed = true;
    // 延長戦方式
    int overtimeType = 1; // 0: 延長戦なし, 1: サドンデス, 2: 連荘優先サドンデス, 3: 4局固定

    /* リーチ/ドラ関連 */
    // 一発あり
    bool ippatsuAllowed = true;
    // 裏ドラあり
    bool uraDoraAllowed = true;
    // カンドラあり
    bool kanDoraAllowed = true;
    // カン裏あり
    bool kanUraAllowed = true;
    // カンドラ後乗せ
    bool postKanDoraAddition = true;
    // ツモ番なしリーチあり
    bool riichiWithoutTsumoBonusAllowed = false;
    // リーチ後暗槓許可レベル
    int postRiichiClosedKanPermissionLevel = 2; // 0: 暗槓不可, 1: 牌姿の変わる暗槓不可, 2： 待ちの変わる暗槓不可

    /* 役満関連 */
    // 役満の複合あり
    bool yakumanCombinationAllowed = true;
    // ダブル役満あり
    bool doubleYakumanAllowed = true;
    // 数え役満あり
    bool countedYakumanAllowed = true;
    // 役満パオあり
    bool yakumanPaoAllowed = true;
    // 切り上げ満貫あり
    bool roundUpManganAllowed = false;
};

# cmajiang : A mahjong library for Python

## 概要
cmajiangは、Pythonの麻雀ライブラリです。

以下は、対局を開始して、配牌を行い、1巡目で自摸を行い、手牌を表示する例です。

```python
>>> from cmajiang import *

>>> game = Game()
>>> game.kaiju() # 開始
>>> game.qipai() # 配牌
>>> game.zimo()  # 自摸

>>> game.shoupai[0] # 手牌
```
<image src="https://raw.githubusercontent.com/wiki/TadaoYamaoka/cmajiang/images/shoupai.svg?sanitize=true" />

## 機能

* 対局

```python
>>> game.kaiju() # 開始
>>> # 流局まで自摸と打牌を繰り返す
>>> while game.status != Status.PINGJU:
        if game.status == Status.ZIMO:
            game.reply(
                game.lunban_player_id,
                Message.DAPAI,
                game.get_dapai()[0])
        game.next()
```

* 牌の文字列表現
  
  * アルファベットは、牌種を表す。`m`:萬子、`p`:筒子、`s`:索子、`z`:字牌
  * 数字は、牌種内の順序を表す。萬子・筒子・索子の場合、`0`:赤牌、`1`～`9`:牌の数字。字牌の場合、`1`～`4`:風牌、`5`～`7`:三元牌
  * `_`は自摸切りを表す。
  * `*`は立直を表す。
  * `+`/`=`/`-`は鳴かれたことを表す。`-`:下家、`=`:対面、`+`:上家
  * `_`一文字は裏向きの牌を表す。

* 面子の文字列表現

  * 1文字目は牌種を表す。
  * 数字は、牌種内の順序を表す。萬子・筒子・索子の場合、`0`:赤牌、`1`～`9`:牌の数字。字牌の場合、`1`～`4`:風牌、`5`～`7`:三元牌
  * `+`/`=`/`-`は鳴かれたことを表す。`-`:下家、`=`:対面、`+`:上家
  * ポンの場合、`+`/`=`/`-`は末尾に付ける(大明槓も同様)。
  * 加槓の場合、ポンの面子の末尾に数字を付与する。
  * 数字は、1から昇順に並べる(`0`は`5`の次とする)。

* 牌姿(手牌)の文字列表現

  * 純手牌(副露面子以外の打牌可能な手牌)、副露面子の順に`,`区切りで並べる。
  * 純手牌は、同種の牌が続く場合、牌種を省略し数字のみとする。
  * 純手牌は、萬子→筒子→索子→字牌(風牌→三元牌)の順、同種の牌は数字順に並べる(`0`は`5`の次とする)。
  * 手牌が14枚のある場合、純手牌の最後1枚が自摸した牌を表す。自摸した牌は牌種を省略しない。
  * 副露直後は、末尾に`,`を付与する。
  * 立直後は、純手牌の末尾に`*`を付与する。
  * 副露面子は、副露順に面子を`,`区切りで並べる。
  * 純手牌内の`_`は不明な牌を表す。

* 向聴数

```python
>>> xiangting(Shoupai("m1188p2388s05z111"))
```
```
1
```

* 和了点数計算

```python
hule(Shoupai("m123p123z1z1,s1-23,z222="), menfeng=0, baopai=["z1"], haidi=True)
```
```
海底摸月	1翻
三色同順	1翻
混全帯幺九	1翻
ドラ	3翻
40符 6翻 跳満 18000点
```

## インストール

* GitHubのソースからインストール

以下のコマンドでインストールします。インストールにはpybind11と対応したC++コンパイラが必要です。

```
pip install git+https://github.com/TadaoYamaoka/cmajiang
```

## ドキュメント

準備中

## 謝辞

多くの部分で
[majiang-core](https://github.com/kobalab/majiang-core)
のソースを流用しています。

## ライセンス

MIT

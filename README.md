# main.cpp
* 行列ABの積を計算するコード<br>
* 「キャッシュを意識したコード」および「OpenMPによる並列化」による高速化の効果を切り替えて評価可能
* コード15行目以下の定義を変更しながら評価を行う

## コード切り替えのための定義（#define）
1. PARALLEL
    * OpenMPを用いた並列実装へ切り替え
    * SERIALとは共存不可
    * キャッシュを意識したコード
2. SERIAL
    * 1スレッドを用いた"未"並列処理
    * PARALLELとは共存不可
    * SERIAL_CASHとは共存可能で単体だとキャッシュを意識しないコード
3. SERIAL_CASH
    * 未並列処理をキャッシュを意識した処理に切り替える定義
    * SERIALと同時に有効にすることでキャッシュを意識したコードへ切り替え

## その他の定義（#define）
1. ROW_&#42;およびCOL_&#42;
    * 行列サイズを決定する定義
2. TRY
    * 処理の実行回数を定義
3. THREADS
    * 何スレッドで並列化を行うか手動選択（THREADS>1）
    * 環境における最大スレッド数以上を指定すると、最大スレッド数を自動選択される
4. DEBUG
    * 実装が正しいことの確認用定義
    * 評価の際には必要ないため無効にする

# 実行方法
1. 「makefile」ファイルのあるディレクトリ内でコマンド`make`を実行→コンパイルされる
2. ディレクトリに「main」ができるのでコマンド`main`で処理を実行

<br>
<font color="00000a">行列サイズを変更してもう一度実行する時</font><br>

`make clean`コマンドで実行ファイルを削除可能

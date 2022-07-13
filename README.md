# E75 TPC データ解析サンプル

## データファイルのデコード
```shell
root -q -b -l unpack.C
```
入力ファイル、出力ファイルを変更したい場合は、`unpack.C`　コード中の
```C++
c->initialize("output.dat","output.root");
```
のファイル名を書き換えれば良い。

## デコードされた(ROOT)ファイルからグラフ作成

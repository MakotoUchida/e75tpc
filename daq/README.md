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
```shell
root -l disp.C
```
ファイル名を変えたい場合は `disp.C` コード内
```C++
TFile* f = new TFile("output.root");
```
の部分のファイル名を変える。  
入力チャンネル変えたい場合は
```C++
std::vector<int> y = tpc->getChData(3);
```
のチャンネルを変える。  

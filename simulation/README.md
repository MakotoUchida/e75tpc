# E75 TPC simulation
## コンパイル
```shell
cd simulation
mkdir g4tpc-build
cd g4tpc-build
cmake -DGeant4_DIR=/usr/local/geant4/lib/Geant4-11.0.0  $HOME/e75tpc/simulation/g4tpc
make
```
以上の手続きで`g4tpc-build`に実行ファイル`g4tpc`が作成される。
## simulationの実行
```shell
./g4tpc single.mac 1 0 0
```
引数の意味は順番に実験番号(EEEE)、ラン番号(RRRRR)、ストリーム番号(SSS)。  
root出力は`mc.eEEEE.rRRRRR.sSSS.root`となる。

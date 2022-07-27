# Caen HV リモートコントロールの方法
USBケーブルでCAEN HVとMacを接続
```shell
ls /dev/*usbserial*
```
これで表示されたUSBポートにたいして`screen`で接続
```shell
screen /dev/tty.usbserial-XXXX 9600
```
ただしXXXXの部分は毎回変わるので注意する。  
接続が成功したらまず`caen`とタイプすると制御画面が表示される。

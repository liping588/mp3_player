 MP3 播放器
============

使用硬體
--------

* ARDUINO UNO R3 (串列埠晶片：CH340)
    * 購買地點：蝦皮找 ARDUINO UNO R3
* DFPlayer Mini MP3 播放模組
    * 購買網址：https://goods.ruten.com.tw/item/show?21532302854342
* 七段顯示模組 (驅動晶片：TM1637)
    * 購買網址：https://goods.ruten.com.tw/item/show?21643107650926
* 21鍵紅外線遙控器(NEC編碼) + HX1838 紅外線接收頭 
    * 購買網址：https://www.ruten.com.tw/item/show?21310198500426
* 5鍵類比鍵盤模組
    * 購買網址：https://www.ruten.com.tw/item/show?22018503365656
* 全頻喇叭(4Ω , 3W)
    * 購買網址：https://goods.ruten.com.tw/item/show?21852320652466

使用軟體
--------

* Arduino IDE
    * 下載網址：https://www.arduino.cc/en/software

硬體接線
---------

 ![image](https://github.com/liping588/mp3_player/blob/master/image/mp3_link.jpeg)

* ARDUINO UNO R3 PIN_2  -> HX1838 紅外線接收頭 VOUT
* ARDUINO UNO R3 PIN_3  -> 七段顯示模組 CLK
* ARDUINO UNO R3 PIN_4  -> 七段顯示模組 DIO
* ARDUINO UNO R3 PIN_10 -> 1KΩ電阻 - DFPlayer Mini MP3 PIN3(TX)
* ARDUINO UNO R3 PIN_11 -> 1KΩ電阻 - DFPlayer Mini MP3 PIN2(RX)
* ARDUINO UNO R3 PIN_A0 -> 5鍵類比鍵盤模組 OUT
* DFPlayer Mini MP3 PIN6(SPK-) -> 全頻喇叭 PIN -
* DFPlayer Mini MP3 PIN8(SPK+) -> 全頻喇叭 PIN +

程式碼編譯方法
--------------

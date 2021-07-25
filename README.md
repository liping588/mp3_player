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
* 21鍵紅外線遙控器(NEC編碼) + VS1838 紅外線接收頭
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

 ![image](https://github.com/liping588/mp3_player/blob/master/image/mp3_pic%20.jpg)

* ARDUINO UNO R3 PIN_2  --> VS1838 紅外線接收頭 VOUT
    * VS1838: 建議 VOUT 接 10KΩ 電阻上拉到 VCC，而  VCC 串 100Ω 電阻
* ARDUINO UNO R3 PIN_3  --> 七段顯示模組 CLK
* ARDUINO UNO R3 PIN_4  --> 七段顯示模組 DIO
* ARDUINO UNO R3 PIN_10 --> 1KΩ電阻 - DFPlayer Mini MP3 PIN_3(TX)
* ARDUINO UNO R3 PIN_11 --> 1KΩ電阻 - DFPlayer Mini MP3 PIN_2(RX)
* ARDUINO UNO R3 PIN_A0 --> 5鍵類比鍵盤模組 OUT
* DFPlayer Mini MP3 PIN6(SPK-) --> 全頻喇叭 PIN -
* DFPlayer Mini MP3 PIN8(SPK+) --> 全頻喇叭 PIN +

程式編譯與下載
--------------

* 1.在 mp3_player.ino 檔案連點二下會開啟 Arduino IDE

* 2.在 [草稿碼]->[匯入程式庫]->[管理程式庫] 安裝下列四個模組
    * IRremote               (控制紅外線發線與接收)
    * Grove 4Digital Display (控制 TM1637 七段顯示器模組)
    * DFRobotDFPlayerMini    (控制 DFPlayerMini MP3 播放模組)
    * TimerOne               (定時器函式)

* 3.按下驗証扭以編譯程式，如果程式庫都有安裝，編譯應該不會有錯誤

* 4.確認 [工具]->[序列埠] 是否選擇正確

* 5.按下上傳扭，將程式下載到 ARDUINO UNO R3 板子上

操作說明
--------

* DFPlayer Mini MP3 的 SD 卡檔案配置
    * 1.將 SD 卡格式化為 FAT 或 FAT32
    * 2.修改 MP3 檔名格式為 0001~0099數字 + 底線 + 歌曲名稱
        - 範例1：0001_歌曲1.mp3
        - 範例2：0050_abc123.mp3
    * 3.在根目錄下建立一個 mp3 目錄並將修改好的 MP3 檔案放進去

* 紅外線遙控器
    * PLAY/PAUSE: 播放或暫停
    * PREV: 切換到上一首
    * NEXT: 切換到下一首
    * VOL+: 大聲
    * VOL-: 小聲
    * EQ: 音效模式切換(共五種)

* 類比鍵盤
    * SW1: 切換到上一首
    * SW2: 大聲
    * SW3: 小聲
    * SW4: 切換到下一首
    * SW5: 播放或暫停

* 七段顯示器
    * 前二位顯示曲目 01-99，後二位顯示聲音階數 00-30 和 EQ 模式 01-05

* 播放行為
    * 播放完一首即停止，不會持續播放下一首
    * 曲目 1~99 首可供選擇
    * 聲音 0~30 階可供調整

未來計劃
--------

* 增加功放晶片 PAM8403(5v,4Ω,3W+3W)
    * 參考網址1：https://goods.ruten.com.tw/item/show?21444615471874
    * 參考網址2：https://goods.ruten.com.tw/item/show?21708318144504

* 增加循環與隨機放功能
    * 透過紅外線遙控器 CH, CH+, CH- 按鍵來作為模式切換

* 增加記憶功能
    * 透過 ARDUINO UNO R3 內建的 EEPROM 來記憶曲目、聲音階數、播放模式、EQ 等等

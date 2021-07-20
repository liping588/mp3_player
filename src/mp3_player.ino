#include <Arduino.h>
#include <TimerOne.h>
#include "TM1637.h"
#define DECODE_NEC
#include <IRremote.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define ADKEY_NUMS      6
#define ADKEY_UP_ADC    1018

#define TM1637_CLK_PIN  3
#define TM1637_DIO_PIN  4

#define IR_RECV_PIN     2

#define MP3_RX_PIN      10
#define MP3_TX_PIN      11

const uint16_t g_adkey_key_adc[ADKEY_NUMS] = {0, 144, 330, 506, 742, ADKEY_UP_ADC};
uint8_t g_adkey_last_key = 255, g_adkey_dn_cnt = 0;
int8_t g_ctrl_track = 0, g_ctrl_volume = 0, g_ctrl_eq_val = 0, g_ctrl_cnt_1 = 0;
bool g_ctrl_start = false, g_ctrl_en_1 = false;

TM1637 tm1637(TM1637_CLK_PIN, TM1637_DIO_PIN);

SoftwareSerial mp3_uart(MP3_RX_PIN, MP3_TX_PIN);
DFRobotDFPlayerMini mp3_module;

//------------------------------------------------------------------------------
int8_t adkey_get_key(uint16_t adc)
{
  uint8_t i;

  for (i = 0; i < ADKEY_NUMS; ++i) {
    if ((adc >= g_adkey_key_adc[i]) && (adc <= g_adkey_key_adc[i] + 5)) {
      g_adkey_dn_cnt++;
      if (i == (ADKEY_NUMS - 1)) {
        g_adkey_dn_cnt = 0;
        g_adkey_last_key = 255;
        break;
      } else if ((g_adkey_dn_cnt == 3) && (i != g_adkey_last_key)) {
        g_adkey_dn_cnt = 0;
        g_adkey_last_key = i;
        return i;
      }
    }
  }

  return (-1);
}

//------------------------------------------------------------------------------
void tm1637_update(uint8_t v1, uint8_t v2)
{
  tm1637.point(POINT_ON);
  tm1637.display(0, v1 / 10);
  tm1637.display(1, v1 % 10);
  tm1637.display(2, v2 / 10);
  tm1637.display(3, v2 % 10);
}

void tm1637_update_eq(uint8_t v)
{
  tm1637.point(POINT_ON);
  tm1637.display(0, 14);
  tm1637.display(1, 0);
  tm1637.display(2, v / 10);
  tm1637.display(3, v % 10);
}

//------------------------------------------------------------------------------
void mp3_print_error(uint8_t type, int value)
{
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

//------------------------------------------------------------------------------
void ctrl_init(void)
{
  g_ctrl_track = 1;
  g_ctrl_volume = 15;
  g_ctrl_eq_val = 0;

  IrReceiver.begin(IR_RECV_PIN);

  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);
  tm1637_update(g_ctrl_track, g_ctrl_volume);

  mp3_uart.begin(9600);
  if (!mp3_module.begin(mp3_uart, true, false)) {
    Serial.println("Unable to begin:");
    while(true);
  }

  mp3_module.setTimeOut(500);
  mp3_module.volume(g_ctrl_volume);
  mp3_module.EQ(DFPLAYER_EQ_NORMAL);
  mp3_module.playMp3Folder(g_ctrl_track);
  mp3_module.pause();
}

void ctrl_update(void)
{
  if (g_ctrl_en_1) {
    if (++g_ctrl_cnt_1 >= 4) {
      g_ctrl_cnt_1 = 0;
      g_ctrl_en_1 = false;
      tm1637_update(g_ctrl_track, g_ctrl_volume);
    }
  }
}

void ctrl_track_next(void)
{
  g_ctrl_track = (g_ctrl_track >= 99) ? 99 : g_ctrl_track + 1;
  tm1637_update(g_ctrl_track, g_ctrl_volume);
  mp3_module.playMp3Folder(g_ctrl_track);
  g_ctrl_start = true;
}

void ctrl_track_prev(void)
{
  g_ctrl_track = (g_ctrl_track <= 1) ? 1 : g_ctrl_track - 1;
  tm1637_update(g_ctrl_track, g_ctrl_volume);
  mp3_module.playMp3Folder(g_ctrl_track);
  g_ctrl_start = true;
}

void ctrl_volume_up(void)
{
  g_ctrl_volume = (g_ctrl_volume >= 30) ? 30 : g_ctrl_volume + 1;
  mp3_module.volume(g_ctrl_volume);
  tm1637_update(g_ctrl_track, g_ctrl_volume);
}

void ctrl_volume_down(void)
{
  g_ctrl_volume = (g_ctrl_volume <= 0) ? 0 : g_ctrl_volume - 1;
  mp3_module.volume(g_ctrl_volume);
  tm1637_update(g_ctrl_track, g_ctrl_volume);
}

void ctrl_play_pause(void)
{
  if (g_ctrl_start) {
    mp3_module.pause();
  } else {
    mp3_module.start();
  }
  g_ctrl_start = !g_ctrl_start;
}

void ctrl_eq_trigger(void)
{
  g_ctrl_eq_val = (g_ctrl_eq_val >= 5) ? 0 : g_ctrl_eq_val + 1;
  mp3_module.EQ(g_ctrl_eq_val);
  tm1637_update_eq(g_ctrl_eq_val);
  g_ctrl_cnt_1 = 0;
  g_ctrl_en_1 = true;
}

//------------------------------------------------------------------------------
void time_isr()
{
  ctrl_update();
}

void setup()
{
  Serial.begin(115200);

  Timer1.initialize(500000);
  Timer1.attachInterrupt(time_isr);

  ctrl_init();
}

void loop()
{
  // ----------------
  //  ADC key handle
  // ----------------

  int8_t key = adkey_get_key(analogRead(A0));
  if (key != -1) {
    switch (key) {
    case 0:
      // left key.
      ctrl_track_prev();
      break;

    case 1:
      // up key.
      ctrl_volume_up();
      break;

    case 2:
      // down key.
      ctrl_volume_down();
      break;

    case 3:
      // right key.
      ctrl_track_next();
      break;

    case 4:
      // enter.
      ctrl_play_pause();
      break;
    }
  }

  // -----------
  //  IR handle
  // -----------

  if (IrReceiver.decode()) {

#if 0 // for debug.
    // Print a short summary of received data
    IrReceiver.printIRResultShort(&Serial);
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      // We have an unknown protocol here, print more info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
    }
    Serial.println();
#endif

    // Enable receiving of the next value.
    IrReceiver.resume();

    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
//      Serial.println(IrReceiver.decodedIRData.command); // for debug.

      switch (IrReceiver.decodedIRData.command) {
      case 0x44:
        // |<< (PREV)
        ctrl_track_prev();
        break;

      case 0x40:
        // >>| (NEXT)
        ctrl_track_next();
        break;

      case 0x43:
        // >|| (PLAY/PAUSE)
        ctrl_play_pause();
        break;

      case 0x07:
        // - (VOL-)
        ctrl_volume_down();
        break;

      case 0x15:
        // + (VOL+)
        ctrl_volume_up();
        break;

      case 0x09:
        // EQ
        ctrl_eq_trigger();
        break;
      }
    }
  }

  // ------------
  //  MP3 handle
  // ------------

  if (mp3_module.available()) {
    mp3_print_error(mp3_module.readType(), mp3_module.read());
  }
}

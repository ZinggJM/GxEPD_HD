// GxEPD_HD_Scribble : touch example for HD e-Paper displays from Dalian Good Display Inc. (parallel interface).
//
// Display Library based on Demo Example available from Good Display
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD_HD
//
// To be used with Board: "Generic STM32F1 series" Board part number: "Generic F103VE" of package "STM32 Boards (select from submenu)" for Arduino
// Add this package by adding "https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json"
// to Preferences "Additional Boards Manager URLs" and installing with Boards Manager.
//
// Can also be used with "STM32F103VE" of "Generic STM32F103V series" of package "STM32 Boards (STM32Duino.com)" for Arduino.
// download this package as .zip file from https://github.com/rogerclarkmelbourne/Arduino_STM32
// and install it as described in https://github.com/rogerclarkmelbourne/Arduino_STM32/wiki/Installation
//
// Can also be used with ESP32 boards, e.g. "DOIT ESP32 DEVKIT V1", with my proto board for DESTM32-Tcon-11.
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html

// include library, include base class, make path known
#include <GxEPD_HD_EPD.h>
#include <GxEPD_HD_BW.h>

#include "GxFT5436/GxFT5436.h"

// select the display io class to use, only one
#if (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) || (defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_GENERIC_F103VE))
#include <GxDESTM32T/GxDESTM32T.h>
GxDESTM32T io;
#else
// next is for my DESP32T (proto board) for TCon-11 parallel interface
//#include <GxDESP32T/GxDESP32T.h>
//GxDESP32T io;
// next is for my DESP32T_BP (proto board) for TCon-11 parallel interface
#include <GxDESP32T_BP/GxDESP32T_BP.h>
GxDESP32T_BP io;
#endif

// select the base display class to use, only one
GxGDE060F3 base_display(io, -2.4); // vcom from sticker on flex connector of my panel, as double

// select the graphics display template class to use, only one
#if (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) || (defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_GENERIC_F103VE))
GxEPD_HD_BW < GxGDE060F3, GxGDE060F3::HEIGHT / 2 > display(base_display); // half height, 2 pages, ~11k RAM remaining
#else
GxEPD_HD_BW<GxGDE060F3, GxGDE060F3::HEIGHT> display(base_display); // full height, one page
#endif

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#if (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) // "STM32 Boards (STM32Duino.com)"
// select diagnostic output stream, only one
//HardwareSerial& DiagnosticStream = Serial1; // pins PA9, PA10
HardwareSerial& DiagnosticStream = Serial2; // pins PA2, PA3 for USB jumpers
//HardwareSerial& DiagnosticStream = Serial3; // pins PB10, PB11
//USBSerial& DiagnosticStream = Serial; // pins PA11, PA12 USB direct?
#elif (defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_GENERIC_F103VE)) // "STM32 Boards (select from submenu)"
//HardwareSerial DiagnosticStream(PA10, PA9); // pins PA9, PA10
HardwareSerial DiagnosticStream(PA3, PA2); // pins PA2, PA3 for USB jumpers
//HardwareSerial DiagnosticStream(PB11, PB10); // pins PB10, PB11
#else
HardwareSerial& DiagnosticStream = Serial; // ESP32
#endif

#if (defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_GENERIC_F103VE)) // "STM32 Boards (select from submenu)"
//GxFT5436 touch(/*SDA=*/PC10, /*SCL=*/PC12, /*RST=*/PA12); // SW I2C doesn't work
//GxFT5436 touch(/*RST=*/PA12); // HW I2C, default on PB7, PB6 works
//GxFT5436 touch(/*SDA=*/PB7, /*SCL=*/PB6, /*RST=*/PA12); // HW I2C works
//GxFT5436 touch(/*SDA=*/PB9, /*SCL=*/PB8, /*RST=*/PA12); // HW I2C works
GxFT5436 touch(/*SDA=*/PB11, /*SCL=*/PB10, /*RST=*/PA12); // HW I2C works
#elif (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) // "STM32 Boards (STM32Duino.com)"
//GxFT5436 touch(/*SDA=*/PC10, /*SCL=*/PC12, /*RST=*/PA12); // SW I2C doesn't work
//GxFT5436 touch(/*RST=*/PA12); // HW I2C, default on PB7, PB6 works (I2C1)
//GxFT5436 touch(/*SDA=*/PB7, /*SCL=*/PB6, /*RST=*/PA12); // HW I2C works (I2C1)
GxFT5436 touch(/*SDA=*/PB11, /*SCL=*/PB10, /*RST=*/PA12); // HW I2C works (I2C2)
#elif defined(ESP8266)
GxFT5436 touch(/*RST=*/D3);
#endif

const uint16_t r = 100;
uint16_t x = base_display.WIDTH - r;
uint16_t y = r;

void setup()
{
  DiagnosticStream.begin(115200);
  while (!DiagnosticStream);
  delay(200);
  DiagnosticStream.println();
  DiagnosticStream.println("setup");
  DiagnosticStream.println("hello DiagnosticStream");

  delay(200);
  display.init(&DiagnosticStream);
  touch.init(&DiagnosticStream); // with diagnostics, (maybe, some uncommented ones)
  showClearMe(x, y, r);
  delay(1000);
  randomSeed(analogRead(0));
  DiagnosticStream.println("setup done");
}

uint8_t black2x2[] = {0, 0, 0, 0};
uint8_t black3x3[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t black4x4[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t grey4x4[] = {0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F};

bool refresh_pending = false;
uint32_t refreshed;

void loop()
{
  uint8_t cnt;
  uint16_t tx, ty;
  cnt = touch.newSingleTouch(tx, ty);
  if (cnt > 0)
  {
    transpose(tx, ty);
    //DiagnosticStream.print(cnt); DiagnosticStream.print(" ("); DiagnosticStream.print(tx); DiagnosticStream.print(", "); DiagnosticStream.print(ty); DiagnosticStream.println(")");
    if (near(x, y, tx, ty, r))
    {
      showClearMe(x, y, r);
      refresh_pending = false;
      refreshed = millis();
    }
    else
    {
      //display.writeImage(black2x2, sizeof(black2x2), 8, tx, ty, 2, 2);
      //display.writeImage(black3x3, sizeof(black3x3), 8, tx, ty, 3, 3);
      //display.writeImage(black4x4, sizeof(black4x4), 8, tx, ty, 4, 4);
      display.writeImage(grey4x4, sizeof(grey4x4), 8, tx, ty, 4, 4);
      if (!refresh_pending)
      {
        refresh_pending = true;
        refreshed = millis();
      }
      //display.refresh(true);
    }
  }
  if (refresh_pending && (millis() - refreshed > 500))
  {
    //display.refresh(true);
    display.refresh(false); // needed for grey
    refresh_pending = false;
    refreshed = millis();
  }
}

const char ClearMe[] = "Clear Me";

void showClearMe(uint16_t x, uint16_t y, uint16_t r)
{
  //DiagnosticStream.println("showClearMe");
  display.setFont(&FreeMonoBold18pt7b);
  display.setTextColor(GxEPD_WHITE);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(ClearMe, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t cx = x - tbw / 2 - tbx;
  uint16_t cy = y - tbh / 2 - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.fillCircle(x, y, r, GxEPD_BLACK);
    display.setCursor(cx, cy);
    display.print(ClearMe);
  }
  while (display.nextPage());
  //DiagnosticStream.println("showClearMe done");
}

bool near(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t d)
{
  uint32_t dx = x1 > x2 ? x1 - x2 : x2 - x1;
  uint32_t dy = y1 > y2 ? y1 - y2 : y2 - y1;
  return (dx * dx + dy * dy <= d * d);
}

void transpose(uint16_t& x, uint16_t& y)
{
  if (true)
  {
    uint16_t x1 = x;
    uint16_t y1 = y;
    // fix scaling issue, comment out if controller fixed
    x1 = uint16_t(uint32_t(x) * uint32_t(758) / uint32_t(1024));
    y1 = uint16_t(uint32_t(y) * uint32_t(1024) / uint32_t(758));
    x = y1;
    y = 757 - x1;
  }
}

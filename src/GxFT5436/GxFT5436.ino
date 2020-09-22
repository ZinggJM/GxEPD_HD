// Display Library for parallel interface e-paper panels from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Examples from Good Display, available here: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// GxFT5436.ino : test for FT5436 touch driver class for GDE060F3-T on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD_HD

#include "GxFT5436.h"

#if (defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_GENERIC_F103VE)) // "STM32 Boards (select from submenu)"
//GxFT5436 touch(/*SDA=*/PC10, /*SCL=*/PC12, /*RST=*/PA12); // doesn't work
//GxFT5436 touch(/*SDA=*/PB7, /*SCL=*/PB6, /*RST=*/PA12); // HW I2C
GxFT5436 touch(/*RST=*/PA12); // HW I2C
HardwareSerial DiagnosticStream(PA3, PA2); // pins PA2, PA3 for USB jumpers
#elif (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) // "STM32 Boards (STM32Duino.com)"
// *** doesn't compile, incompatible I2C TwoWire class
//GxFT5436 touch(/*SDA=*/PC10, /*SCL=*/PC12, /*RST=*/PA12); // doesn't work
HardwareSerial& DiagnosticStream = Serial2; // pins PA2, PA3 for USB jumpers
#elif defined(ESP8266)
GxFT5436 touch(/*RST=*/D3);
HardwareSerial& DiagnosticStream = Serial; // ESP32
#endif

void setup()
{
  DiagnosticStream.begin(115200);
  while (!DiagnosticStream);
  DiagnosticStream.println("\nGxFT5436 Test");
  //touch.init(); // no diagnostics
  touch.init(&DiagnosticStream); // with diagnostics, (maybe, some uncommented ones)
}

void loop()
{
  test();
  //delay(5000);
}

bool use_transposition = true;

void transpose(uint16_t& x, uint16_t& y)
{
  if (use_transposition)
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

void transpose(GxFT5436::TouchInfo& touchinfo)
{
  if (use_transposition)
  {
    for (uint8_t i = 0; i < touchinfo.touch_count; i++)
    {
      transpose(touchinfo.x[i], touchinfo.y[i]);
    }
  }
}

enum TestCase
{
  scanSingle, scanMulti, newSingle, newMulti, lastSingle, lastMulti
};

TestCase testcase = newMulti;

void test()
{
  uint8_t cnt;
  uint16_t x, y;
  if (testcase == scanSingle)
  {
    cnt = touch.scanSingleTouch(x, y);
    transpose(x, y);
    DiagnosticStream.print(cnt); DiagnosticStream.print(" ("); DiagnosticStream.print(x); DiagnosticStream.print(", "); DiagnosticStream.print(y); DiagnosticStream.println(")");
    delay(1000);
  }
  if (testcase == scanMulti)
  {
    GxFT5436::TouchInfo touchinfo = touch.scanMultipleTouch();
    if (touchinfo.touch_count > 0)
    {
      transpose(touchinfo);
      for (uint8_t i = 0; i < touchinfo.touch_count; i++)
      {
        DiagnosticStream.print("("); DiagnosticStream.print(touchinfo.x[i]); DiagnosticStream.print(", "); DiagnosticStream.print(touchinfo.y[i]); DiagnosticStream.print(") ");
      }
      DiagnosticStream.println(touchinfo.touch_count);
    }
    else DiagnosticStream.println(0);
    delay(1000);
  }
  if (testcase == newSingle)
  {
    cnt = touch.newSingleTouch(x, y);
    if (cnt > 0)
    {
      transpose(x, y);
      DiagnosticStream.print(cnt); DiagnosticStream.print(" ("); DiagnosticStream.print(x); DiagnosticStream.print(", "); DiagnosticStream.print(y); DiagnosticStream.println(")");
    }
  }
  if (testcase == newMulti)
  {
    GxFT5436::TouchInfo touchinfo = touch.newMultipleTouch();
    if (touchinfo.touch_count > 0)
    {
      transpose(touchinfo);
      for (uint8_t i = 0; i < touchinfo.touch_count; i++)
      {
        DiagnosticStream.print("("); DiagnosticStream.print(touchinfo.x[i]); DiagnosticStream.print(", "); DiagnosticStream.print(touchinfo.y[i]); DiagnosticStream.print(") ");
      }
      DiagnosticStream.println(touchinfo.touch_count);
    }
  }
  if (testcase == lastSingle)
  {
    cnt = touch.newSingleTouch(x, y);
    if (cnt > 0)
    {
      transpose(x, y);
      DiagnosticStream.print(cnt); DiagnosticStream.print(" ("); DiagnosticStream.print(x); DiagnosticStream.print(", "); DiagnosticStream.print(y); DiagnosticStream.println(")");
      cnt = touch.lastSingleTouch(x, y);
      if (cnt > 0)
      {
        transpose(x, y);
        DiagnosticStream.print(cnt); DiagnosticStream.print(" ("); DiagnosticStream.print(x); DiagnosticStream.print(", "); DiagnosticStream.print(y); DiagnosticStream.println(")");
      }
    }
  }
  if (testcase == lastMulti)
  {
    cnt = touch.newSingleTouch(x, y);
    if (cnt > 0)
    {
      transpose(x, y);
      DiagnosticStream.print(cnt); DiagnosticStream.print(" ("); DiagnosticStream.print(x); DiagnosticStream.print(", "); DiagnosticStream.print(y); DiagnosticStream.println(")");
      GxFT5436::TouchInfo touchinfo = touch.lastMultipleTouch();
      if (touchinfo.touch_count > 0)
      {
        transpose(touchinfo);
        for (uint8_t i = 0; i < touchinfo.touch_count; i++)
        {
          DiagnosticStream.print("("); DiagnosticStream.print(x); DiagnosticStream.print(", "); DiagnosticStream.print(y); DiagnosticStream.print(") ");
        }
        DiagnosticStream.println();
      }
    }
  }
}

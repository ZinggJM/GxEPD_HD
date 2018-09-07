// class GxEPD_HD_EPD : Base Class for Display Classes for e-Paper Displays from Dalian Good Display Co., Ltd.: www.good-display.com
//
// based on Demo Examples from Good Display, available here: http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD_HD

#ifndef _GxEPD_HD_EPD_H_
#define _GxEPD_HD_EPD_H_

#include <Arduino.h>
#include <SPI.h>

#define GxEPD_BLACK     0x0000
#define GxEPD_DARKGREY  0x7BEF      /* 128, 128, 128 */
#define GxEPD_LIGHTGREY 0xC618      /* 192, 192, 192 */
#define GxEPD_WHITE     0xFFFF
#define GxEPD_RED       0xF800      /* 255,   0,   0 */

#define GxEPD_YELLOW    GxEPD_RED
#define GxEPD_COLORED   GxEPD_RED

class GxEPD_HD
{
  public:
    enum Panel
    {
      GDE043A2,
      GDE06,
      GDE060BA,
      GDEW080T5
    };
};

class GxEPD_HD_IOCTRL
{
  public:
    virtual void init(bool enable_diagnostic_output) = 0;
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    void clearScreen(uint8_t value = 0xFF); // init controller memory and screen (default white)
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    virtual void writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
    virtual void writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                                uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0) = 0;
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    virtual void drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
    virtual void drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                               uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0) = 0;
    // screen refresh from controller memory to full screen
    virtual void refresh(bool partial_update_mode = false) = 0;
    // screen refresh from controller memory, partial screen
    virtual void refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode = false) = 0;
    virtual void powerOff() = 0;
};

class GxEPD_HD_EPD
{
  public:
    // attributes
    const uint16_t WIDTH;
    const uint16_t HEIGHT;
    const GxEPD_HD::Panel panel;
    const bool hasColor;
    const bool hasPartialUpdate;
    const bool hasFastPartialUpdate;
    // constructor
    GxEPD_HD_EPD(uint16_t w, uint16_t h, GxEPD_HD::Panel p, bool c, bool pu, bool fpu) :
      WIDTH(w), HEIGHT(h), panel(p), hasColor(c), hasPartialUpdate(pu), hasFastPartialUpdate(fpu) {};
    // methods (virtual)
    virtual void init(uint32_t serial_diag_bitrate = 0) = 0; // = 0 : disabled
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    void clearScreen(uint8_t value = 0xFF); // init controller memory and screen (default white)
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    virtual void writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
    virtual void writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                                uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0) = 0;
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    virtual void drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
    virtual void drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                               uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0) = 0;
    // screen refresh from controller memory to full screen
    virtual void refresh(bool partial_update_mode = false) = 0;
    // screen refresh from controller memory, partial screen
    virtual void refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode = false) = 0;
    virtual void powerOff() = 0;
};

#endif


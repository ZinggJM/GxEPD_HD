// Display Library for parallel interface e-paper panels from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Examples from Good Display, available here: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// class GxEPD_HD_EPD : Base class for panel driving classes
// class GxEPD_HD_IOCTRL : Base class for panel driving connection classes
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
#include "GxEPD_HD.h"

class GxEPD_HD_IOCTRL
{
  public:
    virtual void init(GxEPD_HD::Panel panel, uint16_t vcom_mV, Stream* pDiagnosticOutput) = 0;
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    virtual void clearScreen(uint8_t value = 0xFF) = 0; // init controller memory and screen (default white)
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
    virtual void powerOff() = 0; // turns off generation of panel driving voltages
    virtual void hibernate() = 0;
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
    virtual void init(Stream* pDiagnosticOutput = 0) = 0; // (pDiagnosticOutput = 0) : disabled
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    virtual void clearScreen(uint8_t value = 0xFF) = 0; // init controller memory and screen (default white)
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
    virtual void hibernate() = 0;
};

#endif

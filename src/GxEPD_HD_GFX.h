// Display Library for parallel interface e-paper panels from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Examples from Good Display, available here: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// class GxEPD_HD_GFX : Base class for the template classes for buffered graphics and text drawing
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD_HD

#ifndef _GxEPD_HD_GFX_H_
#define _GxEPD_HD_GFX_H_

// optional, can be commented out if not needed
#include "GFX_FontExtension/GFX_FontExtension.h"

#ifndef _GFX_FontExtension_H_
// comment out to revert to Adafruit_GFX
#include <GFX.h>
#endif

#ifndef _GFX_H_
#include <Adafruit_GFX.h>
#endif

#ifndef _GFX_FontExtension_H_
#ifndef _GFX_H_
class GxEPD_HD_GFX : public Adafruit_GFX
#else
class GxEPD_HD_GFX : public GFX
#endif
#else
class GxEPD_HD_GFX : public GFX_FontExtension
#endif
{
  public:
#ifndef _GFX_FontExtension_H_
#ifndef _GFX_H_
    GxEPD_HD_GFX(GxEPD_HD_EPD& _epd_hd, int16_t w, int16_t h) : Adafruit_GFX(w, h), epd_hd(_epd_hd) {};
#else
    GxEPD_HD_GFX(GxEPD_HD_EPD& _epd_hd, int16_t w, int16_t h) : GFX(w, h), epd_hd(_epd_hd) {};
#endif
#else
    GxEPD_HD_GFX(GxEPD_HD_EPD& _epd_hd, int16_t w, int16_t h) : GFX_FontExtension(w, h), epd_hd(_epd_hd) {};
#endif
    virtual uint16_t pages() = 0;
    virtual uint16_t pageHeight() = 0;
    virtual bool mirror(bool m) = 0;
    virtual void init(Stream* pDiagnosticOutput = 0) = 0; // (pDiagnosticOutput = 0) : disabled
    virtual void fillScreen(uint16_t color) = 0; // 0x0 black, >0x0 white, to buffer
    virtual void display() = 0;
    virtual void displayWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation = true) = 0;
    virtual void setFullWindow() = 0;
    virtual void setPartialWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
    virtual void firstPage() = 0;
    virtual bool nextPage(bool norefresh = false) = 0;
    virtual void drawPaged(void (*drawCallback)(const void*), const void* pv) = 0;
    virtual void drawInvertedBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) = 0;
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    virtual void clearScreen(uint8_t value = 0xFF) = 0; // init controller memory and screen (default white)
    virtual void writeScreenBuffer(uint8_t value = 0xFF) = 0; // init controller memory (default white)
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    virtual void writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
    virtual void writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                   int16_t x, int16_t y, int16_t w, int16_t h) = 0;
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    virtual void drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
    virtual void drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                   int16_t x, int16_t y, int16_t w, int16_t h) = 0;
    virtual void refresh(bool partial_update_mode = false) = 0; // screen refresh from controller memory to full screen
    virtual void refresh(int16_t x, int16_t y, int16_t w, int16_t h) = 0; // screen refresh from controller memory, partial screen
    virtual void powerOff() = 0;
    virtual void hibernate() = 0;
  public:
    GxEPD_HD_EPD& epd_hd;
};

#endif

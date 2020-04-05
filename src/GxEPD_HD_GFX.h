// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#ifndef _GxEPD_HD_GFX_H_
#define _GxEPD_HD_GFX_H_

#include <Adafruit_GFX.h>

class GxEPD_HD_GFX : public Adafruit_GFX
{
  public:
    GxEPD_HD_GFX(GxEPD_HD_EPD& _epd_hd, int16_t w, int16_t h) : Adafruit_GFX(w, h), epd_hd(_epd_hd) {};
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
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    virtual void writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
    virtual void writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                                uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0) = 0;
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    virtual void drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
    virtual void drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                               uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0) = 0;
    virtual void refresh(bool partial_update_mode = false) = 0; // screen refresh from controller memory to full screen
    virtual void refresh(int16_t x, int16_t y, int16_t w, int16_t h) = 0; // screen refresh from controller memory, partial screen
    virtual void powerOff() = 0;
    virtual void hibernate() = 0;
  public:
    GxEPD_HD_EPD& epd_hd;
};

#endif

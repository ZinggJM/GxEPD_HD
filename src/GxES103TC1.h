// Display Library for parallel interface e-paper panels from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Examples from Good Display, available here: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Class GxES103TC1 : display class for ES103TC1 parallel interface e-paper display on IT8951 Driver HAT from Waveshare
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD_HD
//
// GxIT8951 class is based on Demo Example from Waveshare for Raspberry PI https://github.com/waveshare/IT8951/archive/master.zip
// Controller: IT8951 : https://www.waveshare.com/w/upload/1/18/IT8951_D_V0.2.4.3_20170728.pdf

#ifndef _GxES103TC1_H_
#define _GxES103TC1_H_

#include "GxEPD_HD_EPD.h"

class GxES103TC1 : public GxEPD_HD_EPD
{
  public:
    // attributes
    static const uint16_t WIDTH = 1872;
    static const uint16_t HEIGHT = 1404;
    static const GxEPD_HD::Panel panel = GxEPD_HD::ES103TC1;
    static const bool hasColor = false;
    static const bool hasPartialUpdate = true;
    static const bool hasFastPartialUpdate = true;
    // constructor
    GxES103TC1(GxEPD_HD_IOCTRL& io, uint16_t vcom_mV = 2000); // abs(vcom*1000)
    GxES103TC1(GxEPD_HD_IOCTRL& io, double vcom);
    // methods (virtual)
    void init(Stream* pDiagnosticOutput = 0); // (pDiagnosticOutput = 0) : disabled
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    void clearScreen(uint8_t value = 0xFF); // init controller memory and screen (default white)
    void writeScreenBuffer(uint8_t value = 0xFF); // init controller memory (default white)
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    void writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h);
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    void drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h);
    // screen refresh from controller memory to full screen
    void refresh(bool partial_update_mode = false);
    // screen refresh from controller memory, partial screen
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode = false);
    void powerOff();
    void hibernate();
  private:
    GxEPD_HD_IOCTRL& IO;
    uint16_t _vcom_mV; // abs(vcom*1000)
};

#endif

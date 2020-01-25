// Class GxGDE060BA : display class for GDE060BA on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
// To be used with "STM32F103VE" of "Generic STM32F103V series" of package "STM32 Boards (STM32Duino.com)" for Arduino.
// install package with Boards Manager after adding to preferences in additional Boards Manager URLs:
// https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html

#ifndef _GxGDE060BA_H_
#define _GxGDE060BA_H_

#include "GxEPD_HD_EPD.h"
#include "GxDESTM32T/GxDESTM32T.h"

class GxGDE060BA : public GxEPD_HD_EPD
{
  public:
    // attributes
    static const uint16_t WIDTH = 800;
    static const uint16_t HEIGHT = 600;
    static const GxEPD_HD::Panel panel = GxEPD_HD::GDE060BA;
    static const bool hasColor = false;
    static const bool hasPartialUpdate = true;
    static const bool hasFastPartialUpdate = true;
    // constructor
    //GxGDE060BA(GxDESTM32T& io);
    GxGDE060BA(GxEPD_HD_IOCTRL& io);
    // methods (virtual)
    void init(Stream* pDiagnosticOutput = 0); // (pDiagnosticOutput = 0) : disabled
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    void clearScreen(uint8_t value = 0xFF); // init controller memory and screen (default white)
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    void writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                                 uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0);
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    void drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                         uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0);
    // screen refresh from controller memory to full screen
    void refresh(bool partial_update_mode = false);
    // screen refresh from controller memory, partial screen
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode = false);
    void powerOff();
  private:
    //GxDESTM32T& IO;
    GxEPD_HD_IOCTRL& IO;
};

#endif

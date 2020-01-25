// Class GxDESTM32T : display IO class for GDE06BA on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
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

#ifndef _GxDESTM32T_H_
#define _GxDESTM32T_H_

#include <Arduino.h>
#include <Stream.h>
#include "../GxEPD_HD_EPD.h"
#include "TPS65185.h"
#include "AVT6203A.h"

class GxDESTM32T : public GxEPD_HD_IOCTRL
{
  public:
    GxDESTM32T();
    void init(GxEPD_HD::Panel panel, Stream* pDiagnosticOutput); // (pDiagnosticOutput = 0) : disabled
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    void clearScreen(uint8_t value = 0xFF); // init controller memory and screen (default white)
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    void writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void writeImage(uint8_t (*get)(uint32_t), uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0);
    void writeImagePart(uint8_t (*get)(uint32_t), uint32_t size, uint8_t depth, uint32_t width,
                        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0);
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    void drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void drawImage(uint8_t (*get)(uint32_t), uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0);
    void drawImagePart(uint8_t (*get)(uint32_t), uint32_t size, uint8_t depth, uint32_t width,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0);
    // screen refresh from controller memory to full screen
    void refresh(bool partial_update_mode = false);
    // screen refresh from controller memory, partial screen
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode = false);
    void powerOff();
    void updateWindow(const uint8_t* bitmap, uint32_t size, uint32_t width, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void writeFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth, uint8_t value);
    void drawFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth, uint8_t value);
  private:
    void epd_draw_gray(uint8_t gray);
    void Debug_str(const char *s);
  private:
    TPS65185 tps;
    AVT6203A avt;
    GxEPD_HD::Panel _panel;
    uint16_t _width;
    uint16_t _height;
    uint16_t _vcom;
    Stream* _pDiagnosticOutput;
    static const uint8_t bw2grey8[];
    static const uint16_t bw2grey16[];
    friend class DESTM32T_DEMO;
};

#endif

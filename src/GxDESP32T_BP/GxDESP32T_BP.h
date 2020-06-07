// Class GxDESP32T_BP : display IO class for GDE060BA on my DESP32T_BP (proto board) for TCon-11 parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html

#ifndef _GxDESP32T_BP_H_
#define _GxDESP32T_BP_H_

#include <Arduino.h>
#include <Stream.h>
#include "../GxEPD_HD_EPD.h"
#include "TPS65185_BP.h"
#include "AVT6203A_BP.h"
#include "STM32F103C8T6_IO.h"

class GxDESP32T_BP : public GxEPD_HD_IOCTRL
{
  public:
    GxDESP32T_BP();
    void init(GxEPD_HD::Panel panel, uint16_t vcom_mV, Stream* pDiagnosticOutput); // (pDiagnosticOutput = 0) : disabled
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
    void updateWindow(const uint8_t* bitmap, uint32_t size, uint32_t width, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void writeFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth, uint8_t value);
    void drawFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth, uint8_t value);
    void powerOff();
    void hibernate();
  private:
    void _wake_up();
    void epd_draw_gray(uint8_t gray);
    void Debug_str(const char *s);
  private:
    TPS65185_BP tps;
    AVT6203A_BP avt;
    STM32F103C8T6_IO IO;
    GxEPD_HD::Panel _panel;
    uint16_t _width;
    uint16_t _height;
    uint16_t _vcom;
    bool _power_is_on, _hibernating;
    Stream* _pDiagnosticOutput;
    static const uint8_t bw2grey8[];
    static const uint16_t bw2grey16[];
    friend class DESTM32T_DEMO;
};

#endif

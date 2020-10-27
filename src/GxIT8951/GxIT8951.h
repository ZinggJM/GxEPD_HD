// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// GxIT8951 class is based on Demo Example from Waveshare for Raspberry PI https://github.com/waveshare/IT8951/archive/master.zip
// Controller: IT8951 : https://www.waveshare.com/w/upload/1/18/IT8951_D_V0.2.4.3_20170728.pdf
//
// The GxIT8951 driver class supports the Waveshare e-Paper IT8951 Driver HAT connected with SPI for the ED060SCT 6" e-paper panel (parallel IF)
// https://www.waveshare.com/product/mini-pc/raspberry-pi/hats/6inch-e-paper-hat.htm
// This Driver HAT requires 5V power supply but works with 3.3V data lines; requires both MOSI and MISO SPI lines.
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#ifndef _GxIT8951_H_
#define _GxIT8951_H_

#include <Arduino.h>
#include <Stream.h>
#include "../GxEPD_HD_EPD.h"

class GxIT8951 : public GxEPD_HD_IOCTRL
{
  public:
    // attributes
    static const uint16_t reset_to_ready_time = 1800; // ms, e.g. 1721883us
    static const uint16_t power_on_time = 10; // ms, e.g. 3879us
    static const uint16_t power_off_time = 200; // ms, e.g. 109875us
    static const uint16_t full_refresh_time = 600; // ms, e.g. 573921us
    static const uint16_t partial_refresh_time = 300; // ms, e.g. 246948us
    static const uint16_t refresh_cmd_time = 10; // ms, e.g. 6460us
    static const uint16_t refresh_par_time = 2; // ms, e.g. 1921us
    static const uint16_t default_wait_time = 1; // ms, default busy check, needed?
    static const uint16_t diag_min_time = 3; // ms, e.g. > refresh_par_time
    static const uint16_t set_vcom_time = 40; // ms, e.g. 37833us
    // constructor
    GxIT8951(int8_t cs, int8_t dc, int8_t rst, int8_t busy);
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
    void powerOff();
    void hibernate();
  private:
    void _writeScreenBuffer(uint8_t value);
    void _refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode);
    void _send8pixel(uint8_t data);
    void _setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t bpp);
    void _PowerOn();
    void _PowerOff();
    void _InitDisplay();
    void _Init_Full();
    void _Init_Part();
    void _waitWhileBusy(const char* comment = 0, uint16_t busy_time = 5000);
    // IT8951
    uint16_t _transfer16(uint16_t value);
    void _writeCommand16(uint16_t c);
    void _writeData16(uint16_t d);
    void _writeData16(const uint16_t* d, uint32_t n);
    uint16_t _readData16();
    void _readData16(uint16_t* d, uint32_t n);
    void _IT8951SystemRun();
    void _IT8951StandBy();
    void _IT8951Sleep();
    uint16_t _IT8951ReadReg(uint16_t usRegAddr);
    void _IT8951WriteReg(uint16_t usRegAddr, uint16_t usValue);
    uint16_t _IT8951GetVCOM(void);
    void _IT8951SetVCOM(uint16_t vcom);
  private:
    struct IT8951DevInfoStruct
    {
      uint16_t usPanelW;
      uint16_t usPanelH;
      uint16_t usImgBufAddrL;
      uint16_t usImgBufAddrH;
      uint16_t usFWVersion[8];   //16 Bytes String
      uint16_t usLUTVersion[8];   //16 Bytes String
    };
    int8_t _cs, _dc, _rst, _busy;
    bool _initial_write, _initial_refresh;
    bool _power_is_on, _using_partial_mode, _hibernating;
    IT8951DevInfoStruct IT8951DevInfo;
    SPISettings _spi_settings;
    SPISettings _spi_settings_for_read;
    GxEPD_HD::Panel _panel;
    uint16_t _width;
    uint16_t _height;
    uint16_t _vcom;
    Stream* _pDiagnosticOutput;
};

#endif

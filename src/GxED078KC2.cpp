// Display Library for parallel interface e-paper panels from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Examples from Good Display, available here: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Class GxED078KC2 : display class for ED078KC2 parallel interface e-paper display on IT8951 Driver HAT from Waveshare
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

#include "GxED078KC2.h"

GxED078KC2::GxED078KC2(GxEPD_HD_IOCTRL& io, uint16_t vcom_mV) :
  GxEPD_HD_EPD(WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate), IO(io), _vcom_mV(vcom_mV)
{
}

GxED078KC2::GxED078KC2(GxEPD_HD_IOCTRL& io, double vcom) :
  GxEPD_HD_EPD(WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate), IO(io), _vcom_mV(abs(vcom * 1000))
{
}

void GxED078KC2::init(Stream* pDiagnosticOutput)
{
  IO.init(panel, _vcom_mV, pDiagnosticOutput);
}

void GxED078KC2::clearScreen(uint8_t value)
{
  IO.clearScreen(value);
}

void GxED078KC2::writeScreenBuffer(uint8_t value)
{
  IO.writeScreenBuffer(value);
}

void GxED078KC2::writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  IO.writeImage(bitmap, size, depth, x, y, w, h);
}

void GxED078KC2::writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                int16_t x, int16_t y, int16_t w, int16_t h)
{
  IO.writeImagePart(bitmap, size, depth, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h);
}

void GxED078KC2::drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  IO.drawImage(bitmap, size, depth, x, y, w, h);
}

void GxED078KC2::drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                int16_t x, int16_t y, int16_t w, int16_t h)
{
  IO.drawImagePart(bitmap, size, depth, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h);
}

void GxED078KC2::refresh(bool partial_update_mode)
{
  IO.refresh(partial_update_mode);
}

void GxED078KC2::refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode)
{
  IO.refresh(x, y, w, h, partial_update_mode);
}

void GxED078KC2::powerOff()
{
  IO.powerOff();
}

void GxED078KC2::hibernate()
{
  IO.hibernate();
}

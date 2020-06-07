// Display Library for parallel interface e-paper panels from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Examples from Good Display, available here: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Class GxGDEW080T5 : display class for GDEW080T5 on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD_HD

#include "GxGDEW080T5.h"

GxGDEW080T5::GxGDEW080T5(GxEPD_HD_IOCTRL& io, uint16_t vcom_mV) :
  GxEPD_HD_EPD(WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate), IO(io), _vcom_mV(vcom_mV)
{
}

GxGDEW080T5::GxGDEW080T5(GxEPD_HD_IOCTRL& io, double vcom) :
  GxEPD_HD_EPD(WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate), IO(io), _vcom_mV(abs(vcom * 1000))
{
}

void GxGDEW080T5::init(Stream* pDiagnosticOutput)
{
  IO.init(panel, _vcom_mV, pDiagnosticOutput);
}

void GxGDEW080T5::clearScreen(uint8_t value)
{
  IO.clearScreen(value);
}

void GxGDEW080T5::writeScreenBuffer(uint8_t value)
{
  IO.writeScreenBuffer(value);
}

void GxGDEW080T5::writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  IO.writeImage(bitmap, size, depth, x, y, w, h);
}

void GxGDEW080T5::writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                 int16_t x, int16_t y, int16_t w, int16_t h)
{
  IO.writeImagePart(bitmap, size, depth, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h);
}

void GxGDEW080T5::drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  IO.drawImage(bitmap, size, depth, x, y, w, h);
}

void GxGDEW080T5::drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                int16_t x, int16_t y, int16_t w, int16_t h)
{
  IO.drawImagePart(bitmap, size, depth, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h);
}

void GxGDEW080T5::refresh(bool partial_update_mode)
{
  IO.refresh(partial_update_mode);
}

void GxGDEW080T5::refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode)
{
  IO.refresh(x, y, w, h, partial_update_mode);
}

void GxGDEW080T5::powerOff()
{
  IO.powerOff();
}

void GxGDEW080T5::hibernate()
{
  IO.hibernate();
}

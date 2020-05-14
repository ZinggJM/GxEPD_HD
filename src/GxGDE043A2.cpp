// Display Library for parallel interface e-paper panels from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Examples from Good Display, available here: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Class GxGDE043A2 : display class for GDE043A2 on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD_HD

#include "GxGDE043A2.h"

GxGDE043A2::GxGDE043A2(GxEPD_HD_IOCTRL& io, uint16_t vcom_mV) :
  GxEPD_HD_EPD(WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate), IO(io), _vcom_mV(vcom_mV)
{
}

GxGDE043A2::GxGDE043A2(GxEPD_HD_IOCTRL& io, double vcom) :
  GxEPD_HD_EPD(WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate), IO(io), _vcom_mV(abs(vcom * 1000))
{
}

void GxGDE043A2::init(Stream* pDiagnosticOutput)
{
  IO.init(GxEPD_HD::GDE043A2, _vcom_mV, pDiagnosticOutput);
}

void GxGDE043A2::clearScreen(uint8_t value)
{
  IO.clearScreen(value);
}

void GxGDE043A2::writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (x >= WIDTH) return;
  if (y >= HEIGHT) return;
  if (x + w > WIDTH) w = WIDTH - x;
  if (y + h > HEIGHT) h = HEIGHT - y;
  uint16_t x8 = x - x % 8; // byte boundary
  uint16_t w8 = ((w + x - x8 + 7) / 8) * 8; // byte boundary
  IO.writeImage(bitmap, size, depth, x8, y, w8, h);
}

void GxGDE043A2::writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                                uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx, uint16_t dy)
{
  if (x + dx >= WIDTH) return;
  if (y + dy >= HEIGHT) return;
  if (x + dx + w > WIDTH) w = WIDTH - x - dx;
  if (y + dy + h > HEIGHT) h = HEIGHT - y - dy;
  uint16_t x8 = x - x % 8; // byte boundary
  uint16_t w8 = ((w + x - x8 + 7) / 8) * 8; // byte boundary
  IO.writeImagePart(bitmap, size, depth, width, x8, y, w8, h, dx, dy);
}

void GxGDE043A2::drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (x >= WIDTH) return;
  if (y >= HEIGHT) return;
  if (x + w > WIDTH) w = WIDTH - x;
  if (y + h > HEIGHT) h = HEIGHT - y;
  uint16_t x8 = x - x % 8; // byte boundary
  uint16_t w8 = ((w + x - x8 + 7) / 8) * 8; // byte boundary
  IO.drawImage(bitmap, size, depth, x8, y, w8, h);
}

void GxGDE043A2::drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                               uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx, uint16_t dy)
{
  if (x + dx >= WIDTH) return;
  if (y + dy >= HEIGHT) return;
  if (x + dx + w > WIDTH) w = WIDTH - x - dx;
  if (y + dy + h > HEIGHT) h = HEIGHT - y - dy;
  uint16_t x8 = x - x % 8; // byte boundary
  uint16_t w8 = ((w + x - x8 + 7) / 8) * 8; // byte boundary
  IO.drawImagePart(bitmap, size, depth, width, x8, y, w8, h, dx, dy);
}

void GxGDE043A2::refresh(bool partial_update_mode)
{
  IO.refresh(partial_update_mode);
}

void GxGDE043A2::refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode)
{
  IO.refresh(x, y, w, h, partial_update_mode);
}

void GxGDE043A2::powerOff()
{
  IO.powerOff();
}

void GxGDE043A2::hibernate()
{
  IO.hibernate();
}

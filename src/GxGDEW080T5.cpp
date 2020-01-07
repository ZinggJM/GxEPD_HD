// Class GxGDEW080T5 : display class for GDEW080T5 on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
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

#include "GxGDEW080T5.h"

GxGDEW080T5::GxGDEW080T5(GxDESTM32T& io) :
  GxEPD_HD_EPD(WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate), IO(io)
{
}

void GxGDEW080T5::init(Stream* pDiagnosticOutput)
{
  IO.init(GxEPD_HD::GDEW080T5, pDiagnosticOutput);
}

void GxGDEW080T5::clearScreen(uint8_t value)
{
  IO.clearScreen(value);
}

void GxGDEW080T5::writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (x >= WIDTH) return;
  if (y >= HEIGHT) return;
  if (x + w > WIDTH) w = WIDTH - x;
  if (y + h > HEIGHT) h = HEIGHT - y;
  uint16_t x8 = x - x % 8; // byte boundary
  uint16_t w8 = ((w + x - x8 + 7) / 8) * 8; // byte boundary
  IO.writeImage(bitmap, size, depth, x8, y, w8, h);
}

void GxGDEW080T5::writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
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

void GxGDEW080T5::drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (x >= WIDTH) return;
  if (y >= HEIGHT) return;
  if (x + w > WIDTH) w = WIDTH - x;
  if (y + h > HEIGHT) h = HEIGHT - y;
  uint16_t x8 = x - x % 8; // byte boundary
  uint16_t w8 = ((w + x - x8 + 7) / 8) * 8; // byte boundary
  IO.drawImage(bitmap, size, depth, x8, y, w8, h);
}

void GxGDEW080T5::drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
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

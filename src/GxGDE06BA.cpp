// Class GxGDE06BA : display class for GDE06BA on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
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

#include "GxGDE06BA.h"

GxGDE06BA::GxGDE06BA(GxDESTM32T& io) :
  GxEPD_HD_EPD(WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate), IO(io)
{
}

void GxGDE06BA::init(Stream* pDiagnosticOutput)
{
  IO.init(GxEPD_HD::GDE060BA, pDiagnosticOutput);
}

void GxGDE06BA::clearScreen(uint8_t value)
{
}

void GxGDE06BA::writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (x >= WIDTH) return;
  if (y >= HEIGHT) return;
  if (x + w > WIDTH) w = WIDTH - x;
  if (y + h > HEIGHT) h = HEIGHT - y;
  uint16_t x8 = x - x % 8; // byte boundary
  uint16_t w8 = ((w + x - x8 + 7) / 8) * 8; // byte boundary
  IO.writeImage(bitmap, size, depth, x8, y, w8, h);
}

void GxGDE06BA::writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
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

void GxGDE06BA::drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (x >= WIDTH) return;
  if (y >= HEIGHT) return;
  if (x + w > WIDTH) w = WIDTH - x;
  if (y + h > HEIGHT) h = HEIGHT - y;
  uint16_t x8 = x - x % 8; // byte boundary
  uint16_t w8 = ((w + x - x8 + 7) / 8) * 8; // byte boundary
  IO.drawImage(bitmap, size, depth, x8, y, w8, h);
}

void GxGDE06BA::drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
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

void GxGDE06BA::refresh(bool partial_update_mode)
{
  IO.refresh(partial_update_mode);
}

void GxGDE06BA::refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode)
{
  IO.refresh(x, y, w, h, partial_update_mode);
}

void GxGDE06BA::powerOff()
{
  IO.powerOff();
}

void GxGDE06BA::demo()
{

}


// Display Library for parallel interface e-paper panels from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Examples from Good Display, available here: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// class GFX_FontExtension : Subclass for Adafruit_GFX to add multiple UTF-8 fonts
//
// based on code extracted from https://github.com/adafruit/Adafruit-GFX-Library and https://github.com/Bodmer/Adafruit-GFX-Library#enhanced-fork
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD_HD
//
// Font files can be created using the fontconverter from https://github.com/Bodmer/GFX_Font_Converter
// see tutorial https://www.youtube.com/watch?v=L8MmTISmwZ8

#include "GFX_FontExtension.h"

#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#endif

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

inline GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c) {
#ifdef __AVR__
  return &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
#else
  // expression in __AVR__ section may generate "dereferencing type-punned
  // pointer will break strict-aliasing rules" warning In fact, on other
  // platforms (such as STM32) there is no need to do this pointer magic as
  // program memory may be read in a usual way So expression may be simplified
  return gfxFont->glyph + c;
#endif //__AVR__
}

void GFX_FontExtension::drawChar(int16_t x, int16_t y, uint16_t c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y, const GFXfont* gfxFont)
{
  if (!gfxFont) // 'Classic' built-in font
  {
#ifndef _GFX_H_
    Adafruit_GFX::drawChar(x, y, c, color, bg, size_x, size_y);
#else
    GFX::drawChar(x, y, c, color, bg, size_x, size_y);
#endif
  }
  else // Custom font
  {
    // Character is assumed previously filtered by write() to eliminate
    // newlines, returns, non-printable characters, etc.  Calling
    // drawChar() directly with 'bad' characters of font may cause mayhem!

    c -= pgm_read_word(&gfxFont->first);
    GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
    uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

    uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
    uint8_t  w  = pgm_read_byte(&glyph->width),
             h  = pgm_read_byte(&glyph->height);
    int8_t   xo = pgm_read_byte(&glyph->xOffset),
             yo = pgm_read_byte(&glyph->yOffset);
    uint8_t  xx, yy, bits = 0, bit = 0;
    int16_t  xo16 = 0, yo16 = 0;

    if (size_x > 1 || size_y > 1)
    {
      xo16 = xo;
      yo16 = yo;
    }
    for (yy = 0; yy < h; yy++)
    {
      for (xx = 0; xx < w; xx++)
      {
        if (!(bit++ & 7))
        {
          bits = pgm_read_byte(&bitmap[bo++]);
        }
        if (bits & 0x80)
        {
          if (size_x == 1 && size_y == 1)
          {
            drawPixel(x + xo + xx, y + yo + yy, color);
          }
          else
          {
            fillRect(x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y, size_x, size_y, color);
          }
        }
        bits <<= 1;
      }
    }

  } // End classic vs custom font
}

uint16_t GFX_FontExtension::decodeUTF8(uint8_t c)
{
  // 7 bit Unicode Code Point
  if ((c & 0x80) == 0x00)
  {
    decoderState = 0;
    return (uint16_t)c;
  }
  if (decoderState == 0)
  {
    // 11 bit Unicode Code Point
    if ((c & 0xE0) == 0xC0)
    {
      decoderBuffer = ((c & 0x1F) << 6); // Save first 5 bits
      decoderState = 1;
      return 0;
    }
    // 16 bit Unicode Code Point
    if ((c & 0xF0) == 0xE0)
    {
      decoderBuffer = ((c & 0x0F) << 12); // Save first 4 bits
      decoderState = 2;
      return 0;
    }
    // 21 bit Unicode  Code Point not supported so fall-back to extended ASCII
    if ((c & 0xF8) == 0xF0) return (uint16_t)c;
  }
  else
  {
    if (decoderState == 2)
    {
      decoderBuffer |= ((c & 0x3F) << 6); // Add next 6 bits of 16 bit code point
      decoderState--;
      return 0;
    }
    else // decoderState must be == 1
    {
      decoderBuffer |= (c & 0x3F); // Add last 6 bits of code point
      decoderState = 0;
      return decoderBuffer;
    }
  }
  decoderState = 0;
  return (uint16_t)c; // fall-back to extended ASCII
}

size_t GFX_FontExtension::write(uint8_t data)
{
  uint16_t c = (uint16_t)data;
  c = decodeUTF8(data);
  if (c == 0) return 1;
  const GFXfont* gfxFont = gfxFonts[c / 256];
  if (!gfxFont) // 'Classic' built-in font
  {
    if (c > 255) return 1; // Stop 16 bit characters
    if (c == '\n') // Newline?
    {
      cursor_x  = 0;              // Reset x to zero,
      cursor_y += textsize_y * 8; // advance y one line
    }
    else if (c != '\r') // Ignore carriage returns
    {
      if (wrap && ((cursor_x + textsize_x * 6) > _width)) // Off right?
      {
        cursor_x  = 0;              // Reset x to zero,
        cursor_y += textsize_y * 8; // advance y one line
      }
#ifndef _GFX_H_
      Adafruit_GFX::drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
#else
      GFX::drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
#endif
      cursor_x += textsize_x * 6;   // Advance x one char
    }
  }
  else // Custom font
  {
    if (c == '\n')
    {
      cursor_x  = 0;
      cursor_y += (int16_t)textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
    }
    else if (c != '\r')
    {
      uint16_t first = pgm_read_word(&gfxFont->first);
      if ((c >= first) && (c <= pgm_read_word(&gfxFont->last)))
      {
        GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer( &gfxFont->glyph))[c - first]);
        uint8_t   w     = pgm_read_byte(&glyph->width),
                  h     = pgm_read_byte(&glyph->height);
        if ((w > 0) && (h > 0)) // Is there an associated bitmap?
        {
          int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
          if (wrap && ((cursor_x + textsize_x * (xo + w)) > _width))
          {
            cursor_x  = 0;
            cursor_y += (int16_t)textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
          }
          drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y, gfxFont);
        }
        cursor_x += (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize_x;
      }
    }
  }
  return 1;
}

void GFX_FontExtension::setFont(const GFXfont* f)
{
  if (f) gfxFonts[f->first / 256] = f;
  else gfxFonts[0] = 0;
}

void GFX_FontExtension::setFont(const GFXfont* f, uint8_t page)
{
  if (f && (page == f->first / 256)) gfxFonts[page] = f;
  else if (!f) gfxFonts[page] = 0;
}

void GFX_FontExtension::getTextBounds(const char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
  uint8_t c; // Current character

  *x1 = x;
  *y1 = y;
  *w = *h = 0;

  int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;

  while ((c = *str++))
    charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

  if (maxx >= minx) {
    *x1 = minx;
    *w = maxx - minx + 1;
  }
  if (maxy >= miny) {
    *y1 = miny;
    *h = maxy - miny + 1;
  }
}

void GFX_FontExtension::getTextBounds(const String & str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
  if (str.length() != 0)
  {
    getTextBounds(const_cast<char *>(str.c_str()), x, y, x1, y1, w, h);
  }
}

void GFX_FontExtension::getTextBounds(const __FlashStringHelper * str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
  uint8_t *s = (uint8_t *)str, c;

  *x1 = x;
  *y1 = y;
  *w = *h = 0;

  int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;

  while ((c = pgm_read_byte(s++)))
    charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

  if (maxx >= minx) {
    *x1 = minx;
    *w = maxx - minx + 1;
  }
  if (maxy >= miny) {
    *y1 = miny;
    *h = maxy - miny + 1;
  }
}

void GFX_FontExtension::charBounds(unsigned char uc8, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy)
{
  uint16_t c = (uint16_t)uc8;
  c = decodeUTF8(uc8);
  if (c == 0) return;
  const GFXfont* gfxFont = gfxFonts[c / 256];
  if (gfxFont) {

    if (c == '\n') { // Newline?
      *x = 0;        // Reset x to zero, advance y by one line
      *y += textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
    } else if (c != '\r') { // Not a carriage return; is normal char
      uint8_t first = pgm_read_byte(&gfxFont->first),
              last = pgm_read_byte(&gfxFont->last);
      if ((c >= first) && (c <= last)) { // Char present in this font?
        GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c - first);
        uint8_t gw = pgm_read_byte(&glyph->width),
                gh = pgm_read_byte(&glyph->height),
                xa = pgm_read_byte(&glyph->xAdvance);
        int8_t xo = pgm_read_byte(&glyph->xOffset),
               yo = pgm_read_byte(&glyph->yOffset);
        if (wrap && ((*x + (((int16_t)xo + gw) * textsize_x)) > _width)) {
          *x = 0; // Reset x to zero, advance y by one line
          *y += textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        }
        int16_t tsx = (int16_t)textsize_x, tsy = (int16_t)textsize_y,
                x1 = *x + xo * tsx, y1 = *y + yo * tsy, x2 = x1 + gw * tsx - 1,
                y2 = y1 + gh * tsy - 1;
        if (x1 < *minx)
          *minx = x1;
        if (y1 < *miny)
          *miny = y1;
        if (x2 > *maxx)
          *maxx = x2;
        if (y2 > *maxy)
          *maxy = y2;
        *x += xa * tsx;
      }
    }

  } else { // Default font

    if (c == '\n') {        // Newline?
      *x = 0;               // Reset x to zero,
      *y += textsize_y * 8; // advance y one line
      // min/max x/y unchaged -- that waits for next 'normal' character
    } else if (c != '\r') { // Normal char; ignore carriage returns
      if (wrap && ((*x + textsize_x * 6) > _width)) { // Off right?
        *x = 0;                                       // Reset x to zero,
        *y += textsize_y * 8;                         // advance y one line
      }
      int x2 = *x + textsize_x * 6 - 1, // Lower-right pixel of char
          y2 = *y + textsize_y * 8 - 1;
      if (x2 > *maxx)
        *maxx = x2; // Track max x, y
      if (y2 > *maxy)
        *maxy = y2;
      if (*x < *minx)
        *minx = *x; // Track min x, y
      if (*y < *miny)
        *miny = *y;
      *x += textsize_x * 6; // Advance x one char
    }
  }
}

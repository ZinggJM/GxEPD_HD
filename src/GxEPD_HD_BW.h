// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/Gxepd_hd

#ifndef _GxEPD_HD_BW_H_
#define _GxEPD_HD_BW_H_

#include <Adafruit_GFX.h>
#include "GxEPD_HD_EPD.h"
#include "GxGDE06BA.h"
#include "GxEPD_HD_GFX.h"

template<typename GxEPD_HD_Type, const uint16_t page_height>
class GxEPD_HD_BW : public GxEPD_HD_GFX
{
  public:
    GxEPD_HD_Type epd_hd;
    GxEPD_HD_BW(GxEPD_HD_Type epd_hd_instance) : epd_hd(epd_hd_instance), GxEPD_HD_GFX(epd_hd, GxEPD_HD_Type::WIDTH, GxEPD_HD_Type::HEIGHT)
    {
      _page_height = page_height;
      _pages = (HEIGHT / _page_height) + ((HEIGHT % _page_height) > 0);
      _reverse = false;
      _using_partial_mode = false;
      _current_page = 0;
      setFullWindow();
    }

    uint16_t pages()
    {
      return _pages;
    }

    uint16_t pageHeight()
    {
      return _page_height;
    }

    bool mirror(bool m)
    {
      swap (_mirror, m);
      return m;
    }

    void drawPixel(int16_t x, int16_t y, uint16_t color)
    {
      if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;
      if (_mirror) x = width() - x - 1;
      // check rotation, move pixel around if necessary
      switch (getRotation())
      {
        case 1:
          swap(x, y);
          x = WIDTH - x - 1;
          break;
        case 2:
          x = WIDTH - x - 1;
          y = HEIGHT - y - 1;
          break;
        case 3:
          swap(x, y);
          y = HEIGHT - y - 1;
          break;
      }
      // transpose partial window to 0,0
      x -= _pw_x;
      y -= _pw_y;
      // adjust for current page
      y -= _current_page * _page_height;
      if (_reverse) y = _page_height - y - 1;
      // check if in current page
      if ((y < 0) || (y >= _page_height)) return;
      uint16_t i = x / 8 + y * (_pw_w / 8);
      if (color)
        _buffer[i] = (_buffer[i] | (1 << (7 - x % 8)));
      else
        _buffer[i] = (_buffer[i] & (0xFF ^ (1 << (7 - x % 8))));
    }

    void init(uint32_t serial_diag_bitrate = 0) // = 0 : disabled
    {
      epd_hd.init(serial_diag_bitrate);
      _using_partial_mode = false;
      _current_page = 0;
      setFullWindow();
    }

    void fillScreen(uint16_t color) // 0x0 black, >0x0 white, to buffer
    {
      uint8_t data = (color == GxEPD_BLACK) ? 0x00 : 0xFF;
      for (uint16_t x = 0; x < sizeof(_buffer); x++)
      {
        _buffer[x] = data;
      }
    }

    // display buffer content to screen, useful for full screen buffer
    void display()
    {
      epd_hd.drawImage(_buffer, sizeof(_buffer), 1, 0, 0, WIDTH, HEIGHT);
    }

    // display part of buffer content to screen, useful for full screen buffer
    void displayWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation = true)
    {
      if (using_rotation) _rotate(x, y, w, h);
      epd_hd.drawImagePart(_buffer, sizeof(_buffer), 1, WIDTH, x, y, w, h);
    }

    void setFullWindow()
    {
      _using_partial_mode = false;
      _pw_x = 0;
      _pw_y = 0;
      _pw_w = WIDTH;
      _pw_h = HEIGHT;
    }

    void setPartialWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
      _rotate(x, y, w, h);
      _using_partial_mode = true;
      _pw_x = gx_uint16_min(x, WIDTH);
      _pw_y = gx_uint16_min(y, HEIGHT);
      _pw_w = gx_uint16_min(w, WIDTH - _pw_x);
      _pw_h = gx_uint16_min(h, HEIGHT - _pw_y);
      // make _pw_x, _pw_w multiple of 8
      _pw_w += _pw_x % 8;
      if (_pw_w % 8 > 0) _pw_w += 8 - _pw_w % 8;
      _pw_x -= _pw_x % 8;
    }

    void firstPage()
    {
      fillScreen(GxEPD_WHITE);
      _current_page = 0;
      _second_phase = false;
    }

    bool nextPage()
    {
      if (1 == _pages)
      {
        if (_using_partial_mode)
        {
          uint32_t offset = _reverse ? (HEIGHT - _pw_h) * _pw_w / 8 : 0;
          epd_hd.writeImage(_buffer + offset, sizeof(_buffer), 1, _pw_x, _pw_y, _pw_w, _pw_h);
          epd_hd.refresh(_pw_x, _pw_y, _pw_w, _pw_h, true);
        }
        else
        {
          epd_hd.writeImage(_buffer, sizeof(_buffer), 1, 0, 0, WIDTH, HEIGHT);
          epd_hd.refresh(false);
        }
        return false;
      }
      uint16_t page_ys = _current_page * _page_height;
      if (_using_partial_mode)
      {
        uint16_t page_ye = _current_page < (_pages - 1) ? page_ys + _page_height : HEIGHT;
        uint16_t dest_ys = _pw_y + page_ys; // transposed
        uint16_t dest_ye = gx_uint16_min(_pw_y + _pw_h, _pw_y + page_ye);
        if (dest_ye > dest_ys)
        {
          uint32_t offset = _reverse ? (_page_height - (dest_ye - dest_ys)) * _pw_w / 8 : 0;
          epd_hd.writeImage(_buffer + offset, sizeof(_buffer), 1, _pw_x, dest_ys, _pw_w, dest_ye - dest_ys);
        }
        _current_page++;
        if (_current_page == _pages)
        {
          _current_page = 0;
          epd_hd.refresh(_pw_x, _pw_y, _pw_w, _pw_h, true);
          return false;
        }
        fillScreen(GxEPD_WHITE);
        return true;
      }
      else
      {
        epd_hd.writeImage(_buffer, sizeof(_buffer), 1, 0, page_ys, WIDTH, gx_uint16_min(_page_height, HEIGHT - page_ys));
        _current_page++;
        if (_current_page == _pages)
        {
          _current_page = 0;
          epd_hd.refresh(false);
          //epd_hd.powerOff();
          return false;
        }
        fillScreen(GxEPD_WHITE);
        return true;
      }
    }

    // GxEPD style paged drawing; drawCallback() is called as many times as needed
    void drawPaged(void (*drawCallback)(const void*), const void* pv)
    {
      if (_using_partial_mode)
      {
        for (_current_page = 0; _current_page < _pages; _current_page++)
        {
          uint16_t page_ys = _current_page * _page_height;
          uint16_t page_ye = _current_page < (_pages - 1) ? page_ys + _page_height : HEIGHT;
          uint16_t dest_ys = _pw_y + page_ys; // transposed
          uint16_t dest_ye = gx_uint16_min(_pw_y + _pw_h, _pw_y + page_ye);
          if (dest_ye > dest_ys)
          {
            fillScreen(GxEPD_WHITE);
            drawCallback(pv);
            uint32_t offset = _reverse ? (_page_height - (dest_ye - dest_ys)) * _pw_w / 8 : 0;
            epd_hd.writeImage(_buffer + offset, sizeof(_buffer), 1, _pw_x, dest_ys, _pw_w, dest_ye - dest_ys);
          }
        }
        epd_hd.refresh(_pw_x, _pw_y, _pw_w, _pw_h, true);
      }
      else
      {
        for (_current_page = 0; _current_page < _pages; _current_page++)
        {
          uint16_t page_ys = _current_page * _page_height;
          fillScreen(GxEPD_WHITE);
          drawCallback(pv);
          epd_hd.writeImage(_buffer, sizeof(_buffer), 1, 0, page_ys, WIDTH, gx_uint16_min(_page_height, HEIGHT - page_ys));
        }
        epd_hd.refresh(false);
      }
      _current_page = 0;
    }

    void drawInvertedBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
    {
      // taken from Adafruit_GFX.cpp, modified
      int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
      uint8_t byte = 0;
      for (int16_t j = 0; j < h; j++)
      {
        for (int16_t i = 0; i < w; i++ )
        {
          if (i & 7) byte <<= 1;
          else
          {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
            byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
#else
            byte = bitmap[j * byteWidth + i / 8];
#endif
          }
          if (!(byte & 0x80))
          {
            drawPixel(x + i, y + j, color);
          }
        }
      }
    }

    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    void clearScreen(uint8_t value = 0xFF) // init controller memory and screen (default white)
    {
      epd_hd.clearScreen(value);
    }
    void writeScreenBuffer(uint8_t value = 0xFF) // init controller memory (default white)
    {
      epd_hd.writeScreenBuffer(value);
    }
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    void writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
      epd_hd.writeImage(bitmap, size, depth, x, y, w, h);
    }
    void writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0)
    {
      epd_hd.writeImagePart(bitmap, size, depth, width, x, y, w, h, dx, dy);
    }
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    void drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
      epd_hd.drawImage(bitmap, size, depth, x, y, w, h);
    }
    void drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx = 0, uint16_t dy = 0)
    {
      epd_hd.drawImagePart(bitmap, size, depth, width, x, y, w, h, dx, dy);
    }
    void refresh(bool partial_update_mode = false) // screen refresh from controller memory to full screen
    {
      epd_hd.refresh(partial_update_mode);
    }
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h) // screen refresh from controller memory, partial screen
    {
      epd_hd.refresh(x, y, w, h);
    }
    void powerOff()
    {
      epd_hd.powerOff();
    }
  private:
    template <typename T> static inline void
    swap(T & a, T & b)
    {
      T t = a;
      a = b;
      b = t;
    };
    static inline uint16_t gx_uint16_min(uint16_t a, uint16_t b)
    {
      return (a < b ? a : b);
    };
    static inline uint16_t gx_uint16_max(uint16_t a, uint16_t b)
    {
      return (a > b ? a : b);
    };
    void _rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h)
    {
      switch (getRotation())
      {
        case 1:
          swap(x, y);
          swap(w, h);
          x = WIDTH - x - w;
          break;
        case 2:
          x = WIDTH - x - w;
          y = HEIGHT - y - h;
          break;
        case 3:
          swap(x, y);
          swap(w, h);
          y = HEIGHT - y - h;
          break;
      }
    }
  private:
    uint8_t _buffer[(GxEPD_HD_Type::WIDTH / 8) * page_height];
    bool _using_partial_mode, _second_phase, _mirror, _reverse;
    uint16_t _width_bytes, _pixel_bytes;
    int16_t _current_page;
    uint16_t _pages, _page_height;
    uint16_t _pw_x, _pw_y, _pw_w, _pw_h;
};

#endif


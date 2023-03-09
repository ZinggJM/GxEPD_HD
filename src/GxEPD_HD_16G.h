// Display Library for parallel interface e-paper panels from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Examples from Good Display, available here: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// class GxEPD_HD_BW : Template class for buffered graphics and text drawing in 16 grey levels, supporting paged drawing
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD_HD

#ifndef _GxEPD_HD_16G_H_
#define _GxEPD_HD_16G_H_

#include "GxEPD_HD_EPD.h"
#include "GxGDE043A2.h"
#include "GxGDE060BA.h"
#include "GxGDE060F3.h"
#include "GxGDEW080T5.h"
#include "GxED060SCT.h"
#include "GxED060KC1.h"
#include "GxED078KC2.h"
#include "GxES103TC1.h"
#include "GxEPD_HD_GFX.h"

template<typename GxEPD_HD_Type, const uint16_t page_height>
class GxEPD_HD_16G : public GxEPD_HD_GFX
{
  public:
    static const uint16_t DEPTH = 4; // 4 bits per pixel, 16 grey levels
    static const uint16_t GREYLEVELS = 16;
    GxEPD_HD_Type& epd_hd;
    GxEPD_HD_16G(GxEPD_HD_Type& epd_hd_instance) : GxEPD_HD_GFX(epd_hd_instance, GxEPD_HD_Type::WIDTH, GxEPD_HD_Type::HEIGHT), epd_hd(epd_hd_instance)
    {
      _page_height = page_height;
      _pages = (HEIGHT / _page_height) + ((HEIGHT % _page_height) > 0);
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
      _swap_ (_mirror, m);
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
          _swap_(x, y);
          x = WIDTH - x - 1;
          break;
        case 2:
          x = WIDTH - x - 1;
          y = HEIGHT - y - 1;
          break;
        case 3:
          _swap_(x, y);
          y = HEIGHT - y - 1;
          break;
      }
      // transpose partial window to 0,0
      x -= _pw_x;
      y -= _pw_y;
      // clip to (partial) window
      if ((x < 0) || (x >= _pw_w) || (y < 0) || (y >= _pw_h)) return;
      // adjust for current page
      y -= _current_page * _page_height;
      // check if in current page
      if ((y < 0) || (y >= _page_height)) return;
      uint16_t i = x / (8 / DEPTH) + y * (_pw_w / (8 / DEPTH));
      uint8_t grey = color == 0 ? 0 : uint8_t((((color & 0xF800) >> 8) + ((color & 0x07E0) >> 3) + ((color & 0x001F) << 3) - 1) / 3); // 0..249 (0xF9) // GxEPD_LIGHTGREY is one too high
      if (x % 2) _buffer[i] = ((_buffer[i] & 0xF0) | (grey >> 4));
      else _buffer[i] = ((_buffer[i] & 0x0F) | (grey & 0xF0));
    }

    void init(Stream* pDiagnosticOutput = 0) // (pDiagnosticOutput = 0) : disabled
    {
      epd_hd.init(pDiagnosticOutput);
      _using_partial_mode = false;
      _current_page = 0;
      setFullWindow();
    }

    void fillScreen(uint16_t color)
    {
      uint8_t grey = color == 0 ? 0 : uint8_t((((color & 0xF800) >> 8) + ((color & 0x07E0) >> 3) + ((color & 0x001F) << 3) - 1) / 3); // 0..255
      uint8_t data = (grey / 16) * 0x11;
      for (uint32_t x = 0; x < sizeof(_buffer); x++)
      {
        _buffer[x] = data;
      }
    }

    // display buffer content to screen, useful for full screen buffer
    void display()
    {
      epd_hd.drawImage(_buffer, sizeof(_buffer), DEPTH, 0, 0, WIDTH, HEIGHT);
    }

    // transfer buffer content to controller buffer, useful for full screen buffer
    void transfer()
    {
      epd_hd.writeImage(_buffer, sizeof(_buffer), DEPTH, 0, 0, WIDTH, HEIGHT);
    }

    // display part of buffer content to screen, useful for full screen buffer
    void displayWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation = true)
    {
      x = gx_uint16_min(x, width());
      y = gx_uint16_min(y, height());
      w = gx_uint16_min(w, width() - x);
      h = gx_uint16_min(h, height() - y);
      if (using_rotation) _rotate(x, y, w, h);
      epd_hd.drawImagePart(_buffer, sizeof(_buffer), DEPTH, x, y, WIDTH, HEIGHT, x, y, w, h);
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
      _pw_x = gx_uint16_min(x, width());
      _pw_y = gx_uint16_min(y, height());
      _pw_w = gx_uint16_min(w, width() - _pw_x);
      _pw_h = gx_uint16_min(h, height() - _pw_y);
      _rotate(_pw_x, _pw_y, _pw_w, _pw_h);
      _using_partial_mode = true;
      //
      // make _pw_x, _pw_w multiple of 2, needed because of graphics buffer (bytes)
      //_pw_w += _pw_x % 2;
      //if (_pw_w % 2 > 0) _pw_w += 2 - _pw_w % 2;
      //_pw_x -= _pw_x % 2;
      //
      // make _pw_x, _pw_w multiple of 4, for controller best fit: 
      // "4 bpp mode – X Start position must be multiples of 4"
      _pw_w += _pw_x % 4;
      if (_pw_w % 4 > 0) _pw_w += 4 - _pw_w % 4;
      _pw_x -= _pw_x % 4;
    }

    void firstPage()
    {
      fillScreen(GxEPD_WHITE);
      _current_page = 0;
      _second_phase = false;
    }

    bool nextPage(bool norefresh = false)
    {
      if (1 == _pages)
      {
        if (_using_partial_mode)
        {
          epd_hd.writeImage(_buffer, sizeof(_buffer), DEPTH, _pw_x, _pw_y, _pw_w, _pw_h);
          if (!norefresh) epd_hd.refresh(_pw_x, _pw_y, _pw_w, _pw_h, false);
        }
        else
        {
          epd_hd.writeImage(_buffer, sizeof(_buffer), DEPTH, 0, 0, WIDTH, HEIGHT);
          if (!norefresh) epd_hd.refresh(false);
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
          epd_hd.writeImage(_buffer, sizeof(_buffer), DEPTH, _pw_x, dest_ys, _pw_w, dest_ye - dest_ys);
        }
        _current_page++;
        if (_current_page == _pages)
        {
          _current_page = 0;
          if (!norefresh) epd_hd.refresh(_pw_x, _pw_y, _pw_w, _pw_h, false);
          return false;
        }
        fillScreen(GxEPD_WHITE);
        return true;
      }
      else
      {
        epd_hd.writeImage(_buffer, sizeof(_buffer), DEPTH, 0, page_ys, WIDTH, gx_uint16_min(_page_height, HEIGHT - page_ys));
        _current_page++;
        if (_current_page == _pages)
        {
          _current_page = 0;
          if (!norefresh) epd_hd.refresh(false);
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
            epd_hd.writeImage(_buffer, sizeof(_buffer), DEPTH, _pw_x, dest_ys, _pw_w, dest_ye - dest_ys);
          }
        }
        epd_hd.refresh(_pw_x, _pw_y, _pw_w, _pw_h, false);
      }
      else
      {
        for (_current_page = 0; _current_page < _pages; _current_page++)
        {
          uint16_t page_ys = _current_page * _page_height;
          fillScreen(GxEPD_WHITE);
          drawCallback(pv);
          epd_hd.writeImage(_buffer, sizeof(_buffer), DEPTH, 0, page_ys, WIDTH, gx_uint16_min(_page_height, HEIGHT - page_ys));
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
    void writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h)
    {
      epd_hd.writeImagePart(bitmap, size, depth, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h);
    }
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    void drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
      epd_hd.drawImage(bitmap, size, depth, x, y, w, h);
    }
    void drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h)
    {
      epd_hd.drawImagePart(bitmap, size, depth, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h);
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
    void hibernate()
    {
      //Serial.println("GxEPD_HD_BW::hibernate()");
      epd_hd.hibernate();
    }
  private:
    template <typename T> static inline void
    _swap_(T & a, T & b)
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
          _swap_(x, y);
          _swap_(w, h);
          x = WIDTH - x - w;
          break;
        case 2:
          x = WIDTH - x - w;
          y = HEIGHT - y - h;
          break;
        case 3:
          _swap_(x, y);
          _swap_(w, h);
          y = HEIGHT - y - h;
          break;
      }
    }
  private:
    uint8_t _buffer[(GxEPD_HD_Type::WIDTH / 8) * DEPTH * page_height];
    bool _using_partial_mode, _second_phase, _mirror;
    uint16_t _width_bytes, _pixel_bytes;
    int16_t _current_page;
    uint16_t _pages, _page_height;
    uint16_t _pw_x, _pw_y, _pw_w, _pw_h;
};

#endif

// class GxEPD_HD : Base Class for Display Classes for e-Paper Displays from Dalian Good Display Co., Ltd.: www.good-display.com
//
// based on Demo Examples from Good Display, available here: http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD_HD

#ifndef _GxEPD_HD_H_
#define _GxEPD_HD_H_

#include <Arduino.h>
#include <Adafruit_GFX.h>

// the only colors supported by any of these displays; mapping of other colors is class specific
#define GxEPD_BLACK     0x0000
#define GxEPD_DARKGREY  0x7BEF      /* 128, 128, 128 */
#define GxEPD_LIGHTGREY 0xC618      /* 192, 192, 192 */
#define GxEPD_WHITE     0xFFFF
#define GxEPD_RED       0xF800      /* 255,   0,   0 */

class GxEPD_HD : public Adafruit_GFX
{
  public:
    // bitmap presentation modes may be partially implemented by subclasses
    enum bm_mode //BM_ModeSet
    {
      bm_normal = 0,
      bm_default = 1, // for use for BitmapExamples
      // these potentially can be combined
      bm_invert = (1 << 1),
      bm_flip_x = (1 << 2),
      bm_flip_y = (1 << 3),
      bm_r90 = (1 << 4),
      bm_r180 = (1 << 5),
      bm_r270 = bm_r90 | bm_r180,
      bm_partial_update = (1 << 6),
      bm_invert_red = (1 << 7),
      bm_transparent = (1 << 8)
    };
  public:
    GxEPD_HD(int16_t w, int16_t h) : Adafruit_GFX(w, h) {};
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;
    virtual void init(Stream* pDiagnosticOutput = 0) = 0; // (pDiagnosticOutput = 0) : disabled
    virtual void fillScreen(uint16_t color) = 0; // to buffer
    virtual void update(void) = 0;
    // to buffer, may be cropped, drawPixel() used, update needed, subclass may support some modes
    virtual void drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t m = bm_normal) = 0;
    // to buffer, may be cropped, drawPixel() used, update needed, subclass may support some modes, default for example bitmaps
    virtual void drawExampleBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t m = bm_default)
    {
      drawBitmap(bitmap, x, y, w, h, color, m);
    };
    // monochrome or gray levels to full screen, filled with white if size is less, no update needed
    virtual void drawPicture(const uint8_t *picture, uint32_t size, uint8_t depth = 1) // b/w or grey is class specific
    {
      drawBitmap(picture, size); // default is monochrome
    };
    // monochrome or gray levels to screen window, filled with white if size is less, no update needed
    virtual void drawPicture(const uint8_t *picture, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h){};
    // monochrome to full screen, filled with white if size is less, no update needed
    virtual void drawBitmap(const uint8_t *bitmap, uint32_t size, int16_t m = bm_normal) = 0; // monochrome
    virtual void drawExampleBitmap(const uint8_t *bitmap, uint32_t size, int16_t m = bm_default) // monochrome
    {
      drawBitmap(bitmap, size, m);
    };
    virtual void eraseDisplay(bool using_partial_update = false) {};
    // partial update of rectangle from buffer to screen, does not power off
    virtual void updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation = true) {};
  protected:
    void drawBitmapBM(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t m);
    static inline uint16_t gx_uint16_min(uint16_t a, uint16_t b) {return (a < b ? a : b);};
    static inline uint16_t gx_uint16_max(uint16_t a, uint16_t b) {return (a > b ? a : b);};
};

#endif


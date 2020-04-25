// GxEPD_HD_Spiffs_Example : example for HD e-Paper displays from Dalian Good Display Inc. (parallel interface).
//
// Display Library based on Demo Example available from Good Display
//
// BMP handling code extracts taken from: https://github.com/prenticedavid/MCUFRIEND_kbv/tree/master/examples/showBMP_kbv_Uno
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD_HD
//
// note that BMP bitmaps are drawn at physical position in physical orientation of the screen

#if defined(ESP32)
#include "SPIFFS.h"
#endif

#include <FS.h>
#define FileClass fs::File
#define EPD_CS SS

// include library, include base class, make path known
#include <GxEPD_HD_EPD.h>
#include <GxEPD_HD_BW.h>

// select the display io class to use, only one
// for my DESP32T (proto board) for TCon-11 parallel interface
//#include <GxDESP32T/GxDESP32T.h>
//GxDESP32T io;
// for my DESP32T_BP (proto board) for TCon-11 parallel interface
#include <GxDESP32T_BP/GxDESP32T_BP.h>
GxDESP32T_BP io;

// select the base display class to use, only one
//GxGDE043A2 base_display(io); // default vcom used (-2.0V)
//GxGDE060BA base_display(io); // default vcom used (-2.0V)
GxGDE060BA base_display(io, -2.3); // vcom from sticker on flex connector of my panel, as double
//GxGDE060BA base_display(io, 2300); // or as abs(vcom*1000) in mV, as uint16_t 
//GxGDEW080T5 base_display(io); // default vcom used (-2.2V)

// select the graphics display template class to use, only one
//GxEPD_HD_BW<GxGDE043A2, GxGDE043A2::HEIGHT> display(base_display); // full height, one page
GxEPD_HD_BW<GxGDE060BA, GxGDE060BA::HEIGHT> display(base_display); // full height, one page

// function declaration with default parameter
// note that BMP bitmaps are drawn at physical position in physical orientation of the screen
void drawBitmapFromSpiffs(const char *filename, int16_t x, int16_t y, bool clear = true, bool no_refresh = false);
void drawBitmapFromSpiffs_16G(const char *filename, int16_t x, int16_t y, bool clear = true, bool no_refresh = false);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("GxEPD_HD_Spiffs_Example");

  //display.init(&Serial);
  display.init();

  SPIFFS.begin();
  Serial.println("SPIFFS started");
  listFiles();
  spiffs_sizes();

  //drawBitmaps_200x200();
  //drawBitmaps_other();
  //drawBitmaps_test();

  drawBitmaps_200x200_16G();
  drawBitmaps_other_16G();
  drawBitmaps_test_16G();

  //display.powerOff();
  display.hibernate();

  Serial.println("GxEPD_HD_Spiffs_Example done");
}

void loop(void)
{
}

void spiffs_sizes()
{
  size_t total = SPIFFS.totalBytes();
  size_t used = SPIFFS.usedBytes();
  Serial.println();
  Serial.print("SPIFFS: used: "); Serial.print(used); Serial.print(" free: "); Serial.print(total - used);  Serial.print(" total: "); Serial.println(total);
}

void drawBitmaps_200x200()
{
  int16_t x = (display.width() - 200) / 2;
  int16_t y = (display.height() - 200) / 2;
  drawBitmapFromSpiffs("logo200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("first200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("second200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("third200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("fourth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("fifth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("sixth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("seventh200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("eighth200x200.bmp", x, y);
  delay(2000);
}

void drawBitmaps_other()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  drawBitmapFromSpiffs("chanceflurries.bmp", w2 - 50, h2 - 50);
  delay(2000);
  drawBitmapFromSpiffs("betty_1.bmp", w2 - 100, h2 - 160);
  delay(2000);
  drawBitmapFromSpiffs("betty_4.bmp", w2 - 102, h2 - 126);
  delay(2000);
  drawBitmapFromSpiffs("marilyn_240x240x8.bmp", w2 - 120, h2 - 120);
  delay(2000);
  drawBitmapFromSpiffs("miniwoof.bmp", w2 - 60, h2 - 80);
  delay(2000);
  drawBitmapFromSpiffs("tiger.bmp", w2 - 160, h2 - 120);
  delay(2000);
  drawBitmapFromSpiffs("tiger_178x160x4.bmp", w2 - 89, h2 - 80);
  delay(2000);
  drawBitmapFromSpiffs("tiger_240x317x4.bmp", w2 - 120, h2 - 160);
  delay(2000);
  drawBitmapFromSpiffs("tiger_320x200x24.bmp", w2 - 160, h2 - 100);
  delay(2000);
  drawBitmapFromSpiffs("woof.bmp", w2 - 120, h2 - 160);
  delay(2000);
}

void drawBitmaps_test()
{
  drawBitmapFromSpiffs("output5.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("output6.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("tractor_1.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("tractor_4.bmp", 0, 0);
  delay(2000);
  //drawBitmapFromSpiffs("tractor_8.bmp", 0, 0); // depth 32 not supportet
  //delay(2000);
  drawBitmapFromSpiffs("tractor_11.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("tractor_44.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("tractor_88.bmp", 0, 0);
  delay(2000);
}

void drawBitmaps_200x200_16G()
{
  int16_t x = (display.width() - 200) / 2;
  int16_t y = (display.height() - 200) / 2;
  drawBitmapFromSpiffs_16G("logo200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_16G("first200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_16G("second200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_16G("third200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_16G("fourth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_16G("fifth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_16G("sixth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_16G("seventh200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_16G("eighth200x200.bmp", x, y);
  delay(2000);
}

void drawBitmaps_other_16G()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  drawBitmapFromSpiffs_16G("chanceflurries.bmp", w2 - 50, h2 - 50);
  delay(2000);
  drawBitmapFromSpiffs_16G("betty_1.bmp", w2 - 100, h2 - 160);
  delay(2000);
  drawBitmapFromSpiffs_16G("betty_4.bmp", w2 - 102, h2 - 126);
  delay(2000);
  drawBitmapFromSpiffs_16G("marilyn_240x240x8.bmp", w2 - 120, h2 - 120);
  delay(2000);
  drawBitmapFromSpiffs_16G("miniwoof.bmp", w2 - 60, h2 - 80);
  delay(2000);
  drawBitmapFromSpiffs_16G("tiger.bmp", w2 - 160, h2 - 120);
  delay(2000);
  drawBitmapFromSpiffs_16G("tiger_178x160x4.bmp", w2 - 89, h2 - 80);
  delay(2000);
  drawBitmapFromSpiffs_16G("tiger_240x317x4.bmp", w2 - 120, h2 - 160);
  delay(2000);
  drawBitmapFromSpiffs_16G("tiger_320x200x24.bmp", w2 - 160, h2 - 100);
  delay(2000);
  drawBitmapFromSpiffs_16G("woof.bmp", w2 - 120, h2 - 160);
  delay(2000);
}

void drawBitmaps_test_16G()
{
  drawBitmapFromSpiffs_16G("output5.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs_16G("output6.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs_16G("tractor_1.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs_16G("tractor_4.bmp", 0, 0);
  delay(2000);
  //drawBitmapFromSpiffs_16G("tractor_8.bmp", 0, 0); // depth 32 not supportet
  //delay(2000);
  drawBitmapFromSpiffs_16G("tractor_11.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs_16G("tractor_44.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs_16G("tractor_88.bmp", 0, 0);
  delay(2000);
}

static const uint16_t input_buffer_pixels = 1024; // may affect performance

static const uint16_t max_row_width = 1024; // for up to 8" display 1024x768
static const uint16_t max_palette_pixels = 256; // for depth <= 8

uint8_t input_buffer[3 * input_buffer_pixels]; // up to depth 24
uint8_t output_row_mono_buffer[max_row_width / 8]; // buffer for at least one row of b/w bits
uint8_t mono_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 b/w
uint8_t output_row_grey_buffer[max_row_width / 2]; // buffer for at least one row of 4 grey bits
uint8_t grey_palette_buffer[max_palette_pixels]; // palette buffer for depth <= 8, 8 grey bits

// note that BMP bitmaps are drawn at physical position in physical orientation of the screen
void drawBitmapFromSpiffs(const char *filename, int16_t x, int16_t y, bool clear, bool no_refresh)
{
  fs::File file;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.epd_hd.WIDTH) || (y >= display.epd_hd.HEIGHT)) return;
  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');
#if defined(ESP32)
  file = SPIFFS.open(String("/") + filename, "r");
#else
  file = SPIFFS.open(filename, "r");
#endif
  if (file && file.size())
  {
    // Parse BMP header
    if (read16(file) == 0x4D42) // BMP signature
    {
      uint32_t fileSize = read32(file);
      uint32_t creatorBytes = read32(file);
      uint32_t imageOffset = read32(file); // Start of image data
      uint32_t headerSize = read32(file);
      uint32_t width  = read32(file);
      uint32_t height = read32(file);
      uint16_t planes = read16(file);
      uint16_t depth = read16(file); // bits per pixel
      uint32_t format = read32(file);
      (void) creatorBytes; // not used
      if ((planes == 1) && ((format == 0) || (format == 3))) // uncompressed is handled, 565 also
      {
        Serial.print("File size: "); Serial.println(fileSize);
        Serial.print("Image Offset: "); Serial.println(imageOffset);
        Serial.print("Header size: "); Serial.println(headerSize);
        Serial.print("Bit Depth: "); Serial.println(depth);
        Serial.print("Image size: ");
        Serial.print(width);
        Serial.print('x');
        Serial.println(height);
        // BMP rows are padded (if needed) to 4-byte boundary
        uint32_t rowSize = (width * depth / 8 + 3) & ~3;
        if (depth < 8) rowSize = ((width * depth + 8 - depth) / 8 + 3) & ~3;
        if (height < 0)
        {
          height = -height;
          flip = false;
        }
        uint16_t w = width;
        uint16_t h = height;
        if ((x + w - 1) >= display.epd_hd.WIDTH)  w = display.epd_hd.WIDTH  - x;
        if ((y + h - 1) >= display.epd_hd.HEIGHT) h = display.epd_hd.HEIGHT - y;
        if (w <= max_row_width) // handle with direct drawing
        {
          valid = true;
          uint8_t bitmask = 0xFF;
          uint8_t bitshift = 8 - depth;
          uint16_t red, green, blue;
          bool whitish;
          if (depth <= 8)
          {
            if (depth < 8) bitmask >>= depth;
            //file.seek(54); //palette is always @ 54
            file.seek(imageOffset - (4 << depth)); // 54 for regular, diff for colorsimportant
            for (uint16_t pn = 0; pn < (1 << depth); pn++)
            {
              blue  = file.read();
              green = file.read();
              red   = file.read();
              file.read();
              whitish = ((red + green + blue) > 3 * 0x80); // whitish
              if (0 == pn % 8) mono_palette_buffer[pn / 8] = 0;
              mono_palette_buffer[pn / 8] |= whitish << pn % 8;
            }
          }
          if (clear) display.clearScreen();
          uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
          for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
          {
            uint32_t in_remain = rowSize;
            uint32_t in_idx = 0;
            uint32_t in_bytes = 0;
            uint8_t in_byte = 0; // for depth <= 8
            uint8_t in_bits = 0; // for depth <= 8
            uint8_t out_byte = 0xFF; // white (for w%8!=0 border)
            uint32_t out_idx = 0;
            file.seek(rowPosition);
            for (uint16_t col = 0; col < w; col++) // for each pixel
            {
              // Time to read more pixel data?
              if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
              {
                in_bytes = file.read(input_buffer, in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain);
                in_remain -= in_bytes;
                in_idx = 0;
              }
              switch (depth)
              {
                case 24:
                  blue = input_buffer[in_idx++];
                  green = input_buffer[in_idx++];
                  red = input_buffer[in_idx++];
                  whitish = ((red + green + blue) > 3 * 0x80); // whitish
                  break;
                case 16:
                  {
                    uint8_t lsb = input_buffer[in_idx++];
                    uint8_t msb = input_buffer[in_idx++];
                    if (format == 0) // 555
                    {
                      blue  = (lsb & 0x1F) << 3;
                      green = ((msb & 0x03) << 6) | ((lsb & 0xE0) >> 2);
                      red   = (msb & 0x7C) << 1;
                    }
                    else // 565
                    {
                      blue  = (lsb & 0x1F) << 3;
                      green = ((msb & 0x07) << 5) | ((lsb & 0xE0) >> 3);
                      red   = (msb & 0xF8);
                    }
                    whitish = ((red + green + blue) > 3 * 0x80); // whitish
                  }
                  break;
                case 1:
                case 4:
                case 8:
                  {
                    if (0 == in_bits)
                    {
                      in_byte = input_buffer[in_idx++];
                      in_bits = 8;
                    }
                    uint16_t pn = (in_byte >> bitshift) & bitmask;
                    whitish = mono_palette_buffer[pn / 8] & (0x1 << pn % 8);
                    in_byte <<= depth;
                    in_bits -= depth;
                  }
                  break;
                default:
                  valid = false;
              }
              if (!valid) break;
              if (whitish)
              {
                // keep white
              }
              else
              {
                out_byte &= ~(0x80 >> col % 8); // black
              }
              if ((7 == col % 8) || (col == w - 1)) // write that last byte! (for w%8!=0 border)
              {
                output_row_mono_buffer[out_idx++] = out_byte;
                out_byte = 0xFF; // white (for w%8!=0 border)
              }
            } // end pixel
            if (!valid) break;
            uint16_t yrow = y + (flip ? h - row - 1 : row);
            display.writeImage(output_row_mono_buffer, w / 8, 1, x, yrow, w, 1);
          } // end line
          Serial.print("loaded in "); Serial.print(millis() - startTime); Serial.println(" ms");
          if (!no_refresh) display.refresh();
        } // max_row_width
      } // format
    } // BMP
    file.close();
    if (!valid)
    {
      Serial.println("bitmap format not handled.");
    }
  } // if (file)
  else Serial.println("File not found");
}

// note that BMP bitmaps are drawn at physical position in physical orientation of the screen
void drawBitmapFromSpiffs_16G(const char *filename, int16_t x, int16_t y, bool clear, bool no_refresh)
{
  fs::File file;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.epd_hd.WIDTH) || (y >= display.epd_hd.HEIGHT)) return;
  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');
#if defined(ESP32)
  file = SPIFFS.open(String("/") + filename, "r");
#else
  file = SPIFFS.open(filename, "r");
#endif
  if (file && file.size())
  {
    // Parse BMP header
    if (read16(file) == 0x4D42) // BMP signature
    {
      uint32_t fileSize = read32(file);
      uint32_t creatorBytes = read32(file);
      uint32_t imageOffset = read32(file); // Start of image data
      uint32_t headerSize = read32(file);
      uint32_t width  = read32(file);
      uint32_t height = read32(file);
      uint16_t planes = read16(file);
      uint16_t depth = read16(file); // bits per pixel
      uint32_t format = read32(file);
      (void) creatorBytes; // not used
      if ((planes == 1) && ((format == 0) || (format == 3))) // uncompressed is handled, 565 also
      {
        Serial.print("File size: "); Serial.println(fileSize);
        Serial.print("Image Offset: "); Serial.println(imageOffset);
        Serial.print("Header size: "); Serial.println(headerSize);
        Serial.print("Bit Depth: "); Serial.println(depth);
        Serial.print("Image size: ");
        Serial.print(width);
        Serial.print('x');
        Serial.println(height);
        // BMP rows are padded (if needed) to 4-byte boundary
        uint32_t rowSize = (width * depth / 8 + 3) & ~3;
        if (depth < 8) rowSize = ((width * depth + 8 - depth) / 8 + 3) & ~3;
        if (height < 0)
        {
          height = -height;
          flip = false;
        }
        uint16_t w = width;
        uint16_t h = height;
        if ((x + w - 1) >= display.epd_hd.WIDTH)  w = display.epd_hd.WIDTH  - x;
        if ((y + h - 1) >= display.epd_hd.HEIGHT) h = display.epd_hd.HEIGHT - y;
        if (w <= max_row_width) // handle with direct drawing
        {
          valid = true;
          uint8_t bitmask = 0xFF;
          uint8_t bitshift = 8 - depth;
          uint16_t red, green, blue;
          uint8_t grey;
          if (depth <= 8)
          {
            if (depth < 8) bitmask >>= depth;
            //file.seek(54); //palette is always @ 54
            file.seek(imageOffset - (4 << depth)); // 54 for regular, diff for colorsimportant
            for (uint16_t pn = 0; pn < (1 << depth); pn++)
            {
              blue  = file.read();
              green = file.read();
              red   = file.read();
              file.read();
              grey = uint8_t((red + green + blue) / 3);
              grey_palette_buffer[pn] = grey;
            }
          }
          if (clear) display.clearScreen();
          uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
          for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
          {
            uint32_t in_remain = rowSize;
            uint32_t in_idx = 0;
            uint32_t in_bytes = 0;
            uint8_t in_byte = 0; // for depth <= 8
            uint8_t in_bits = 0; // for depth <= 8
            uint8_t out_byte = 0xFF; // white (for w%8!=0 border)
            uint32_t out_idx = 0;
            file.seek(rowPosition);
            for (uint16_t col = 0; col < w; col++) // for each pixel
            {
              // Time to read more pixel data?
              if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
              {
                in_bytes = file.read(input_buffer, in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain);
                in_remain -= in_bytes;
                in_idx = 0;
              }
              switch (depth)
              {
                case 24:
                  blue = input_buffer[in_idx++];
                  green = input_buffer[in_idx++];
                  red = input_buffer[in_idx++];
                  grey = uint8_t((red + green + blue) / 3);
                  break;
                case 16:
                  {
                    uint8_t lsb = input_buffer[in_idx++];
                    uint8_t msb = input_buffer[in_idx++];
                    if (format == 0) // 555
                    {
                      blue  = (lsb & 0x1F) << 3;
                      green = ((msb & 0x03) << 6) | ((lsb & 0xE0) >> 2);
                      red   = (msb & 0x7C) << 1;
                    }
                    else // 565
                    {
                      blue  = (lsb & 0x1F) << 3;
                      green = ((msb & 0x07) << 5) | ((lsb & 0xE0) >> 3);
                      red   = (msb & 0xF8);
                    }
                    grey = uint8_t((red + green + blue) / 3);
                  }
                  break;
                case 1:
                case 4:
                case 8:
                  {
                    if (0 == in_bits)
                    {
                      in_byte = input_buffer[in_idx++];
                      in_bits = 8;
                    }
                    uint16_t pn = (in_byte >> bitshift) & bitmask;
                    grey = grey_palette_buffer[pn];
                    in_byte <<= depth;
                    in_bits -= depth;
                  }
                  break;
                default:
                  valid = false;
              }
              if (!valid) break;
              if (0 == col % 2) // even, low nibble
              {
                // MSB first
                out_byte = 0x0F | (grey & 0xF0);
                output_row_grey_buffer[out_idx] = out_byte;
              }
              else
              {
                out_byte &= 0xF0;
                out_byte |= (grey >> 4);
                output_row_grey_buffer[out_idx++] = out_byte;
              }
            } // end pixel
            if (!valid) break;
            uint16_t yrow = y + (flip ? h - row - 1 : row);
            display.writeImage(output_row_grey_buffer, w / 2, 4, x, yrow, w, 1);
          } // end line
          Serial.print("loaded in "); Serial.print(millis() - startTime); Serial.println(" ms");
          if (!no_refresh) display.refresh();
        } // max_row_width
      } // format
    } // BMP
    file.close();
    if (!valid)
    {
      Serial.println("bitmap format not handled.");
    }
  } // if (file)
  else Serial.println("File not found");
}

uint16_t read16(fs::File& f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(fs::File& f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

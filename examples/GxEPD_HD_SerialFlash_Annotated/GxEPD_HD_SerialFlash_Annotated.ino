// GxEPD_HD_SerialFlash_Annotated : example for HD e-Paper displays from Dalian Good Display Inc. (parallel interface).
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
// To be used with Board: "Generic STM32F1 series" Board part number: "Generic F103VE" of package "STM32 Boards (select from submenu)" for Arduino
// Add this package by adding "https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json"
// to Preferences "Additional Boards Manager URLs" and installing with Boards Manager.
//
// Can also be used with "STM32F103VE" of "Generic STM32F103V series" of package "STM32 Boards (STM32Duino.com)" for Arduino.
// download this package as .zip file from https://github.com/rogerclarkmelbourne/Arduino_STM32
// and install it as described in https://github.com/rogerclarkmelbourne/Arduino_STM32/wiki/Installation
//
// Can also be used with ESP32 boards, e.g. "DOIT ESP32 DEVKIT V1", with my proto board for DESTM32-Tcon-11.
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html
//
// Can also be used with ESP32 or other large RAM board with Waveshare IT8951 board and matched e-paper panel
//
// this example uses the SerialFlash library from: https://github.com/PaulStoffregen/SerialFlash
// with a modification for use with the STM32 package available here: https://github.com/ZinggJM/SerialFlash
// download it as .zip file and install with Library Mananger method "Add .ZIP Library..."
//
// note that BMP bitmaps are drawn at physical position in physical orientation of the screen

// include library, include base class, make path known
#include <GxEPD_HD_EPD.h>
#include <GxEPD_HD_BW.h>

#include <SerialFlash.h>
#include <SPI.h>

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

// digital pin for flash chip CS pin:
#if (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) || (defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_GENERIC_F103VE))
const int FlashChipSelect = SS; // for onboard W25Q16BVSIG
//const int FlashChipSelect = PC4; // e.g. for breakout flash board added
#else
//const int FlashChipSelect = 32; // for W25Q16BVSIG on my DESP32T_BP proto board
const int FlashChipSelect = 2; // as used with my Wemos LOLIN32 Lite proto board SD connector
#endif

// select the display io class to use, only one
#if (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) || (defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_GENERIC_F103VE))
#include <GxDESTM32T/GxDESTM32T.h>
GxDESTM32T io;
#else
// next is for my DESP32T (proto board) for TCon-11 parallel interface
//#include <GxDESP32T/GxDESP32T.h>
//GxDESP32T io;
// next is for my DESP32T_BP (proto board) for TCon-11 parallel interface
//#include <GxDESP32T_BP/GxDESP32T_BP.h>
//GxDESP32T_BP io;
// next is for ED060SCT, ED060KC1, ED078KC2, ES103TC1 on matching IT8951 Driver HAT e.g. with ESP32
#include <GxIT8951/GxIT8951.h>
GxIT8951 io(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
#endif

// select the base display class to use, only one
//GxGDE043A2 base_display(io); // default vcom used (-2.0V)
//GxGDE060BA base_display(io); // default vcom used (-2.0V)
//GxGDE060BA base_display(io, -2.3); // vcom from sticker on flex connector of my panel, as double
//GxGDE060BA base_display(io, uint16_t(2300)); // or as abs(vcom*1000) in mV, as uint16_t
//GxGDE060F3 base_display(io, -2.4); // vcom from sticker on flex connector of my panel, as double
//GxGDEW080T5 base_display(io); // default vcom used (-2.2V)
// ED060SCT, ED060KC1, ED078KC2, ES103TC1 on matching IT8951 Driver HAT e.g. with ESP32
//GxED060SCT base_display(io); // default vcom used (-2.0V)
//GxED060KC1 base_display(io); // default vcom used (-2.0V)
GxED078KC2 base_display(io); // default vcom used (-2.0V)
//GxES103TC1 base_display(io); // default vcom used (-2.0V)

// select the graphics display template class to use, only one
#if (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) || (defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_GENERIC_F103VE))
//GxEPD_HD_BW < GxGDE043A2, GxGDE043A2::HEIGHT / 2 > display(base_display); // half height, 2 pages, ~30k RAM remaining
GxEPD_HD_BW < GxGDE060BA, GxGDE060BA::HEIGHT / 2 > display(base_display); // half height, 2 pages, ~30k RAM remaining
//GxEPD_HD_BW < GxGDE060F3, GxGDE060F3::HEIGHT / 2 > display(base_display); // half height, 2 pages, ~11k RAM remaining
//GxEPD_HD_BW < GxGDEW080T5, GxGDEW080T5::HEIGHT / 2 > display(base_display); // half height, 2 pages, ~11k RAM remaining
#else
//GxEPD_HD_BW<GxGDE043A2, GxGDE043A2::HEIGHT> display(base_display); // full height, one page
//GxEPD_HD_BW<GxGDE060BA, GxGDE060BA::HEIGHT> display(base_display); // full height, one page
//GxEPD_HD_BW<GxGDE060F3, GxGDE060F3::HEIGHT> display(base_display); // full height, one page
//GxEPD_HD_BW<GxGDEW080T5, GxGDEW080T5::HEIGHT> display(base_display); // full height, one page
// ED060SCT on matching IT8951 Driver HAT e.g. with ESP32
//GxEPD_HD_BW<GxED060SCT, GxED060SCT::HEIGHT> display(base_display); // full height, one page, on ESP32
//GxEPD_HD_BW < GxED060SCT, GxED060SCT::HEIGHT / 2 > display(base_display); // half height, 2 pages, e.g. on ESP8266
//GxEPD_HD_BW < GxED060SCT, GxED060SCT::HEIGHT / 4 > display(base_display); // quarter height, 4 pages, e.g. on MKR1000
// ED060KC1 on matching IT8951 Driver HAT e.g. with ESP32
//GxEPD_HD_BW < GxED060KC1, GxED060KC1::HEIGHT / 2 > display(base_display); // half height, 2 pages, on ESP32
//GxEPD_HD_BW < GxED060KC1, GxED060KC1::HEIGHT / 8 > display(base_display); // 1/8 height, 8 pages, e.g. on ESP8266
// ED060KC2, ES103TC1 on matching IT8951 Driver HAT e.g. with ESP32
GxEPD_HD_BW < GxED078KC2, GxED078KC2::HEIGHT / 8 > display(base_display); // 1/8, 8 pages, on ESP32
//GxEPD_HD_BW < GxES103TC1, GxES103TC1::HEIGHT / 8 > display(base_display); // 1/8, 8 pages, on ESP32
#endif

#if (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) || (defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_GENERIC_F103VE))
// select diagnostic output stream, only one
//HardwareSerial& DiagnosticStream = Serial1; // pins PA9, PA10
HardwareSerial& DiagnosticStream = Serial2; // pins PA2, PA3 for USB jumpers
//HardwareSerial& DiagnosticStream = Serial3; // pins PB10, PB11
//USBSerial& DiagnosticStream = Serial; // pins PA11, PA12 USB direct?
#else
HardwareSerial& DiagnosticStream = Serial; // ESP32
#endif

// function declaration with default parameter
// note that BMP bitmaps are drawn at physical position in physical orientation of the screen
void drawBitmapFromSerialFlash(const char *filename, int16_t x, int16_t y, bool clear = true, bool no_refresh = false);
void drawBitmapFromSerialFlash_16G(const char *filename, int16_t x, int16_t y, bool clear = true, bool no_refresh = false);

void setup()
{
  DiagnosticStream.begin(115200);
  DiagnosticStream.println();
  DiagnosticStream.println("GxEPD_HD_SerialFlash_Example");

  display.init();

  //listFiles();

  showPage1();
  delay(2000);
  showPage2();
  delay(2000);
  showPage1();
  delay(2000);
  showPage2();

  display.powerOff();
  display.hibernate();

  DiagnosticStream.println("GxEPD_HD_SerialFlash_Example done");
}

void loop(void)
{
}

void showPage1()
{
  // BMP bitmaps are drawn at physical position in physical orientation of the screen
  int16_t x1 = 40;
  int16_t y1 = 40;
  int16_t x2 = display.epd_hd.WIDTH / 2;
  int16_t y2 = display.epd_hd.HEIGHT / 3;
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setRotation(0);
    placeText(x1, y1 + 320 + 20, "betty_1.bmp");
    placeText(x2, y2 + 252 + 20, "betty_4.bmp");
  }
  while (display.nextPage(true)); // norefresh
  drawBitmapFromSerialFlash_16G("betty_1.bmp", x1, y1, false, true);
  drawBitmapFromSerialFlash_16G("betty_4.bmp", x2, y2, false, true);
  display.refresh();
}

void showPage2()
{
  // BMP bitmaps are drawn at physical position in physical orientation of the screen
  int16_t x1 = display.epd_hd.WIDTH / 2 - 40;
  int16_t y1 = (display.epd_hd.HEIGHT - 240) / 2;
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setRotation(0);
    display.setCursor(x1, y1 + 240 + 20);
    display.println("tiger.bmp");
    display.setRotation(3);
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println();
    display.println("Groop");
    display.println("I implore thee,");
    display.setTextSize(1);
    display.println("my foonting turlingdromes.");
    display.println("And hooptiously drangle me");
    display.println("with crinkly bindlewurdles,");
    display.println("Or I will rend thee");
    display.println("in the gobberwarts");
    display.println("with my blurglecruncheon,");
    display.println("see if I don't!");
    //display.setCursor(y1, x1 + 320 + 20);
    //display.println();
    placeCenteredText(x1 + 320 + 40, "the quick brown fox");
    placeCenteredText(x1 + 320 + 60, "jumps over the lazy dog");
  }
  while (display.nextPage(true)); // norefresh
  drawBitmapFromSerialFlash_16G("tiger.bmp", x1, y1, false, true);
  display.refresh();
}

void placeText(int16_t x, int16_t y, const char* text)
{
  display.setCursor(x, y);
  display.print(text);
}

void placeCenteredText(int16_t y, const char* text)
{
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  //uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setCursor(x, y);
  display.print(text);
}

void listFiles()
{
  if (SerialFlash.begin(FlashChipSelect))
  {
    DiagnosticStream.println("All Files on SPI Flash chip:");
    SerialFlash.opendir();
    while (1)
    {
      char filename[64];
      uint32_t filesize;

      if (SerialFlash.readdir(filename, sizeof(filename), filesize))
      {
        DiagnosticStream.print("  ");
        DiagnosticStream.print(filename);
        spaces(20 - strlen(filename));
        DiagnosticStream.print("  ");
        DiagnosticStream.print(filesize);
        DiagnosticStream.print(" bytes");
        DiagnosticStream.println();
      }
      else
      {
        DiagnosticStream.println("no more files...");
        break; // no more files
      }
    }
  }
  else
  {
    DiagnosticStream.println("Unable to access SPI Flash chip");
  }
#if defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)
  SPI.end(); // release SPI pins
  pinMode(FlashChipSelect, INPUT); // CS also!
#endif
}

void spaces(int num)
{
  for (int i = 0; i < num; i++)
  {
    DiagnosticStream.print(" ");
  }
}

static const uint16_t input_buffer_pixels = 1872; // may affect performance

static const uint16_t max_row_width = 1872; // for up to 7.8" or 10.3" display 1872x1404
static const uint16_t max_palette_pixels = 256; // for depth <= 8

uint8_t input_buffer[3 * input_buffer_pixels]; // up to depth 24
uint8_t output_row_mono_buffer[max_row_width / 8]; // buffer for at least one row of b/w bits
uint8_t mono_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 b/w
uint8_t output_row_grey_buffer[max_row_width / 2]; // buffer for at least one row of 4 grey bits
uint8_t grey_palette_buffer[max_palette_pixels]; // palette buffer for depth <= 8, 8 grey bits

// note that BMP bitmaps are drawn at physical position in physical orientation of the screen
void drawBitmapFromSerialFlash(const char *filename, int16_t x, int16_t y, bool clear, bool no_refresh)
{
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.epd_hd.WIDTH) || (y >= display.epd_hd.HEIGHT)) return;
  DiagnosticStream.println();
  DiagnosticStream.print("Loading image '");
  DiagnosticStream.print(filename);
  DiagnosticStream.println('\'');
  if (SerialFlash.begin(FlashChipSelect))
  {
    SerialFlashFile file = SerialFlash.open(filename);
    if (file)
    {
      // Parse BMP header
      uint16_t signature = read16(file);
      if (signature == 0x4D42) // BMP signature
      {
        uint32_t fileSize = read32(file);
        uint32_t creatorBytes = read32(file); (void)creatorBytes; //unused
        uint32_t imageOffset = read32(file); // Start of image data
        uint32_t headerSize = read32(file);
        uint32_t width  = read32(file);
        int32_t height = (int32_t) read32(file);
        uint16_t planes = read16(file);
        uint16_t depth = read16(file); // bits per pixel
        uint32_t format = read32(file);
        if ((planes == 1) && ((format == 0) || (format == 3))) // uncompressed is handled, 565 also
        {
          DiagnosticStream.print("File size: "); DiagnosticStream.println(fileSize);
          DiagnosticStream.print("Image Offset: "); DiagnosticStream.println(imageOffset);
          DiagnosticStream.print("Header size: "); DiagnosticStream.println(headerSize);
          DiagnosticStream.print("Bit Depth: "); DiagnosticStream.println(depth);
          DiagnosticStream.print("Image size: ");
          DiagnosticStream.print(width);
          DiagnosticStream.print('x');
          DiagnosticStream.println(height);
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
            bool whitish = false;
            if (depth <= 8)
            {
              if (depth < 8) bitmask >>= depth;
              //file.seek(54); //palette is always @ 54
              file.seek(imageOffset - (4 << depth)); // 54 for regular, diff for colorsimportant
              for (uint16_t pn = 0; pn < (1 << depth); pn++)
              {
                blue  = read8(file);
                green = read8(file);
                red   = read8(file);
                read8(file);
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
                  case 32:
                    blue = input_buffer[in_idx++];
                    green = input_buffer[in_idx++];
                    red = input_buffer[in_idx++];
                    in_idx++; // skip alpha
                    whitish = ((red + green + blue) > 3 * 0x80); // whitish
                    break;
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
                  case 2:
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
            DiagnosticStream.print("loaded in "); DiagnosticStream.print(millis() - startTime); DiagnosticStream.println(" ms");
            if (!no_refresh) display.refresh();
          } // max_row_width
        } // format
        else
        {
          DiagnosticStream.print("bad format: "); DiagnosticStream.println(format);
          DiagnosticStream.print("planes:     "); DiagnosticStream.println(planes);
        }
      } // BMP
      else
      {
        DiagnosticStream.println("bad signature: 0x"); DiagnosticStream.println(signature, HEX);
      }
      file.close();
      if (!valid)
      {
        DiagnosticStream.println("bitmap format not handled.");
      }
    } // if (file)
    else DiagnosticStream.println("File not found");
  }
  else DiagnosticStream.println("Unable to access SPI Flash chip");
#if defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)
  SPI.end(); // release SPI pins
  pinMode(FlashChipSelect, INPUT); // CS also!
#endif
}

// note that BMP bitmaps are drawn at physical position in physical orientation of the screen
void drawBitmapFromSerialFlash_16G(const char *filename, int16_t x, int16_t y, bool clear, bool no_refresh)
{
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.epd_hd.WIDTH) || (y >= display.epd_hd.HEIGHT)) return;
  DiagnosticStream.println();
  DiagnosticStream.print("Loading image '");
  DiagnosticStream.print(filename);
  DiagnosticStream.println('\'');
  if (SerialFlash.begin(FlashChipSelect))
  {
    SerialFlashFile file = SerialFlash.open(filename);
    if (file)
    {
      // Parse BMP header
      uint16_t signature = read16(file);
      if (signature == 0x4D42) // BMP signature
      {
        uint32_t fileSize = read32(file);
        uint32_t creatorBytes = read32(file); (void)creatorBytes; //unused
        uint32_t imageOffset = read32(file); // Start of image data
        uint32_t headerSize = read32(file);
        uint32_t width  = read32(file);
        int32_t height = (int32_t) read32(file);
        uint16_t planes = read16(file);
        uint16_t depth = read16(file); // bits per pixel
        uint32_t format = read32(file);
        if ((planes == 1) && ((format == 0) || (format == 3))) // uncompressed is handled, 565 also
        {
          DiagnosticStream.print("File size: "); DiagnosticStream.println(fileSize);
          DiagnosticStream.print("Image Offset: "); DiagnosticStream.println(imageOffset);
          DiagnosticStream.print("Header size: "); DiagnosticStream.println(headerSize);
          DiagnosticStream.print("Bit Depth: "); DiagnosticStream.println(depth);
          DiagnosticStream.print("Image size: ");
          DiagnosticStream.print(width);
          DiagnosticStream.print('x');
          DiagnosticStream.println(height);
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
                blue  = read8(file);
                green = read8(file);
                red   = read8(file);
                read8(file);
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
                  case 32:
                    blue = input_buffer[in_idx++];
                    green = input_buffer[in_idx++];
                    red = input_buffer[in_idx++];
                    in_idx++; // skip alpha
                    grey = uint8_t((red + green + blue) / 3);
                    break;
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
                  case 2:
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
            DiagnosticStream.print("loaded in "); DiagnosticStream.print(millis() - startTime); DiagnosticStream.println(" ms");
            if (!no_refresh) display.refresh();
          } // max_row_width
        } // format
        else
        {
          DiagnosticStream.print("bad format: "); DiagnosticStream.println(format);
          DiagnosticStream.print("planes:     "); DiagnosticStream.println(planes);
        }
      } // BMP
      else
      {
        DiagnosticStream.println("bad signature: 0x"); DiagnosticStream.println(signature, HEX);
      }
      file.close();
      if (!valid)
      {
        DiagnosticStream.println("bitmap format not handled.");
      }
    } // if (file)
    else DiagnosticStream.println("File not found");
  }
  else DiagnosticStream.println("Unable to access SPI Flash chip");
#if defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)
  SPI.end(); // release SPI pins
  pinMode(FlashChipSelect, INPUT); // CS also!
#endif
}

uint8_t read8(SerialFlashFile& f)
{
  uint8_t result;
  f.read((void*)&result, 1);
  return result;
}

uint16_t read16(SerialFlashFile& f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint16_t result;
  f.read((void*)&result, 2);
  return result;
}

uint32_t read32(SerialFlashFile& f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint32_t result;
  f.read((void*)&result, 4);
  return result;
}

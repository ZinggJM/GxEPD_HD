// GxEPD_HD_WiFi_Example : example for HD e-Paper displays from Dalian Good Display Inc. (parallel interface).
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

// include library, include base class, make path known
#include <GxEPD_HD_EPD.h>
#include <GxEPD_HD_BW.h>

// select the display io class to use, only one
// for my DESP32T (proto board) for TCon-11 parallel interface
//#include <GxDESP32T/GxDESP32T.h>
//GxDESP32T io;
// for my DESP32T_BP (proto board) for TCon-11 parallel interface
//#include <GxDESP32T_BP/GxDESP32T_BP.h>
//GxDESP32T_BP io;
// for ED060SCT, ED060KC1, ED078KC2, ES103TC1 on matching IT8951 Driver HAT e.g. with ESP32
#include <GxIT8951/GxIT8951.h>
GxIT8951 io(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);

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
//GxEPD_HD_BW<GxGDE043A2, GxGDE043A2::HEIGHT> display(base_display); // full height, one page
//GxEPD_HD_BW<GxGDE060BA, GxGDE060BA::HEIGHT> display(base_display); // full height, one page
//GxEPD_HD_BW<GxGDE060F3, GxGDE060F3::HEIGHT> display(base_display); // full height, one page
//GxEPD_HD_BW<GxGDEW080T5, GxGDEW080T5::HEIGHT> display(base_display); // full height, one page
// ED060SCT on IT8951 Driver HAT e.g. with ESP32
//GxEPD_HD_BW<GxED060SCT, GxED060SCT::HEIGHT> display(base_display); // full height, one page, on ESP32
// ED060KC1 on matching IT8951 Driver HAT e.g. with ESP32
//GxEPD_HD_BW < GxED060KC1, GxED060KC1::HEIGHT / 2 > display(base_display); // half height, 2 pages, on ESP32
// ED060KC2, ES103TC1 on matching IT8951 Driver HAT e.g. with ESP32
GxEPD_HD_BW < GxED078KC2, GxED078KC2::HEIGHT / 8 > display(base_display); // 1/8, 8 pages, on ESP32
//GxEPD_HD_BW < GxES103TC1, GxES103TC1::HEIGHT / 8 > display(base_display); // 1/8, 8 pages, on ESP32

#if defined (ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <WiFiClient.h>
#include <WiFiClientSecure.h>

const char* ssid     = "........";
const char* password = "........";
const int httpPort  = 80;
const int httpsPort = 443;
// note: the certificates have been moved to a separate header file, as R"CERT( destroys IDE Auto Format capability

#include "GxEPD_HD_github_raw_certs.h"

const char* certificate_rawcontent = cert_DigiCert_TLS_RSA_SHA256_2020_CA1; // ok, should work until 2031-04-13 23:59:59
//const char* certificate_rawcontent = github_io_chain_pem_first;  // ok, should work until Tue, 21 Mar 2023 23:59:59 GMT
//const char* certificate_rawcontent = github_io_chain_pem_second;  // ok, should work until Tue, 21 Mar 2023 23:59:59 GMT
//const char* certificate_rawcontent = github_io_chain_pem_third;  // ok, should work until Tue, 21 Mar 2023 23:59:59 GMT

const char* host_rawcontent   = "raw.githubusercontent.com";
const char* path_rawcontent   = "/ZinggJM/GxEPD2/master/extras/bitmaps/";
const char* path_prenticedavid   = "/prenticedavid/MCUFRIEND_kbv/master/extras/bitmaps/";
const char* fp_rawcontent     = "8F 0E 79 24 71 C5 A7 D2 A7 46 76 30 C1 3C B7 2A 13 B0 01 B2"; // as of 29.7.2022

void showBitmapFrom_HTTPS(const char* host, const char* path, const char* filename, const char* fingerprint, int16_t x, int16_t y, const char* certificate = certificate_rawcontent);
void showBitmapFrom_HTTPS_16G(const char* host, const char* path, const char* filename, const char* fingerprint, int16_t x, int16_t y, const char* certificate = certificate_rawcontent);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("GxEPD_HD_WiFi_Example");

  //display.init(&Serial);
  display.init();

#ifdef RE_INIT_NEEDED
  WiFi.persistent(true);
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.disconnect();
#endif

  if (!WiFi.getAutoConnect() || ( WiFi.getMode() != WIFI_STA) || ((WiFi.SSID() != ssid) && String(ssid) != "........"))
  {
    Serial.println();
    Serial.print("WiFi.getAutoConnect()=");
    Serial.println(WiFi.getAutoConnect());
    Serial.print("WiFi.SSID()=");
    Serial.println(WiFi.SSID());
    WiFi.mode(WIFI_STA); // switch off AP
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
  }
  int ConnectTimeout = 30; // 15 seconds
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    Serial.print(WiFi.status());
    if (--ConnectTimeout <= 0)
    {
      Serial.println();
      Serial.println("WiFi connect timeout");
      return;
    }
  }
  Serial.println();
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.println(WiFi.localIP());

  setClock();

  drawBitmaps_200x200();
  drawBitmaps_other();

  drawBitmaps_test();

  drawBitmaps_200x200_16G();
  drawBitmaps_other_16G();

  drawBitmaps_test_16G();

  //display.powerOff();
  display.hibernate();

  Serial.println("GxEPD_HD_WiFi_Example done");
}

void loop(void)
{
}

void drawBitmaps_200x200()
{
  int16_t x = (display.width() - 200) / 2;
  int16_t y = (display.height() - 200) / 2;
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "logo200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "first200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "second200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "third200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "fourth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "fifth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "sixth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "seventh200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "eighth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
}

void drawBitmaps_other()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  showBitmapFrom_HTTP("www.squix.org", "/blog/wunderground/", "chanceflurries.bmp", w2 - 50, h2 - 50);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "betty_1.bmp", fp_rawcontent, w2 - 100, h2 - 160);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "betty_4.bmp", fp_rawcontent, w2 - 102, h2 - 126);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "marilyn_240x240x8.bmp", fp_rawcontent, w2 - 120, h2 - 120);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "miniwoof.bmp", fp_rawcontent, w2 - 60, h2 - 80);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "test.bmp", fp_rawcontent, w2 - 120, h2 - 160);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "tiger.bmp", fp_rawcontent, w2 - 160, h2 - 120);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "tiger_178x160x4.bmp", fp_rawcontent, w2 - 89, h2 - 80);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "tiger_240x317x4.bmp", fp_rawcontent, w2 - 120, h2 - 160);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "tiger_320x200x24.bmp", fp_rawcontent, w2 - 160, h2 - 100);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "tiger16T.bmp", fp_rawcontent, w2 - 160, h2 - 100);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "woof.bmp", fp_rawcontent, w2 - 100, h2 - 100);
  delay(2000);
}

void drawBitmaps_test()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "betty_4.bmp", fp_rawcontent, w2 - 102, h2 - 126);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "output5.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "output6.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_1.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_4.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  //showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_8.bmp", fp_rawcontent, 0, 0); // format 1: BI_RLE8 is not supported
  //delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_11.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_44.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_88.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  //showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "liberato640x384.bmp", fp_rawcontent, 0, 0);
  //delay(2000);
}

void drawBitmaps_200x200_16G()
{
  int16_t x = (display.width() - 200) / 2;
  int16_t y = (display.height() - 200) / 2;
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "logo200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "first200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "second200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "third200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "fourth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "fifth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "sixth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "seventh200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "eighth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
}

void drawBitmaps_other_16G()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  showBitmapFrom_HTTP_16G("www.squix.org", "/blog/wunderground/", "chanceflurries.bmp", w2 - 50, h2 - 50);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "betty_1.bmp", fp_rawcontent, w2 - 100, h2 - 160);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "betty_4.bmp", fp_rawcontent, w2 - 102, h2 - 126);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "marilyn_240x240x8.bmp", fp_rawcontent, w2 - 120, h2 - 120);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "miniwoof.bmp", fp_rawcontent, w2 - 60, h2 - 80);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "test.bmp", fp_rawcontent, w2 - 100, h2 - 100);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "tiger.bmp", fp_rawcontent, w2 - 160, h2 - 120);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "tiger_178x160x4.bmp", fp_rawcontent, w2 - 89, h2 - 80);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "tiger_240x317x4.bmp", fp_rawcontent, w2 - 120, h2 - 160);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "tiger_320x200x24.bmp", fp_rawcontent, w2 - 160, h2 - 100);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "tiger16T.bmp", fp_rawcontent, w2 - 160, h2 - 100);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "woof.bmp", fp_rawcontent, w2 - 100, h2 - 100);
  delay(2000);
}

void drawBitmaps_test_16G()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_prenticedavid, "betty_4.bmp", fp_rawcontent, w2 - 102, h2 - 126);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "output5.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "output6.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "tractor_1.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "tractor_4.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  //showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "tractor_8.bmp", fp_rawcontent, 0, 0); // depth 32 not supportet
  //delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "tractor_11.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "tractor_44.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "tractor_88.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  //showBitmapFrom_HTTPS_16G(host_rawcontent, path_rawcontent, "liberato640x384.bmp", fp_rawcontent, 0, 0);
  //delay(2000);
}

static const uint16_t input_buffer_pixels = 1872; // may affect performance

static const uint16_t max_row_width = 1872; // for up to 7.8" display 1872x1404
static const uint16_t max_palette_pixels = 256; // for depth <= 8

uint8_t input_buffer[3 * input_buffer_pixels]; // up to depth 24
uint8_t output_row_mono_buffer[max_row_width / 8]; // buffer for at least one row of b/w bits
uint8_t mono_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 b/w
uint8_t output_row_grey_buffer[max_row_width / 2]; // buffer for at least one row of 4 grey bits
uint8_t grey_palette_buffer[max_palette_pixels]; // palette buffer for depth <= 8, 8 grey bits

void showBitmapFrom_HTTP(const char* host, const char* path, const char* filename, int16_t x, int16_t y)
{
  WiFiClient client;
  bool connection_ok = false;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.epd_hd.WIDTH) || (y >= display.epd_hd.HEIGHT)) return;
  Serial.println(); Serial.print("downloading file \""); Serial.print(filename);  Serial.println("\"");
  Serial.print("connecting to "); Serial.println(host);
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(String("http://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: Gxepd_hd_WiFi_Example\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (!connection_ok)
    {
      connection_ok = line.startsWith("HTTP/1.1 200 OK");
      if (connection_ok) Serial.println(line);
      //if (!connection_ok) Serial.println(line);
    }
    if (!connection_ok) Serial.println(line);
    //Serial.println(line);
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!connection_ok) return;
  // Parse BMP header
  uint16_t signature = read16(client);
  if (signature == 0x4D42) // BMP signature
  {
    uint32_t fileSize = read32(client);
    uint32_t creatorBytes = read32(client); (void)creatorBytes; //unused
    uint32_t imageOffset = read32(client); // Start of image data
    uint32_t headerSize = read32(client);
    uint32_t width  = read32(client);
    int32_t height = (int32_t) read32(client);
    uint16_t planes = read16(client);
    uint16_t depth = read16(client); // bits per pixel
    uint32_t format = read32(client);
    uint32_t bytes_read = 7 * 4 + 3 * 2; // read so far
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
        bool whitish = false;
        if (depth <= 8)
        {
          if (depth < 8) bitmask >>= depth;
          //bytes_read += skip(client, 54 - bytes_read); //palette is always @ 54
          bytes_read += skip(client, imageOffset - (4 << depth) - bytes_read); // 54 for regular, diff for colorsimportant
          for (uint16_t pn = 0; pn < (1 << depth); pn++)
          {
            blue  = client.read();
            green = client.read();
            red   = client.read();
            client.read();
            bytes_read += 4;
            whitish = ((red + green + blue) > 3 * 0x80); // whitish
            if (0 == pn % 8) mono_palette_buffer[pn / 8] = 0;
            mono_palette_buffer[pn / 8] |= whitish << pn % 8;
          }
        }
        display.clearScreen();
        uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
        //Serial.print("skip "); Serial.println(rowPosition - bytes_read);
        bytes_read += skip(client, rowPosition - bytes_read);
        for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
        {
          if (!connection_ok || !(client.connected() || client.available())) break;
          delay(1); // yield() to avoid WDT
          uint32_t in_remain = rowSize;
          uint32_t in_idx = 0;
          uint32_t in_bytes = 0;
          uint8_t in_byte = 0; // for depth <= 8
          uint8_t in_bits = 0; // for depth <= 8
          uint8_t out_byte = 0xFF; // white (for w%8!=0 border)
          uint32_t out_idx = 0;
          for (uint16_t col = 0; col < w; col++) // for each pixel
          {
            yield();
            if (!connection_ok || !(client.connected() || client.available())) break;
            // Time to read more pixel data?
            if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
            {
              uint32_t get = in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain;
              uint32_t got = readBytes(client, input_buffer, get);
              while ((got < get) && connection_ok)
              {
                //Serial.print("got "); Serial.print(got); Serial.print(" < "); Serial.print(get); Serial.print(" @ "); Serial.println(bytes_read);
                uint32_t gotmore = readBytes(client, input_buffer + got, get - got);
                got += gotmore;
                connection_ok = gotmore > 0;
              }
              in_bytes = got;
              in_remain -= got;
              bytes_read += got;
            }
            if (!connection_ok)
            {
              Serial.print("Error: got no more after "); Serial.print(bytes_read); Serial.println(" bytes read!");
              break;
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
            }
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
          int16_t yrow = y + (flip ? h - row - 1 : row);
          display.writeImage(output_row_mono_buffer, w / 8, 1, x, yrow, w, 1);
        } // end line
        Serial.print("downloaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
        display.refresh();
      }
      Serial.print("bytes read "); Serial.println(bytes_read);
    }
    else
    {
      Serial.print("bad format: "); Serial.println(format);
      Serial.print("planes:     "); Serial.println(planes);
    }
  }
  else
  {
    Serial.println("bad signature: 0x"); Serial.println(signature, HEX);
  }
  client.stop();
  if (!valid)
  {
    Serial.println("bitmap format not handled.");
  }
}

void showBitmapFrom_HTTP_16G(const char* host, const char* path, const char* filename, int16_t x, int16_t y)
{
  WiFiClient client;
  bool connection_ok = false;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.epd_hd.WIDTH) || (y >= display.epd_hd.HEIGHT)) return;
  Serial.println(); Serial.print("downloading file \""); Serial.print(filename);  Serial.println("\"");
  Serial.print("connecting to "); Serial.println(host);
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(String("http://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: Gxepd_hd_WiFi_Example\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (!connection_ok)
    {
      connection_ok = line.startsWith("HTTP/1.1 200 OK");
      if (connection_ok) Serial.println(line);
      //if (!connection_ok) Serial.println(line);
    }
    if (!connection_ok) Serial.println(line);
    //Serial.println(line);
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!connection_ok) return;
  // Parse BMP header
  uint16_t signature = read16(client);
  if (signature == 0x4D42) // BMP signature
  {
    uint32_t fileSize = read32(client);
    uint32_t creatorBytes = read32(client); (void)creatorBytes; //unused
    uint32_t imageOffset = read32(client); // Start of image data
    uint32_t headerSize = read32(client);
    uint32_t width  = read32(client);
    int32_t height = (int32_t) read32(client);
    uint16_t planes = read16(client);
    uint16_t depth = read16(client); // bits per pixel
    uint32_t format = read32(client);
    uint32_t bytes_read = 7 * 4 + 3 * 2; // read so far
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
        uint8_t grey = 0;
        if (depth <= 8)
        {
          if (depth < 8) bitmask >>= depth;
          //bytes_read += skip(client, 54 - bytes_read); //palette is always @ 54
          bytes_read += skip(client, imageOffset - (4 << depth) - bytes_read); // 54 for regular, diff for colorsimportant
          for (uint16_t pn = 0; pn < (1 << depth); pn++)
          {
            blue  = client.read();
            green = client.read();
            red   = client.read();
            client.read();
            bytes_read += 4;
            grey = uint8_t((red + green + blue) / 3);
            grey_palette_buffer[pn] = grey;
          }
        }
        display.clearScreen();
        uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
        //Serial.print("skip "); Serial.println(rowPosition - bytes_read);
        bytes_read += skip(client, rowPosition - bytes_read);
        for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
        {
          if (!connection_ok || !(client.connected() || client.available())) break;
          delay(1); // yield() to avoid WDT
          uint32_t in_remain = rowSize;
          uint32_t in_idx = 0;
          uint32_t in_bytes = 0;
          uint8_t in_byte = 0; // for depth <= 8
          uint8_t in_bits = 0; // for depth <= 8
          uint8_t out_byte = 0xFF; // white (for w%8!=0 border)
          uint32_t out_idx = 0;
          for (uint16_t col = 0; col < w; col++) // for each pixel
          {
            yield();
            if (!connection_ok || !(client.connected() || client.available())) break;
            // Time to read more pixel data?
            if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
            {
              uint32_t get = in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain;
              uint32_t got = readBytes(client, input_buffer, get);
              while ((got < get) && connection_ok)
              {
                //Serial.print("got "); Serial.print(got); Serial.print(" < "); Serial.print(get); Serial.print(" @ "); Serial.println(bytes_read);
                uint32_t gotmore = readBytes(client, input_buffer + got, get - got);
                got += gotmore;
                connection_ok = gotmore > 0;
              }
              in_bytes = got;
              in_remain -= got;
              bytes_read += got;
            }
            if (!connection_ok)
            {
              Serial.print("Error: got no more after "); Serial.print(bytes_read); Serial.println(" bytes read!");
              break;
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
            }
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
          int16_t yrow = y + (flip ? h - row - 1 : row);
          display.writeImage(output_row_grey_buffer, w / 2, 4, x, yrow, w, 1);
        } // end line
        Serial.print("downloaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
        display.refresh();
      }
      Serial.print("bytes read "); Serial.println(bytes_read);
    }
    else
    {
      Serial.print("bad format: "); Serial.println(format);
      Serial.print("planes:     "); Serial.println(planes);
    }
  }
  else
  {
    Serial.println("bad signature: 0x"); Serial.println(signature, HEX);
  }
  client.stop();
  if (!valid)
  {
    Serial.println("bitmap format not handled.");
  }
}

void showBitmapFrom_HTTPS(const char* host, const char* path, const char* filename, const char* fingerprint, int16_t x, int16_t y, const char* certificate)
{
  // Use WiFiClientSecure class to create TLS connection
#if defined (ESP8266)
  BearSSL::WiFiClientSecure client;
  BearSSL::X509List cert(certificate ? certificate : certificate_rawcontent);
#else
  WiFiClientSecure client;
#endif
  bool connection_ok = false;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.epd_hd.WIDTH) || (y >= display.epd_hd.HEIGHT)) return;
  Serial.println(); Serial.print("downloading file \""); Serial.print(filename);  Serial.println("\"");
  Serial.print("connecting to "); Serial.println(host);
#if defined (ESP8266)
  client.setBufferSizes(4096, 4096); // required
  //client.setBufferSizes(8192, 4096); // may help for some sites
  if (certificate) client.setTrustAnchors(&cert);
  else if (fingerprint) client.setFingerprint(fingerprint);
  else client.setInsecure();
#elif defined (ESP32)
  if (certificate) client.setCACert(certificate);
#endif
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(String("https://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: Gxepd_hd_WiFi_Example\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (!connection_ok)
    {
      connection_ok = line.startsWith("HTTP/1.1 200 OK");
      if (connection_ok) Serial.println(line);
      //if (!connection_ok) Serial.println(line);
    }
    if (!connection_ok) Serial.println(line);
    //Serial.println(line);
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!connection_ok) return;
  // Parse BMP header
  uint16_t signature = 0;
  for (int16_t i = 0; i < 50; i++)
  {
    if (!client.available()) delay(100);
    else signature = read16(client);
    //Serial.print("signature: 0x"); Serial.println(signature, HEX);
    if (signature == 0x4D42) break;
  }
  if (signature == 0x4D42) // BMP signature
  {
    uint32_t fileSize = read32(client);
    uint32_t creatorBytes = read32(client); (void)creatorBytes; //unused
    uint32_t imageOffset = read32(client); // Start of image data
    uint32_t headerSize = read32(client);
    uint32_t width  = read32(client);
    int32_t height = (int32_t) read32(client);
    uint16_t planes = read16(client);
    uint16_t depth = read16(client); // bits per pixel
    uint32_t format = read32(client);
    uint32_t bytes_read = 7 * 4 + 3 * 2; // read so far
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
        bool whitish = false;
        if (depth <= 8)
        {
          if (depth < 8) bitmask >>= depth;
          //bytes_read += skip(client, 54 - bytes_read); //palette is always @ 54
          bytes_read += skip(client, imageOffset - (4 << depth) - bytes_read); // 54 for regular, diff for colorsimportant
          for (uint16_t pn = 0; pn < (1 << depth); pn++)
          {
            blue  = client.read();
            green = client.read();
            red   = client.read();
            client.read();
            bytes_read += 4;
            whitish = ((red + green + blue) > 3 * 0x80); // whitish
            if (0 == pn % 8) mono_palette_buffer[pn / 8] = 0;
            mono_palette_buffer[pn / 8] |= whitish << pn % 8;
          }
        }
        display.clearScreen();
        uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
        //Serial.print("skip "); Serial.println(rowPosition - bytes_read);
        bytes_read += skip(client, rowPosition - bytes_read);
        for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
        {
          if (!connection_ok || !(client.connected() || client.available())) break;
          delay(1); // yield() to avoid WDT
          uint32_t in_remain = rowSize;
          uint32_t in_idx = 0;
          uint32_t in_bytes = 0;
          uint8_t in_byte = 0; // for depth <= 8
          uint8_t in_bits = 0; // for depth <= 8
          uint8_t out_byte = 0xFF; // white (for w%8!=0 border)
          uint32_t out_idx = 0;
          for (uint16_t col = 0; col < w; col++) // for each pixel
          {
            yield();
            if (!connection_ok || !(client.connected() || client.available())) break;
            // Time to read more pixel data?
            if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
            {
              uint32_t get = in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain;
              uint32_t got = readBytes(client, input_buffer, get);
              while ((got < get) && connection_ok)
              {
                //Serial.print("got "); Serial.print(got); Serial.print(" < "); Serial.print(get); Serial.print(" @ "); Serial.println(bytes_read);
                //if ((get - got) > client.available()) delay(200); // does improve? yes, if >= 200
                uint32_t gotmore = readBytes(client, input_buffer + got, get - got);
                got += gotmore;
                connection_ok = gotmore > 0;
              }
              in_bytes = got;
              in_remain -= got;
              bytes_read += got;
            }
            if (!connection_ok)
            {
              Serial.print("Error: got no more after "); Serial.print(bytes_read); Serial.println(" bytes read!");
              break;
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
            }
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
          int16_t yrow = y + (flip ? h - row - 1 : row);
          display.writeImage(output_row_mono_buffer, w / 8, 1, x, yrow, w, 1);
        } // end line
        Serial.print("downloaded in "); Serial.print(millis() - startTime); Serial.println(" ms");
        Serial.print("bytes read "); Serial.println(bytes_read);
        display.refresh();
      }
    }
    else
    {
      Serial.print("bad format: "); Serial.println(format);
      Serial.print("planes:     "); Serial.println(planes);
    }
  }
  else
  {
    Serial.println("bad signature: 0x"); Serial.println(signature, HEX);
  }
  client.stop();
  if (!valid)
  {
    Serial.println("bitmap format not handled.");
  }
}

void showBitmapFrom_HTTPS_16G(const char* host, const char* path, const char* filename, const char* fingerprint, int16_t x, int16_t y, const char* certificate)
{
  // Use WiFiClientSecure class to create TLS connection
#if defined (ESP8266)
  BearSSL::WiFiClientSecure client;
  BearSSL::X509List cert(certificate ? certificate : certificate_rawcontent);
#else
  WiFiClientSecure client;
#endif
  bool connection_ok = false;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.epd_hd.WIDTH) || (y >= display.epd_hd.HEIGHT)) return;
  Serial.println(); Serial.print("downloading file \""); Serial.print(filename);  Serial.println("\"");
  Serial.print("connecting to "); Serial.println(host);
#if defined (ESP8266)
  client.setBufferSizes(4096, 4096); // required
  //client.setBufferSizes(8192, 4096); // may help for some sites
  if (certificate) client.setTrustAnchors(&cert);
  else if (fingerprint) client.setFingerprint(fingerprint);
  else client.setInsecure();
#elif defined (ESP32)
  if (certificate) client.setCACert(certificate);
#endif
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(String("https://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: Gxepd_hd_WiFi_Example\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (!connection_ok)
    {
      connection_ok = line.startsWith("HTTP/1.1 200 OK");
      if (connection_ok) Serial.println(line);
      //if (!connection_ok) Serial.println(line);
    }
    if (!connection_ok) Serial.println(line);
    //Serial.println(line);
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!connection_ok) return;
  // Parse BMP header
  uint16_t signature = 0;
  for (int16_t i = 0; i < 50; i++)
  {
    if (!client.available()) delay(100);
    else signature = read16(client);
    //Serial.print("signature: 0x"); Serial.println(signature, HEX);
    if (signature == 0x4D42) break;
  }
  if (signature == 0x4D42) // BMP signature
  {
    uint32_t fileSize = read32(client);
    uint32_t creatorBytes = read32(client); (void)creatorBytes; //unused
    uint32_t imageOffset = read32(client); // Start of image data
    uint32_t headerSize = read32(client);
    uint32_t width  = read32(client);
    int32_t height = (int32_t) read32(client);
    uint16_t planes = read16(client);
    uint16_t depth = read16(client); // bits per pixel
    uint32_t format = read32(client);
    uint32_t bytes_read = 7 * 4 + 3 * 2; // read so far
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
        uint8_t grey = 0;
        if (depth <= 8)
        {
          if (depth < 8) bitmask >>= depth;
          //bytes_read += skip(client, 54 - bytes_read); //palette is always @ 54
          bytes_read += skip(client, imageOffset - (4 << depth) - bytes_read); // 54 for regular, diff for colorsimportant
          for (uint16_t pn = 0; pn < (1 << depth); pn++)
          {
            blue  = client.read();
            green = client.read();
            red   = client.read();
            client.read();
            bytes_read += 4;
            grey = uint8_t((red + green + blue) / 3);
            grey_palette_buffer[pn] = grey;
          }
        }
        display.clearScreen();
        uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
        //Serial.print("skip "); Serial.println(rowPosition - bytes_read);
        bytes_read += skip(client, rowPosition - bytes_read);
        for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
        {
          if (!connection_ok || !(client.connected() || client.available())) break;
          delay(1); // yield() to avoid WDT
          uint32_t in_remain = rowSize;
          uint32_t in_idx = 0;
          uint32_t in_bytes = 0;
          uint8_t in_byte = 0; // for depth <= 8
          uint8_t in_bits = 0; // for depth <= 8
          uint8_t out_byte = 0xFF; // white (for w%8!=0 border)
          uint32_t out_idx = 0;
          for (uint16_t col = 0; col < w; col++) // for each pixel
          {
            yield();
            if (!connection_ok || !(client.connected() || client.available())) break;
            // Time to read more pixel data?
            if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
            {
              uint32_t get = in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain;
              uint32_t got = readBytes(client, input_buffer, get);
              while ((got < get) && connection_ok)
              {
                //Serial.print("got "); Serial.print(got); Serial.print(" < "); Serial.print(get); Serial.print(" @ "); Serial.println(bytes_read);
                //if ((get - got) > client.available()) delay(200); // does improve? yes, if >= 200
                uint32_t gotmore = readBytes(client, input_buffer + got, get - got);
                got += gotmore;
                connection_ok = gotmore > 0;
              }
              in_bytes = got;
              in_remain -= got;
              bytes_read += got;
            }
            if (!connection_ok)
            {
              Serial.print("Error: got no more after "); Serial.print(bytes_read); Serial.println(" bytes read!");
              break;
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
            }
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
          int16_t yrow = y + (flip ? h - row - 1 : row);
          display.writeImage(output_row_grey_buffer, w / 2, 4, x, yrow, w, 1);
        } // end line
        Serial.print("downloaded in "); Serial.print(millis() - startTime); Serial.println(" ms");
        Serial.print("bytes read "); Serial.println(bytes_read);
        display.refresh();
      }
    }
    else
    {
      Serial.print("bad format: "); Serial.println(format);
      Serial.print("planes:     "); Serial.println(planes);
    }
  }
  else
  {
    Serial.println("bad signature: 0x"); Serial.println(signature, HEX);
  }
  client.stop();
  if (!valid)
  {
    Serial.println("bitmap format not handled.");
  }
}

uint16_t read16(WiFiClient& client)
{
  // BMP data is stored little-endian, same as Arduino.
  uint16_t result;
  ((uint8_t *)&result)[0] = client.read(); // LSB
  ((uint8_t *)&result)[1] = client.read(); // MSB
  return result;
}

uint32_t read32(WiFiClient& client)
{
  // BMP data is stored little-endian, same as Arduino.
  uint32_t result;
  ((uint8_t *)&result)[0] = client.read(); // LSB
  ((uint8_t *)&result)[1] = client.read();
  ((uint8_t *)&result)[2] = client.read();
  ((uint8_t *)&result)[3] = client.read(); // MSB
  return result;
}

#if defined (ESP8266)

uint32_t skip(BearSSL::WiFiClientSecure& client, int32_t bytes)
{
  int32_t remain = bytes;
  uint32_t start = millis();
  while ((client.connected() || client.available()) && (remain > 0))
  {
    if (client.available())
    {
      client.read();
      remain--;
    }
    else delay(1);
    if (millis() - start > 2000) break; // don't hang forever
  }
  return bytes - remain;
}

uint32_t readBytes(BearSSL::WiFiClientSecure& client, uint8_t* buffer, int32_t bytes)
{
  int32_t remain = bytes;
  uint32_t start = millis();
  while ((client.connected() || client.available()) && (remain > 0))
  {
    if (client.available())
    {
      int16_t v = client.read();
      *buffer++ = uint8_t(v);
      remain--;
    }
    else delay(1);
    if (millis() - start > 2000) break; // don't hang forever
  }
  return bytes - remain;
}

#endif

uint32_t skip(WiFiClient& client, int32_t bytes)
{
  int32_t remain = bytes;
  uint32_t start = millis();
  while ((client.connected() || client.available()) && (remain > 0))
  {
    if (client.available())
    {
      client.read();
      remain--;
    }
    else delay(1);
    if (millis() - start > 2000) break; // don't hang forever
  }
  return bytes - remain;
}

uint32_t readBytes(WiFiClient& client, uint8_t* buffer, int32_t bytes)
{
  int32_t remain = bytes;
  uint32_t start = millis();
  while ((client.connected() || client.available()) && (remain > 0))
  {
    if (client.available())
    {
      int16_t v = client.read();
      *buffer++ = uint8_t(v);
      remain--;
    }
    else delay(1);
    if (millis() - start > 2000) break; // don't hang forever
  }
  return bytes - remain;
}

// Set time via NTP, as required for x.509 validation
void setClock()
{
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

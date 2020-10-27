// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// GxIT8951 class is based on Demo Example from Waveshare for Raspberry PI https://github.com/waveshare/IT8951/archive/master.zip
// Controller: IT8951 : https://www.waveshare.com/w/upload/1/18/IT8951_D_V0.2.4.3_20170728.pdf
//
// The GxIT8951 driver class supports the Waveshare e-Paper IT8951 Driver HAT connected with SPI for the ED060SCT 6" e-paper panel (parallel IF)
// https://www.waveshare.com/product/mini-pc/raspberry-pi/hats/6inch-e-paper-hat.htm
// This Driver HAT requires 5V power supply but works with 3.3V data lines; requires both MOSI and MISO SPI lines.
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxIT8951.h"

#define SerialDiag if (_pDiagnosticOutput) (*_pDiagnosticOutput)

//Built in I80 Command Code
#define IT8951_TCON_SYS_RUN      0x0001
#define IT8951_TCON_STANDBY      0x0002
#define IT8951_TCON_SLEEP        0x0003
#define IT8951_TCON_REG_RD       0x0010
#define IT8951_TCON_REG_WR       0x0011
#define IT8951_TCON_LD_IMG       0x0020
#define IT8951_TCON_LD_IMG_AREA  0x0021
#define IT8951_TCON_LD_IMG_END   0x0022

//I80 User defined command code
#define USDEF_I80_CMD_DPY_AREA     0x0034
#define USDEF_I80_CMD_GET_DEV_INFO 0x0302
#define USDEF_I80_CMD_DPY_BUF_AREA 0x0037
#define USDEF_I80_CMD_VCOM       0x0039

//Rotate mode
#define IT8951_ROTATE_0     0
#define IT8951_ROTATE_90    1
#define IT8951_ROTATE_180   2
#define IT8951_ROTATE_270   3

//Pixel mode , BPP - Bit per Pixel
#define IT8951_2BPP   0
#define IT8951_3BPP   1
#define IT8951_4BPP   2
#define IT8951_8BPP   3

//Endian Type
#define IT8951_LDIMG_L_ENDIAN   0
#define IT8951_LDIMG_B_ENDIAN   1

#define SYS_REG_BASE 0x0000
#define I80CPCR (SYS_REG_BASE + 0x04)
#define MCSR_BASE_ADDR 0x0200
#define LISAR (MCSR_BASE_ADDR + 0x0008)

#if !defined(ESP8266) && !defined(ESP32)
#define yield() {}
#endif

GxIT8951::GxIT8951(int8_t cs, int8_t dc, int8_t rst, int8_t busy) : _cs(cs), _dc(dc), _rst(rst), _busy(busy),
  _spi_settings(24000000, MSBFIRST, SPI_MODE0),
  _spi_settings_for_read(1000000, MSBFIRST, SPI_MODE0)
{
  _initial_write = true;
  _initial_refresh = true;
  _power_is_on = false;
  _using_partial_mode = false;
  _hibernating = false;
  _vcom = 2000;
}

void GxIT8951::init(GxEPD_HD::Panel panel, uint16_t vcom_mV, Stream* pDiagnosticOutput)
{
  _panel = panel;
  if (vcom_mV <= 5000) _vcom = vcom_mV;
  else _vcom = 2000;
  _pDiagnosticOutput = pDiagnosticOutput;
  _width = 800; // default
  _height = 600; // default
  if (panel == GxEPD_HD::ED060SCT)
  {
    _width = 800;
    _height = 600;
  }
  _initial_write = true;
  _initial_refresh = true;
  _power_is_on = false;
  _using_partial_mode = false;
  _hibernating = false;
  //SerialDiag.println("GxIT8951::init()");
  if (_cs >= 0)
  {
    digitalWrite(_cs, HIGH);
    pinMode(_cs, OUTPUT);
  }
  if (_dc >= 0)
  {
    digitalWrite(_dc, HIGH);
    pinMode(_dc, OUTPUT);
  }
  if (_busy >= 0)
  {
    pinMode(_busy, INPUT);
  }
  SPI.begin();
  // we need a long reset pulse
  if (_rst >= 0)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, LOW);
    delay(200);
    digitalWrite(_rst, HIGH);
    delay(200);
    _waitWhileBusy("GxIT8951::init() reset_to_ready", reset_to_ready_time);
  }

  _writeCommand16(USDEF_I80_CMD_GET_DEV_INFO);
  _waitWhileBusy("GetIT8951SystemInfo", power_on_time);
  _readData16((uint16_t*)&IT8951DevInfo, sizeof(IT8951DevInfo) / 2);
  //Show Device information of IT8951
  SerialDiag.print("Panel(W,H) = ("); SerialDiag.print(IT8951DevInfo.usPanelW); SerialDiag.print("(, "); SerialDiag.println(IT8951DevInfo.usPanelH );
  SerialDiag.print("Image Buffer Address = 0x"); SerialDiag.println(uint32_t(IT8951DevInfo.usImgBufAddrL) | (uint32_t(IT8951DevInfo.usImgBufAddrH) << 16));
  //  //Show Firmware and LUT Version
  SerialDiag.print("FW Version = "); SerialDiag.println((char*)IT8951DevInfo.usFWVersion);
  SerialDiag.print("LUT Version = "); SerialDiag.println((char*)IT8951DevInfo.usLUTVersion);
  //Set to Enable I80 Packed mode
  _IT8951WriteReg(I80CPCR, 0x0001);
  if (_vcom != _IT8951GetVCOM())
  {
    _IT8951SetVCOM(_vcom);
    SerialDiag.print("set VCOM = -"); SerialDiag.println((float)_IT8951GetVCOM() / 1000);
  }
  //SerialDiag.print("VCOM = -"); SerialDiag.println((float)_IT8951GetVCOM() / 1000);
  //SerialDiag.println("GxIT8951::init() done");
}

void GxIT8951::clearScreen(uint8_t value)
{
  if (_initial_refresh) _Init_Full();
  else _Init_Part();
  _initial_refresh = false;
  _setPartialRamArea(0, 0, _width, _height, IT8951_8BPP);
  SPI.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  _transfer16(0x0000); // preamble for write data
  _waitWhileBusy("clearScreen preamble", default_wait_time);
  for (uint32_t i = 0; i < uint32_t(_width) * uint32_t(_height); i++)
  {
    SPI.transfer(value);
#if defined(ESP8266) || defined(ESP32)
    if (0 == i % 10000) yield();
#endif
  }
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
  _writeCommand16(IT8951_TCON_LD_IMG_END);
  _waitWhileBusy("clearScreen load end", default_wait_time);
  _refresh(0, 0, _width, _height, false);
}

void GxIT8951::writeScreenBuffer(uint8_t value)
{
  if (_initial_refresh) clearScreen(value);
  else _writeScreenBuffer(value);
}

void GxIT8951::_writeScreenBuffer(uint8_t value)
{
  if (!_using_partial_mode) _Init_Part();
  _setPartialRamArea(0, 0, _width, _height, IT8951_8BPP);
  SPI.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  _transfer16(0x0000); // preamble for write data
  _waitWhileBusy("clearScreen preamble", default_wait_time);
  for (uint32_t i = 0; i < uint32_t(_width) * uint32_t(_height); i++)
  {
    SPI.transfer(value);
    if (0 == i % 10000) yield();
  }
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
  _writeCommand16(IT8951_TCON_LD_IMG_END);
  _waitWhileBusy("_writeScreenBuffer load end", default_wait_time);
}

void GxIT8951::writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  //SerialDiag.print("writeImage("); SerialDiag.print(size); SerialDiag.print(", "); SerialDiag.print(depth);
  //SerialDiag.print(", "); SerialDiag.print(x); SerialDiag.print(", "); SerialDiag.print(y); SerialDiag.print(", ");
  //SerialDiag.print(w); SerialDiag.print(", "); SerialDiag.print(h); SerialDiag.println(")");
  if ((x >= _width) || (y >= _height)) return;
  uint16_t bm_wb = w; // copy original w here, will be _width bytes of bitmap
  if (x + w > _width) w = _width - x;
  if (y + h > _height) h = _height - y;
  if (!_using_partial_mode) _Init_Part();
  uint16_t wb, xd, wbd;
  switch (depth)
  {
    case 1:
      bm_wb = (bm_wb + 7) / 8;
      wb = (w + 7) / 8;
      _setPartialRamArea(x, y, 8 * wb, h, IT8951_8BPP);
      SPI.beginTransaction(_spi_settings);
      if (_cs >= 0) digitalWrite(_cs, LOW);
      _transfer16(0x0000); // preamble for write data
      for (int16_t i = 0; i < h; i++)
      {
        for (int16_t j = 0; j < wb; j++)
        {
          uint32_t idx = uint32_t(j) + uint32_t(i) * uint32_t(bm_wb);
          uint8_t data = idx < size ? bitmap[idx] : 0xFF;
          _send8pixel(~data);
        }
        yield();
      }
      if (_cs >= 0) digitalWrite(_cs, HIGH);
      SPI.endTransaction();
      break;
    case 2:
      bm_wb = (bm_wb + 3) / 4;
      wb = (w + 3) / 4;
      xd = x - x % 8; // 2 bpp mode – X Start position must be multiples of 8
      wbd = 2 * ((w + 7) / 8); // "must be multiples of 8"
      _setPartialRamArea(xd, y, 4 * wbd, h, IT8951_2BPP);
      SPI.beginTransaction(_spi_settings);
      if (_cs >= 0) digitalWrite(_cs, LOW);
      _transfer16(0x0000); // preamble for write data
      for (int16_t i = 0; i < h; i++)
      {
        for (int16_t j = 0; j < wb; j++)
        {
          uint32_t idx = uint32_t(j) + uint32_t(i) * uint32_t(bm_wb);
          uint8_t data = idx < size ? bitmap[idx] : 0xFF;
          SPI.transfer(data);
        }
        if (wbd > wb) SPI.transfer(0xFF);
      }
      if (_cs >= 0) digitalWrite(_cs, HIGH);
      SPI.endTransaction();
      break;
    case 4:
      bm_wb = (bm_wb + 1) / 2;
      wb = (w + 1) / 2;
      xd = x - x % 4; // 4 bpp mode – X Start position must be multiples of 4
      wbd = 2 * ((w + 3) / 4); // "must be multiples of 4"
      _setPartialRamArea(xd, y, 2 * wbd, h, IT8951_4BPP);
      SPI.beginTransaction(_spi_settings);
      if (_cs >= 0) digitalWrite(_cs, LOW);
      _transfer16(0x0000); // preamble for write data
      for (int16_t i = 0; i < h; i++)
      {
        for (int16_t j = 0; j < wb; j++)
        {
          uint32_t idx = uint32_t(j) + uint32_t(i) * uint32_t(bm_wb);
          uint8_t data = idx < size ? bitmap[idx] : 0xFF;
          SPI.transfer(data);
        }
        if (wbd > wb) SPI.transfer(0xFF);
      }
      if (_cs >= 0) digitalWrite(_cs, HIGH);
      SPI.endTransaction();
      break;
    case 8:
      xd = x - x % 2; // 8 bpp mode – X Start position must be multiples of 2
      wbd = 2 * ((w + 1) / 2); // "must be multiples of 2"
      _setPartialRamArea(xd, y, wbd, h, IT8951_8BPP);
      SPI.beginTransaction(_spi_settings);
      if (_cs >= 0) digitalWrite(_cs, LOW);
      _transfer16(0x0000); // preamble for write data
      for (int16_t i = 0; i < h; i++)
      {
        for (int16_t j = 0; j < w; j++)
        {
          uint32_t idx = uint32_t(j) + uint32_t(i) * uint32_t(bm_wb);
          uint8_t data = idx < size ? bitmap[idx] : 0xFF;
          SPI.transfer(data);
        }
        if (wbd > w) SPI.transfer(0xFF);
      }
      if (_cs >= 0) digitalWrite(_cs, HIGH);
      SPI.endTransaction();
      break;
  }
  _writeCommand16(IT8951_TCON_LD_IMG_END);
  yield(); // to avoid WDT on ESP8266 and ESP32
}

void GxIT8951::writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                              int16_t x, int16_t y, int16_t w, int16_t h)
{
  if ((x >= _width) || (y >= _height)) return;
  if (x + w > _width) w = _width - x;
  if (y + h > _height) h = _height - y;
  if (x_part + w > w_bitmap) w = w_bitmap - x_part;
  if (y_part + h > h_bitmap) h = h_bitmap - y_part;
  uint16_t bm_wb, wb, xd, wbd;
  if (!_using_partial_mode) _Init_Part();
  switch (depth)
  {
    case 1:
      bm_wb = (w_bitmap + 7) / 8;
      wb = (w + 7) / 8;
      _setPartialRamArea(x, y, 8 * wb, h, IT8951_8BPP);
      SPI.beginTransaction(_spi_settings);
      if (_cs >= 0) digitalWrite(_cs, LOW);
      _transfer16(0x0000); // preamble for write data
      for (int16_t i = 0; i < h; i++)
      {
        for (int16_t j = 0; j < wb; j++)
        {
          uint32_t idx = uint32_t(j + x_part / 8) + uint32_t(i + y_part) * uint32_t(bm_wb);
          uint8_t data = idx < size ? bitmap[idx] : 0xFF;
          _send8pixel(~data);
        }
        yield();
      }
      if (_cs >= 0) digitalWrite(_cs, HIGH);
      SPI.endTransaction();
      break;
    case 2:
      bm_wb = (w_bitmap + 3) / 4;
      wb = (w + 3) / 4;
      xd = x - x % 8; // 2 bpp mode – X Start position must be multiples of 8
      wbd = 2 * ((w + 7) / 8); // "must be multiples of 8"
      _setPartialRamArea(xd, y, 4 * wbd, h, IT8951_2BPP);
      SPI.beginTransaction(_spi_settings);
      if (_cs >= 0) digitalWrite(_cs, LOW);
      _transfer16(0x0000); // preamble for write data
      for (int16_t i = 0; i < h; i++)
      {
        for (int16_t j = 0; j < wb; j++)
        {
          uint32_t idx = uint32_t(j + x_part / 4) + uint32_t(i + y_part) * uint32_t(bm_wb);
          uint8_t data = idx < size ? bitmap[idx] : 0xFF;
          SPI.transfer(data);
        }
        if (wbd > wb) SPI.transfer(0xFF);
      }
      if (_cs >= 0) digitalWrite(_cs, HIGH);
      SPI.endTransaction();
      break;
    case 4:
      bm_wb = (w_bitmap + 1) / 2;
      wb = (w + 1) / 2;
      xd = x - x % 4; // 4 bpp mode – X Start position must be multiples of 4
      wbd = 2 * ((w + 3) / 4); // "must be multiples of 4"
      _setPartialRamArea(xd, y, 2 * wbd, h, IT8951_4BPP);
      SPI.beginTransaction(_spi_settings);
      if (_cs >= 0) digitalWrite(_cs, LOW);
      _transfer16(0x0000); // preamble for write data
      for (int16_t i = 0; i < h; i++)
      {
        for (int16_t j = 0; j < wb; j++)
        {
          uint32_t idx = uint32_t(j + x_part / 2) + uint32_t(i + y_part) * uint32_t(bm_wb);
          uint8_t data = idx < size ? bitmap[idx] : 0xFF;
          SPI.transfer(data);
        }
        if (wbd > wb) SPI.transfer(0xFF);
      }
      if (_cs >= 0) digitalWrite(_cs, HIGH);
      SPI.endTransaction();
      break;
    case 8:
      xd = x - x % 2; // 8 bpp mode – X Start position must be multiples of 2
      wbd = 2 * ((w + 1) / 2); // "must be multiples of 2"
      _setPartialRamArea(xd, y, wbd, h, IT8951_8BPP);
      SPI.beginTransaction(_spi_settings);
      if (_cs >= 0) digitalWrite(_cs, LOW);
      _transfer16(0x0000); // preamble for write data
      for (int16_t i = 0; i < h; i++)
      {
        for (int16_t j = 0; j < w; j++)
        {
          uint32_t idx = uint32_t(j + x_part) + uint32_t(i + y_part) * uint32_t(w_bitmap);
          uint8_t data = idx < size ? bitmap[idx] : 0xFF;
          SPI.transfer(data);
        }
        if (wbd > w) SPI.transfer(0xFF);
      }
      if (_cs >= 0) digitalWrite(_cs, HIGH);
      SPI.endTransaction();
      break;
  }
  _writeCommand16(IT8951_TCON_LD_IMG_END);
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxIT8951::drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  //SerialDiag.print("drawImage("); SerialDiag.print(size); SerialDiag.print(", "); SerialDiag.print(depth);
  //SerialDiag.print(", "); SerialDiag.print(x); SerialDiag.print(", "); SerialDiag.print(y); SerialDiag.print(", ");
  //SerialDiag.print(w); SerialDiag.print(", "); SerialDiag.print(h); SerialDiag.println(")");
  writeImage(bitmap, size, depth, x, y, w, h);
  _refresh(x, y, w, h, (depth == 1));
}

void GxIT8951::drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                             int16_t x, int16_t y, int16_t w, int16_t h)
{
  writeImagePart(bitmap, size, depth, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h);
  refresh(x, y, w, h, (depth == 1));
}

void GxIT8951::refresh(bool partial_update_mode)
{
  _refresh(0, 0, _width, _height, partial_update_mode);
}

void GxIT8951::refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode)
{
  _refresh(x, y, w, h, partial_update_mode);
}

void GxIT8951::_refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode)
{
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(_width) ? w : int16_t(_width) - x; // limit
  int16_t h1 = y + h < int16_t(_height) ? h : int16_t(_height) - y; // limit
  w1 -= x1 - x;
  h1 -= y1 - y;
  //Send I80 Display Command (User defined command of IT8951)
  _writeCommand16(USDEF_I80_CMD_DPY_AREA); //0x0034
  _waitWhileBusy(0, refresh_par_time); //_waitWhileBusy("refresh cmd", refresh_cmd_time); // needed
  //Write arguments
  _writeData16(x1);
  _waitWhileBusy(0, refresh_par_time); //_waitWhileBusy("refresh x", refresh_par_time); // needed
  _writeData16(y1);
  _waitWhileBusy(0, refresh_par_time); //_waitWhileBusy("refresh y", refresh_par_time); // needed
  _writeData16(w1);
  _waitWhileBusy(0, refresh_par_time); //_waitWhileBusy("refresh w", refresh_par_time); // needed
  _writeData16(h1);
  _waitWhileBusy(0, refresh_par_time); //_waitWhileBusy("refresh h", refresh_par_time); // needed
  _writeData16(partial_update_mode ? 1 : 2); // mode
  _waitWhileBusy("refresh", full_refresh_time);
}

void GxIT8951::powerOff(void)
{
  _PowerOff();
}

void GxIT8951::hibernate()
{
  if (_power_is_on) _PowerOff();
  if (_rst >= 0)
  {
    // this does not work, does not reduce power, uses more than in stand by mode
    //delay(1000);
    //_IT8951Sleep();
    //delay(1000);
    //_hibernating = true;
  }
}

void GxIT8951::_send8pixel(uint8_t data)
{
  for (uint8_t j = 0; j < 8; j++)
  {
    SPI.transfer(data & 0x80 ? 0x00 : 0xFF);
    data <<= 1;
  }
}

void GxIT8951::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t bpp)
{
  uint16_t usArg[5];
  usArg[0] = (IT8951_LDIMG_B_ENDIAN << 8 ) | (bpp << 4) | (IT8951_ROTATE_0);
  usArg[1] = x;
  usArg[2] = y;
  usArg[3] = w;
  usArg[4] = h;
  _writeCommand16(IT8951_TCON_LD_IMG_AREA);
  _writeData16(usArg, 5);
}

void GxIT8951::_PowerOn()
{
  if (!_power_is_on)
  {
    _IT8951SystemRun();
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxIT8951::_PowerOff()
{
  _IT8951StandBy();
  _waitWhileBusy("_PowerOff", power_off_time);
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxIT8951::_InitDisplay()
{
  // we need a long reset pulse
  if (_hibernating && (_rst >= 0))
  {
    digitalWrite(_rst, LOW);
    delay(200);
    digitalWrite(_rst, HIGH);
    delay(200);
  }
}

void GxIT8951::_Init_Full()
{
  _InitDisplay();
  _PowerOn();
  _using_partial_mode = false;
}

void GxIT8951::_Init_Part()
{
  _InitDisplay();
  _PowerOn();
  _using_partial_mode = true;
}

void GxIT8951::_waitWhileBusy(const char* comment, uint16_t busy_time)
{
  if (_busy >= 0)
  {
    delay(1); // add some margin to become active
    unsigned long start = micros();
    while (1)
    {
      if (digitalRead(_busy) != LOW) break;
      delay(1);
      if (micros() - start > 2000000)
      {
        SerialDiag.println("Busy Timeout!");
        break;
      }
    }
    if (comment)
    {
      if (_pDiagnosticOutput)
      {
        unsigned long elapsed = micros() - start;
        //if (elapsed > diag_min_time * 1000)
        {
          SerialDiag.print(comment);
          SerialDiag.print(" : ");
          SerialDiag.println(elapsed);
        }
      }
    }
    (void) start;
  }
  else delay(busy_time);
}

uint16_t GxIT8951::_transfer16(uint16_t value)
{
  uint16_t rv = SPI.transfer(value >> 8) << 8;
  return (rv | SPI.transfer(value));
}

void GxIT8951::_writeCommand16(uint16_t c)
{
  SPI.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  _transfer16(0x6000); // preamble for write command
  _transfer16(c);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void GxIT8951::_writeData16(uint16_t d)
{
  SPI.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  _transfer16(0x0000); // preamble for write data
  _transfer16(d);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void GxIT8951::_writeData16(const uint16_t* d, uint32_t n)
{
  SPI.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  _transfer16(0x0000); // preamble for write data
  for (uint32_t i = 0; i < n; i++)
  {
    _transfer16(*d++);
  }
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

uint16_t GxIT8951::_readData16()
{
  SPI.beginTransaction(_spi_settings_for_read);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  _transfer16(0x1000); // preamble for read data
  _transfer16(0); // dummy
  uint16_t rv = _transfer16(0);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
  return rv;
}

void GxIT8951::_readData16(uint16_t* d, uint32_t n)
{
  SPI.beginTransaction(_spi_settings_for_read);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  _transfer16(0x1000); // preamble for read data
  _transfer16(0); // dummy
  for (uint32_t i = 0; i < n; i++)
  {
    *d++ = _transfer16(0);
  }
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void GxIT8951::_IT8951SystemRun()
{
  _writeCommand16(IT8951_TCON_SYS_RUN);
}

void GxIT8951::_IT8951StandBy()
{
  _writeCommand16(IT8951_TCON_STANDBY);
}

void GxIT8951::_IT8951Sleep()
{
  _writeCommand16(IT8951_TCON_SLEEP);
}

uint16_t GxIT8951::_IT8951ReadReg(uint16_t usRegAddr)
{
  uint16_t usData;
  //Send Cmd and Register Address
  _writeCommand16(IT8951_TCON_REG_RD);
  _writeData16(usRegAddr);
  //Read data from Host Data bus
  usData = _readData16();
  return usData;
}

void GxIT8951::_IT8951WriteReg(uint16_t usRegAddr, uint16_t usValue)
{
  //Send Cmd , Register Address and Write Value
  _writeCommand16(IT8951_TCON_REG_WR);
  _writeData16(usRegAddr);
  _writeData16(usValue);
}

uint16_t GxIT8951::_IT8951GetVCOM(void)
{
  uint16_t vcom;
  _writeCommand16(USDEF_I80_CMD_VCOM);
  _waitWhileBusy(0, default_wait_time); //_waitWhileBusy("_IT8951GetVCOM", default_wait_time); // needed
  _writeData16(0);
  vcom = _readData16();
  return vcom;
}

void GxIT8951::_IT8951SetVCOM(uint16_t vcom)
{
  _writeCommand16(USDEF_I80_CMD_VCOM);
  _waitWhileBusy(0, default_wait_time); //_waitWhileBusy("_IT8951SetVCOM", default_wait_time); // needed
  _writeData16(1);
  _writeData16(vcom);
  _waitWhileBusy("_IT8951SetVCOM", set_vcom_time);
}

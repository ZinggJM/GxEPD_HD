// Display Library for parallel interface e-paper panels from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Examples from Good Display, available here: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Class GxFT5436 : FT5436 touch driver class for GDE060F3-T on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD_HD

#include "GxFT5436.h"

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_STM32F1)

#define DiagOut if(_pDiagnosticOutput) (*_pDiagnosticOutput)

#if (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) // "STM32 Boards (STM32Duino.com)"

GxFT5436::GxFT5436(int8_t rst) : I2C(1), _sda(PB7), _scl(PB6), _rst(rst)
{
  _prev_idx = 0;
  _act_idx = 1;
  _info[0].clear();
  _info[1].clear();
}

GxFT5436::GxFT5436(int8_t sda, int8_t scl, int8_t rst) : I2C(sda == PB11 ? 2 : 1), _sda(sda), _scl(scl), _rst(rst)
{
  _prev_idx = 0;
  _act_idx = 1;
  _info[0].clear();
  _info[1].clear();
}

#elif defined(ESP32)

GxFT5436::GxFT5436(int8_t rst) : I2C(0), _sda(SDA), _scl(SCL), _rst(rst)
{
  _prev_idx = 0;
  _act_idx = 1;
  _info[0].clear();
  _info[1].clear();
}

GxFT5436::GxFT5436(int8_t sda, int8_t scl, int8_t rst) : I2C(0), _sda(sda), _scl(scl), _rst(rst)
{
  _prev_idx = 0;
  _act_idx = 1;
  _info[0].clear();
  _info[1].clear();
}

#else

GxFT5436::GxFT5436(int8_t rst) : _sda(SDA), _scl(SCL), _rst(rst)
{
  _prev_idx = 0;
  _act_idx = 1;
  _info[0].clear();
  _info[1].clear();
}

GxFT5436::GxFT5436(int8_t sda, int8_t scl, int8_t rst) : _sda(sda), _scl(scl), _rst(rst)
{
  _prev_idx = 0;
  _act_idx = 1;
  _info[0].clear();
  _info[1].clear();
}

#endif

void GxFT5436::init(Stream* pDiagnosticOutput)
{
  _pDiagnosticOutput = pDiagnosticOutput;
  _info[0].clear();
  _info[1].clear();
  if (_rst >= 0)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
    delay(100);
  }
#if (defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_GENERIC_F103VE)) // "STM32 Boards (select from submenu)"
  //I2C.begin(uint8_t(_sda), uint8_t(_scl)); // doesn't work, reason unknown, should match void begin(uint8_t, uint8_t);
  I2C.begin((uint8_t)_sda, (uint8_t)_scl); // this works
#elif (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) // "STM32 Boards (STM32Duino.com)"
  I2C.begin();
#else
  I2C.begin(_sda, _scl);
#endif
  I2C.beginTransmission(FT5436_I2C_ADDR);
  uint8_t error = I2C.endTransmission();
  if (error != 0)
  {
    DiagOut.print("GxFT5436::init() - I2C failed for address 0x"); DiagOut.print(FT5436_I2C_ADDR, HEX);
  }
  I2C_Write(FT5436_I2C_ADDR, FT_REG_DEV_MODE, 0);
  I2C_Write(FT5436_I2C_ADDR, FT_REG_THGROUP, 22);
  I2C_Write(FT5436_I2C_ADDR, FT_REG_POINT_RATE, 14);
}

uint8_t GxFT5436::scanSingleTouch(uint16_t& x, uint16_t& y)
{
  scan();
  x = _info[_act_idx].x[0];
  y = _info[_act_idx].y[0];
  return _info[_act_idx].touch_count;
}

GxFT5436::TouchInfo GxFT5436::scanMultipleTouch()
{
  scan();
  return _info[_act_idx];
}

uint8_t GxFT5436::newSingleTouch(uint16_t& x, uint16_t& y)
{
  scan();
  if (_info[_act_idx] == _info[_prev_idx])
  {
    x = 0;
    y = 0;
    return 0;
  }
  x = _info[_act_idx].x[0];
  y = _info[_act_idx].y[0];
  return _info[_act_idx].touch_count;
}

GxFT5436::TouchInfo GxFT5436::newMultipleTouch()
{
  scan();
  if (_info[_act_idx] == _info[_prev_idx]) return TouchInfo();
  //check("prev", _info[_prev_idx]);
  //check("act ", _info[_act_idx]);
  return _info[_act_idx];
}

uint8_t GxFT5436::lastSingleTouch(uint16_t& x, uint16_t& y)
{
  x = _info[_act_idx].x[0];
  y = _info[_act_idx].y[0];
  return _info[_act_idx].touch_count;
}

GxFT5436::TouchInfo GxFT5436::lastMultipleTouch()
{
  return _info[_act_idx];
}

void GxFT5436::scan()
{
  uint32_t start = micros();
  I2C_Read(FT5436_I2C_ADDR, FT_REG_DEV_MODE, _registers, POINT_READ_BUF);
  uint32_t elapsed1 = micros() - start;
  uint8_t touch_count = _registers[FT_TD_STATUS] & FT_MAX_ID;
  if (touch_count > CFG_MAX_TOUCH_POINTS)
  {
    //DiagOut.print("scan() got invalid touch_count: "); DiagOut.print(touch_count); DiagOut.print(" 0x"); DiagOut.println(_registers[FT_TD_STATUS], HEX);
    I2C_Read(FT5436_I2C_ADDR, FT_REG_DEV_MODE, _registers, POINT_READ_BUF);
    touch_count = _registers[FT_TD_STATUS] & FT_MAX_ID;
    if (touch_count > CFG_MAX_TOUCH_POINTS) touch_count = 0;
  }
  //std::swap(_act_idx, _prev_idx);
  int16_t t = _act_idx; _act_idx = _prev_idx; _prev_idx = t;
  _info[_act_idx].touch_count = touch_count;
  for (uint8_t i = 0; i < CFG_MAX_TOUCH_POINTS; i++)
  {
    _info[_act_idx].x[i] = 0;
    _info[_act_idx].y[i] = 0;
  }
  if (touch_count > 0)
  {
    _info[_act_idx].x[0] = ((_registers[3] & 0x0f) << 8) | _registers[4];
    _info[_act_idx].y[0] = ((_registers[5] & 0x0f) << 8) | _registers[6];
  }
  if (touch_count > 1)
  {
    _info[_act_idx].x[1] = ((_registers[3 + 6] & 0x0f) << 8) | _registers[4 + 6];
    _info[_act_idx].y[1] = ((_registers[5 + 6] & 0x0f) << 8) | _registers[6 + 6];
  }
  if (touch_count > 2)
  {
    _info[_act_idx].x[2] = ((_registers[3 + 2 * 6] & 0x0f) << 8) | _registers[4 + 2 * 6];
    _info[_act_idx].y[2] = ((_registers[5 + 2 * 6] & 0x0f) << 8) | _registers[6 + 2 * 6];
  }
  if (touch_count > 3)
  {
    _info[_act_idx].x[3] = ((_registers[3 + 3 * 6] & 0x0f) << 8) | _registers[4 + 3 * 6];
    _info[_act_idx].y[3] = ((_registers[5 + 3 * 6] & 0x0f) << 8) | _registers[6 + 3 * 6];
  }
  if (touch_count > 4)
  {
    _info[_act_idx].x[4] = ((_registers[3 + 4 * 6] & 0x0f) << 8) | _registers[4 + 4 * 6];
    _info[_act_idx].y[4] = ((_registers[5 + 4 * 6] & 0x0f) << 8) | _registers[6 + 4 * 6];
  }
  uint32_t elapsed2 = micros() - start;
  if (touch_count > 0)
  {
    //DiagOut.print("scan() "); DiagOut.print(elapsed1); DiagOut.print(" "); DiagOut.println(elapsed2);
  }
  (void) elapsed1;
  (void) elapsed2;
}

void GxFT5436::check(const char text[], TouchInfo& touchinfo)
{
  for (uint8_t i = 0; i < CFG_MAX_TOUCH_POINTS; i++)
  {
    DiagOut.print(text); DiagOut.print(" ("); DiagOut.print(touchinfo.x[i]); DiagOut.print(", "); DiagOut.print(touchinfo.y[i]); DiagOut.print(") ");
  }
  DiagOut.println(touchinfo.touch_count);
}

void GxFT5436::I2C_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t data)
{
  //DiagOut.println("I2C_Write");
  I2C.beginTransmission(dev_addr);
  I2C.write(reg_addr);
  I2C.write(data);
  I2C.endTransmission();
  //DiagOut.println("I2C_Write done");
}

uint8_t GxFT5436::I2C_Read(uint8_t dev_addr, uint8_t reg_addr)
{
  uint8_t data = 0;
  I2C.beginTransmission(dev_addr);
  I2C.write(reg_addr);
  I2C.endTransmission();
  I2C.requestFrom(dev_addr, uint8_t(1));
  if (I2C.available())
  {
    data = I2C.read();
  }
  return data;
}

void GxFT5436::I2C_Read(uint8_t dev_addr, uint8_t reg_addr, uint8_t* data, uint8_t n)
{
  I2C.beginTransmission(dev_addr);
  I2C.write(reg_addr);
  I2C.endTransmission();
  I2C.requestFrom(dev_addr, n);
  uint8_t i = 0;
  while (I2C.available())
  {
    data[i++] = I2C.read();
  }
  //DiagOut.print("I2C_Read "); DiagOut.println(i);
}

GxFT5436::TouchInfo::TouchInfo()
{
  clear();
}

void GxFT5436::TouchInfo::clear()
{
  touch_count = 0;
  for (uint8_t i = 0; i < CFG_MAX_TOUCH_POINTS; i++)
  {
    x[i] = 0;
    y[i] = 0;
  }
}

bool GxFT5436::TouchInfo::operator==(TouchInfo to)
{
  return ((touch_count == to.touch_count) &&
          (x[0] == to.x[0]) && (y[0] == to.y[0]) &&
          (x[1] == to.x[1]) && (y[1] == to.y[1]) &&
          (x[2] == to.x[2]) && (y[2] == to.y[2]) &&
          (x[3] == to.x[3]) && (y[3] == to.y[3]) &&
          (x[4] == to.x[4]) && (y[4] == to.y[4]));
}

#endif

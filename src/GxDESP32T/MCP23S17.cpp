// Class MCP23S17 : display IO helper class for GDE06BA on my DESP32T (proto board) for TCon-11 parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html
//
// this class provides the 16bit parallel connection for the AVT6203A data bus of the DESTM32-Tcon-11.

#if defined(ARDUINO_ARCH_ESP32)

#include "MCP23S17.h"

// my proto board has less optimal wiring, port A and B swapped
//#define PORTS_SWAPPED

// using BANK 0 addressing allows direct sequential write for 16bit bus
#define USE_BANK 0

#define IOCONRST 0x0B // BANK 0 active after reset

#if USE_BANK == 0

// defines for BANK = 1
#define IODIRA   0x00
#define IODIRB   0x01
#define IPOLA    0x02
#define IPOLB    0x03
#define GPINTENA 0x04
#define GPINTENB 0x05
#define DEFVALA  0x06
#define DEFVALB  0x07
#define INTCONA  0x08
#define INTCONB  0x09
#define IOCONA   0x0A
#define IOCONB   0x0B
#define GPPUA    0x0C
#define GPPUB    0x0D
#define INTFA    0x0E
#define INTFB    0x0F
#define INTCAPA  0x10
#define INTCAPB  0x11
#define GPIOA    0x12
#define GPIOB    0x13
#define OLATA    0x14
#define OLATB    0x15

// BANK MIRROR SEQOP DISSLW HAEN ODR INTPOL —
#define IOCONVAL 0x00 // BANK 0, SEQOP

#elif USE_BANK == 1

// defines for BANK = 1
#define IODIRA   0x00
#define IPOLA    0x01
#define GPINTENA 0x02
#define DEFVALA  0x03
#define INTCONA  0x04
#define IOCONA   0x05
#define GPPUA    0x06
#define INTFA    0x07
#define INTCAPA  0x08
#define GPIOA    0x09
#define OLATA    0x0A

#define IODIRB   0x10
#define IPOLB    0x11
#define GPINTENB 0x12
#define DEFVALB  0x13
#define INTCONB  0x14
#define GPPUB    0x16
#define INTFB    0x17
#define INTCAPB  0x18
#define GPIOB    0x19
#define OLATB    0x1A

// BANK MIRROR SEQOP DISSLW HAEN ODR INTPOL —
#define IOCONVAL 0xA0 // BANK 1, no SEQOP

#endif

MCP23S17::MCP23S17() : _spi_settings(10000000, MSBFIRST, SPI_MODE0)
{
}

void MCP23S17::init(uint8_t cs)
{
  _cs = cs;
  digitalWrite(_cs, HIGH);
  pinMode(_cs, OUTPUT);
  SPI.begin();
  _writeByte(IOCONRST, IOCONVAL);
  _writeByte(IOCONA, IOCONVAL);
  _writeByte(IODIRA, 0x00); // output
  _writeByte(IODIRB, 0x00); // output
}

void MCP23S17::portModeA(uint8_t mode)
{
  _writeByte(IODIRA, mode == INPUT ? 0xFF : 0x00);
}

void MCP23S17::portModeB(uint8_t mode)
{
  _writeByte(IODIRB, mode == INPUT ? 0xFF : 0x00);
}

void MCP23S17::portMode16(uint8_t mode)
{
  _writeByte(IODIRA, mode == INPUT ? 0xFF : 0x00);
  _writeByte(IODIRB, mode == INPUT ? 0xFF : 0x00);
}

void MCP23S17::writeA(uint8_t value)
{
  _writeByte(OLATA, value);
}

void MCP23S17::writeB(uint8_t value)
{
  _writeByte(OLATB, value);
}

void MCP23S17::write16(uint16_t value)
{
  SPI.beginTransaction(_spi_settings);
#ifdef PORTS_SWAPPED
  value = (value >> 8) | (value << 8); // wrong wiring
#endif
#if USE_BANK == 0
  uint32_t cmd_data = (0x4000 | OLATA) << 16 | value;
  digitalWrite(_cs, LOW);
  SPI.write32(cmd_data);
  digitalWrite(_cs, HIGH);
#else
  _writeByte(OLATA, value);
  _writeByte(OLATB, value >> 8);
#endif
  SPI.endTransaction();
}

uint8_t MCP23S17::readA()
{
  return _readByte(GPIOA);
}

uint8_t MCP23S17::readB()
{
  return _readByte(GPIOB);
}

uint16_t MCP23S17::read16()
{
  uint16_t value = 0;
  SPI.beginTransaction(_spi_settings);
#if USE_BANK == 0
  uint16_t cmdaddr = 0x4100 | GPIOA;
  digitalWrite(_cs, LOW);
  SPI.write16(cmdaddr);
  value = SPI.transfer16(0xFFFF); // sequential
  digitalWrite(_cs, HIGH);
#else
  value = _readByte(GPIOA);
  value |= _readByte(GPIOB) << 8;
#endif
#ifdef PORTS_SWAPPED
  value = (value >> 8) | (value << 8); // wrong wiring
#endif
  SPI.endTransaction();
  return value;
}

void MCP23S17::startTransfer()
{
  SPI.beginTransaction(_spi_settings);
}

void MCP23S17::transfer16(uint16_t value)
{
#ifdef PORTS_SWAPPED
  value = (value >> 8) | (value << 8); // wrong wiring
#endif
#if USE_BANK == 0
  uint32_t cmd_data = (0x4000 | OLATA) << 16 | value;
  digitalWrite(_cs, LOW);
  SPI.write32(cmd_data);
  digitalWrite(_cs, HIGH);
#else
  _writeByte(OLATA, value);
  _writeByte(OLATB, value >> 8);
#endif
}

void MCP23S17::endTransfer()
{
  SPI.endTransaction();
}

void MCP23S17::_writeByte(uint8_t addr, uint8_t value)
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write(0x40); // R/W : w
  SPI.write(addr);
  SPI.write(value);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

uint8_t MCP23S17::_readByte(uint8_t addr)
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write(0x41); // R/W : r
  SPI.write(addr);
  uint8_t value = SPI.transfer(0xFF);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
  return value;
}

#endif

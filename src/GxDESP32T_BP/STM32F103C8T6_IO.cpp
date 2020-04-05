// STM32F103C8T6_IO : IO component class for GDE060BA on my DESP32T_BP (proto board) for TCon-11 for parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html

#if !defined(ARDUINO_ARCH_STM32F1) && !defined(ARDUINO_ARCH_STM32)

#include "STM32F103C8T6_IO.h"

// 14MHz works, 16MHz doesn't work, using 12MHz should be safe
//STM32F103C8T6_IO::STM32F103C8T6_IO() : _spi_settings(8000000, MSBFIRST, SPI_MODE0)
STM32F103C8T6_IO::STM32F103C8T6_IO() : _spi_settings(12000000, MSBFIRST, SPI_MODE0)
{
}

void STM32F103C8T6_IO::init()
{
  digitalWrite(_cs, HIGH);
  pinMode(_cs, OUTPUT);
  SPI.begin();
}

void STM32F103C8T6_IO::end()
{
  SPI.end();
  pinMode(_cs, INPUT);
}

void STM32F103C8T6_IO::set_TPS_WAKEUP_H()
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(TPS_WAKEUP_H_id);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void STM32F103C8T6_IO::set_TPS_WAKEUP_L()
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(TPS_WAKEUP_L_id);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void STM32F103C8T6_IO::set_TPS_PWRUP_H()
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(TPS_PWRUP_H_id);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void STM32F103C8T6_IO::set_TPS_PWRUP_L()
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(TPS_PWRUP_L_id);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void STM32F103C8T6_IO::set_TPS_PWRCOM_H()
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(TPS_PWRCOM_H_id);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void STM32F103C8T6_IO::set_TPS_PWRCOM_L()
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(TPS_PWRCOM_L_id);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void STM32F103C8T6_IO::set_AVT_RST_H()
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(AVT_RST_H_id);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void STM32F103C8T6_IO::set_AVT_RST_L()
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(AVT_RST_L_id);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

bool STM32F103C8T6_IO::getAVT_RDY()
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  //SPI.transfer(0xFF); // flush read
  SPI.write16(AVT_RDY_id);
  delayMicroseconds(2);
  bool rdy = SPI.transfer16(0xFFFF);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
  return rdy;
}

void STM32F103C8T6_IO::writeCommand(uint16_t cmd)
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(WRITE_CMD_id);
  SPI.write16(cmd);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void STM32F103C8T6_IO::writeData(uint16_t data)
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(WRITE_DATA_id);
  SPI.write16(data);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

uint16_t STM32F103C8T6_IO::readData()
{
  uint16_t value;
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(READ_id);
  delayMicroseconds(10);
  value = SPI.transfer16(0xFFFF);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
  return value;
}

void STM32F103C8T6_IO::startTransfer(uint32_t length)
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_cs, LOW);
  SPI.write16(TRANSFER_id);
  SPI.write16(length << 16);
  SPI.write16(length);
}

void STM32F103C8T6_IO::transfer(uint16_t data)
{
  SPI.write16(data);
}

void STM32F103C8T6_IO::endTransfer()
{
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

#endif

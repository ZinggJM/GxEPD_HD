// Class MCP23S17 : display IO helper class for GDE060BA on my DESP32T (proto board) for TCon-11 parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html
//
// this class provides the 16bit parallel connection for the AVT6203A data bus of the DESTM32-Tcon-11.

#ifndef _MCP23S17_H_
#define _MCP23S17_H_

#include <Arduino.h>
#include <SPI.h>

class MCP23S17
{
  public:
    MCP23S17();
    void init(uint8_t cs);
    void portModeA(uint8_t mode); // INPUT, OUTPUT
    void portModeB(uint8_t mode); // INPUT, OUTPUT
    void portMode16(uint8_t mode); // INPUT, OUTPUT
    void writeA(uint8_t value);
    void writeB(uint8_t value);
    void write16(uint16_t value);
    uint8_t readA();
    uint8_t readB();
    uint16_t read16();
    void startTransfer();
    void transfer16(uint16_t value);
    void endTransfer();
  private:
    void _writeByte(uint8_t addr, uint8_t value);
    uint8_t _readByte(uint8_t addr);
  private:
    SPISettings _spi_settings;
    uint8_t _cs;
};

#endif

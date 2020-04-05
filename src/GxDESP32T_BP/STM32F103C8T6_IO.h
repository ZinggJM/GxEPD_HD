// STM32F103C8T6_IO : IO component class for GDE060BA on my DESP32T_BP (proto board) for TCon-11 for parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html

#ifndef _STM32F103C8T6_IO_H_
#define _STM32F103C8T6_IO_H_

#include <Arduino.h>
#include <SPI.h>

class STM32F103C8T6_IO
{
  public:
    enum SERVICE_ID
    {
      null_id,
      TPS_WAKEUP_H_id,
      TPS_WAKEUP_L_id,
      TPS_PWRUP_H_id,
      TPS_PWRUP_L_id,
      TPS_PWRCOM_H_id,
      TPS_PWRCOM_L_id,
      AVT_RST_H_id,
      AVT_RST_L_id,
      AVT_RDY_id,
      WRITE_CMD_id,
      WRITE_DATA_id,
      READ_id,
      TRANSFER_id,
    };
    STM32F103C8T6_IO();
    void init();
    void end();
    void set_TPS_WAKEUP_H();
    void set_TPS_WAKEUP_L();
    void set_TPS_PWRUP_H();
    void set_TPS_PWRUP_L();
    void set_TPS_PWRCOM_H();
    void set_TPS_PWRCOM_L();
    void set_AVT_RST_H();
    void set_AVT_RST_L();
    bool getAVT_RDY();
    void writeCommand(uint16_t cmd);
    void writeData(uint16_t data);
    uint16_t readData();
    void startTransfer(uint32_t length);
    void transfer(uint16_t data);
    void endTransfer();
  private:
    const int8_t _cs = SS;
    SPISettings _spi_settings;
};

#endif

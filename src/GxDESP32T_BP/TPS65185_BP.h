// Class TPS65185_BP : display IO component class for GDE060BA on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
// To be used with "STM32F103VE" of "Generic STM32F103V series" of package "STM32 Boards (STM32Duino.com)" for Arduino.
// install package with Boards Manager after adding to preferences in additional Boards Manager URLs:
// https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html

#ifndef _TPS65185_BP_H_
#define _TPS65185_BP_H_

#include <Arduino.h>
#include "STM32F103C8T6_IO.h"

class TPS65185_BP
{
  public:
    TPS65185_BP(STM32F103C8T6_IO& io) : IO(io), _vcom(2000), _pDiagnosticOutput(0) {};
    void tps_sleep_to_standby(void);
    void tps_standby_to_sleep(void);
    void tps_source_gate_enable(void);
    void tps_source_gate_disable(void);
    void tps_vcom_enable(void);
    void tps_vcom_disable(void);
    uint8_t ti_read_int_status(void);
    void tps_read_all_reg(void);
    void tps_init(uint16_t vcom, Stream* pDiagnosticOutput);
    void tps_end();
  private:
    void I2C_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t dat);
    uint8_t I2C_Read(uint8_t dev_addr, uint8_t reg_addr);
    void I2C_Write_Frame(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t* dat);
    void I2C_Read_Frame(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t* dat);
    void tps_power_sequence_set(void);
    void tps_vcom_set(uint16_t vcom);
    void tps_vposvneg_set(void);
    void Debug_str(const char *s);
    void Debug_hex(unsigned int dat);
    void Debug_dec(unsigned int dat);
  private:
    STM32F103C8T6_IO& IO;
    uint16_t _vcom;
    Stream* _pDiagnosticOutput;
};

#endif

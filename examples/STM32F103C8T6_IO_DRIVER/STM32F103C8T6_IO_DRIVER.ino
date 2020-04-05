// STM32F103C8T6_IO_Driver.ino : IO driver executable for GDE060BA on my DESP32T_BP (proto board) for TCon-11 for parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
// NOTE: THIS IS NOT AN EXAMPLE, BUT A DRIVER EXECUTABLE FOR THE STM32F103C8T6 ON DESP32T_BP PROTO BOARD
//
// To be used with "STM32F103C8" of "Generic STM32F103C series" of package "STM32 Boards (STM32Duino.com)" for Arduino.
// download this package as .zip file from https://github.com/rogerclarkmelbourne/Arduino_STM32
// and install it as described in https://github.com/rogerclarkmelbourne/Arduino_STM32/wiki/Installation
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html

#if defined(ARDUINO_ARCH_STM32F1)

#include <SPI.h>
#include <GxEPD_HD_EPD.h>
#include <GxDESP32T_BP/DESP32T_BP_wiring.h>
#include <GxDESP32T_BP/STM32F103C8T6_IO.h>

void setup()
{
  // see https://github.com/rogerclarkmelbourne/Arduino_STM32/issues/222
  Serial.end(); // needed for PA11, PA12 to work
  uint32_t mapr = AFIO_BASE->MAPR;
  mapr &= ~AFIO_MAPR_SWJ_CFG;
  mapr |= AFIO_MAPR_SWJ_CFG_NO_JTAG_SW;
  AFIO_BASE->MAPR = mapr; // remap JTAG pins as GPIO
  pinMode(PA2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  GPIOB_BASE->CRL = 0x33333333; // 00 11 output PP, 50MHz, pins 0..7, pin2 useless on BluePill
  GPIOB_BASE->CRH = 0x33333333; // 00 11 output PP, 50MHz, pins 8..15
  TPS_WAKEUP_L;
  TPS_PWRUP_L;
  TPS_PWRCOM_L;
  AVT_HCS_H;
  AVT_HDC_H;
  AVT_HRD_H;
  AVT_RST_H;
  AVT_HWE_H;
  pinMode(TPS_WAKEUP_PIN, OUTPUT);
  pinMode(TPS_PWRUP_PIN, OUTPUT);
  pinMode(TPS_PWRCOM_PIN, OUTPUT);
  pinMode(AVT_HCS_PIN, OUTPUT);
  pinMode(AVT_HDC_PIN, OUTPUT);
  pinMode(AVT_HRD_PIN, OUTPUT);
  pinMode(AVT_RST_PIN, OUTPUT);
  pinMode(AVT_HWE_PIN, OUTPUT);
  pinMode(TPS_WAKEUP_PIN, OUTPUT);
  pinMode(AVT_RDY_PIN, INPUT_PULLUP);
  SPI.beginTransactionSlave(SPISettings(4000000, MSBFIRST, SPI_MODE0, DATA_SIZE_16BIT));
  //pinMode(PC13, OUTPUT); // available
  //pinMode(PC14, OUTPUT); // available
  //pinMode(PC15, OUTPUT); // available
  //pinMode(PA15, OUTPUT); // available
}

void write16(uint16_t value)
{
  GPIOA_BASE->BRR = (0x1 << 2);
  GPIOA_BASE->BSRR = value & (0x1 << 2);
  GPIOB_BASE->ODR = value;
}

void loop()
{
#if 1 // STM32F103C8T6_IO_Driver use
  GPIOC_BASE->BSRR = (0x1 << 13);
  uint16_t cmd = SPI.read();
  GPIOC_BASE->BRR = (0x1 << 13);
  switch (cmd)
  {
    case STM32F103C8T6_IO::TPS_WAKEUP_H_id:
      TPS_WAKEUP_H;
      break;
    case STM32F103C8T6_IO::TPS_WAKEUP_L_id:
      TPS_WAKEUP_L;
      break;
    case STM32F103C8T6_IO::TPS_PWRUP_H_id:
      TPS_PWRUP_H;
      break;
    case STM32F103C8T6_IO::TPS_PWRUP_L_id:
      TPS_PWRUP_L;
      break;
    case STM32F103C8T6_IO::TPS_PWRCOM_H_id:
      TPS_PWRCOM_H;
      break;
    case STM32F103C8T6_IO::TPS_PWRCOM_L_id:
      TPS_PWRCOM_L;
      break;
    case STM32F103C8T6_IO::AVT_RST_H_id:
      AVT_RST_H;
      break;
    case STM32F103C8T6_IO::AVT_RST_L_id:
      AVT_RST_L;
      break;
    case STM32F103C8T6_IO::AVT_RDY_id:
      SPI.write(AVT_RDY);
      break;
    case STM32F103C8T6_IO::WRITE_CMD_id:
      {
        uint16_t avt_cmd = SPI.read();
        write16(avt_cmd);
        AVT_HDC_L;
        AVT_HDC_L;
        AVT_HCS_L;
        AVT_HCS_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_H;
        AVT_HCS_H;
        AVT_HDC_H;
      }
      break;
    case STM32F103C8T6_IO::WRITE_DATA_id:
      {
        uint16_t data = SPI.read();
        write16(data);
        AVT_HCS_L;
        AVT_HCS_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_L;
        AVT_HWE_H;
        AVT_HCS_H;
      }
      break;
    case STM32F103C8T6_IO::READ_id:
      {
        uint16_t value;
        pinMode(PA2, INPUT);
        GPIOB_BASE->CRL = 0x44444444; // 01 00 input floating, pins 0..7, pin2 useless on BluePill
        GPIOB_BASE->CRH = 0x44444444; // 01 00 input floating, pins 8..15
        AVT_HCS_L;
        AVT_HCS_L;
        AVT_HDC_H;
        AVT_HRD_L; // 200ns until data ready
        AVT_HRD_L; // 280ns gives some margin
        AVT_HRD_L; // use 10 * 2 clock cycles à 14ns (72MHz)
        AVT_HRD_L; // 4
        AVT_HRD_L; // 5
        AVT_HRD_L; // 6
        AVT_HRD_L; // 7
        AVT_HRD_L; // 8
        AVT_HRD_L; // 9
        AVT_HRD_L; // 10
        //AVT_HRD_H; // for logic analyzer measurement
        value = GPIOB_BASE->IDR & 0xFFFB;
        value |= GPIOA_BASE->IDR & 0x0004;
        AVT_HRD_H;
        AVT_HCS_H;
        write16(0xFFFF);
        pinMode(PA2, OUTPUT);
        GPIOB_BASE->CRL = 0x33333333; // 00 11 output PP, 50MHz, pins 0..7, pin2 useless on BluePill
        GPIOB_BASE->CRH = 0x33333333; // 00 11 output PP, 50MHz, pins 8..15
        SPI.write(value);
      }
      break;
    case STM32F103C8T6_IO::TRANSFER_id:
      {
        uint32_t length = SPI.read() << 16;
        length |= SPI.read();;
        AVT_HCS_L;
        while (length > 0)
        {
          uint16_t data = SPI.read();
          //AVT_HCS_L;
          //write16(data);
          GPIOA_BASE->BRR = (0x1 << 2);
          GPIOA_BASE->BSRR = data & (0x1 << 2);
          GPIOB_BASE->ODR = data;
          AVT_HWE_L;
          AVT_HWE_H;
          //AVT_HCS_H;
          length--;
        }
        AVT_HCS_H;
      }
      break;
  }
#elif 1 // for test of control pins output
  static bool high = false;
  digitalWrite(LED_BUILTIN, high);
  digitalWrite(SYS_WAKEUP_PIN, high);
  digitalWrite(AVT_HCS_PIN, high);
  digitalWrite(AVT_HDC_PIN, high);
  digitalWrite(AVT_HRD_PIN, high);
  digitalWrite(AVT_RST_PIN, high);
  digitalWrite(AVT_HWE_PIN, high);
  digitalWrite(TPS_WAKEUP_PIN, high);
  //digitalWrite(PC13, high); // available
  //digitalWrite(PC14, high); // available
  //digitalWrite(PC15, high); // available
  //digitalWrite(PA15, high); // available
  high = !high;
  delay(2000);
#else // for test of AVT data bus pins
  static bool high = false;
  //uint16_t onepin = 0x0100;
  uint16_t onepin = 0x0001;
  for (int16_t i = 0; i < 16; i++)
  {
    write16(onepin);
    delay(1000);
    onepin <<= 1;
    digitalWrite(LED_BUILTIN, high);
    high = !high;
  }
  delay(2000);
#endif
}

#endif

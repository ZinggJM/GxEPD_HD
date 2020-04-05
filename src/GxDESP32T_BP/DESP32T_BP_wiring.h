// DESP32T_BP_wiring : IO port definitions for display IO component class for GDE060BA on my DESP32T_BP (proto board) for TCon-11 for parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
// This DESP32T_BP proto board uses a STM32F103C8T6 BluePill as SPI Slave as port expander to the TCon-11
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html

// processor on proto board is STM32F103C8T6, e.g. on board known as BluePill

// TCon-11 schematics: http://www.e-paper-display.com/download_detail/downloadsId=562.html
//
// warning: output connector J1 has pin 1 of schematics on bottom left, top (component) view!
//
// NOTE: the STM32F103C8T6 supply is taken from VCC 3.3V out (7,8) of J14 for minimum power use during hibernate
//
// wiring list for 40pin input connector J14, top (component) view, pin 1 is top left
// pin nr., signal name -- destination pin, destination
//
// 1  GND              --  GND
// 2  GND              --  GND
// 3  GND              --  GND
// 4  GND              --  GND
// 5  GND              --  GND
// 6  GND              --  GND
// 7  VCC 3.3V out     --  3.3V pin of STM32F103C8T6 BluePill
// 8  VCC 3.3V out     --  3.3V pin of STM32F103C8T6 BluePill
// 9  HDB0_AVT6203A    --  B0 STM32F103C8T6
// 10 HDB1_AVT6203A    --  B1
// 11 HDB2_AVT6203A    --  (B2) A2 on BluePill (no B2 pin)
// 12 HDB3_AVT6203A    --  B3
// 13 HDB4_AVT6203A    --  B4
// 14 HDB5_AVT6203A    --  B5
// 15 HDB6_AVT6203A    --  B6
// 16 HDB7_AVT6203A    --  B7
// 17 HDB8_AVT6203A    --  B8
// 18 HDB9_AVT6203A    --  B9
// 19 HDB10_AVT6203A   --  B10
// 20 HDB11_AVT6203A   --  B11
// 21 HDB12_AVT6203A   --  B12
// 22 HDB13_AVT6203A   --  B13
// 23 HDB14_AVT6203A   --  B14
// 24 HDB15_AVT6203A   --  B15
// 25 PWR0_TPS65185    --  A15 STM32F103C8T6
// 26 PWRCOM_TPS65185  --  A12 STM32F103C8T6
// 27 PWM_SDA_TPS65185 --  21 ESP32 and pinhole
// 28 PWM_SCL_TPS65185 --  22 ESP32 and pinhole
// 29 HRD_AVT6203A     --  A0 STM32
// 30 HDC_AVT6203A     --  A8
// 31 HRDY_AVT6203A    --  A1
// 32 RESET_AVT6203A   --  A9
// 33 HWE_AVT6203A     --  A3
// 34 HCS_AVT6203A     --  A10
// 35 HRQ_AVT6203A     --  not connected
// 36 WAKEUP_TPS65185  --  A11
// 37 SYSWAKEUP        --   4 ESP32 and pinhole
// 38 GND              -- GND and pinhole and external power in
// 39 VIN 5V           -- VIN ESP32 and pinhole and external power in
// 40 VIN 5V           -- VIN ESP32 and pinhole and external power in
//
// wiring of inputs to STM32F103C8T6
//
// A4 SS    --   5 SS   ESP32 and pinhole
// A5 SCK   --  18 SCK  ESP32 and pinhole
// A6 MISO  --  19 MISO ESP32 and pinhole
// A7 MOSI  --  23 MOSI ESP32 and pinhole
// R  reset --  15 IO15 ESP32 and pinhole, optional reset, (not used yet), (or any pin except ESP32 strapping pins 0, 2, 5)
// GND      --  GND
// 3.3V     --  3.3V    J14 7,8 3.3V out
//
// NOTE: all GNDs are connected together
//       pins carrying the same signal are connected together

// NOTE: pinholes could serve as external processor connection, as alternative to on board ESP32
//       e.g. 2.54mm pinholes in line
// NOTE: external power in is better than supply from ESP32 USB, because of protection diode voltage drop

#ifndef _DESP32T_BP_wiring_H_
#define _DESP32T_BP_wiring_H_

//#define TPS_SCL_PIN  22 // HW SCL
//#define TPS_SDA_PIN  21 // HW SDA
#define SYS_WAKEUP_PIN  4
#define SYS_WAKEUP_H    digitalWrite(SYS_WAKEUP_PIN, HIGH)
#define SYS_WAKEUP_L    digitalWrite(SYS_WAKEUP_PIN, LOW)
#define TPS_SCL_H       digitalWrite(TPS_SCL_PIN, HIGH)
#define TPS_SCL_L       digitalWrite(TPS_SCL_PIN, LOW)
#define TPS_SDA_H       digitalWrite(TPS_SDA_PIN, HIGH)
#define TPS_SDA_L       digitalWrite(TPS_SDA_PIN, LOW)
#define TPS_SDA         digitalRead(TPS_SDA_PIN)

#if !defined(ARDUINO_ARCH_STM32F1)

#define TPS_WAKEUP_H IO.set_TPS_WAKEUP_H()
#define TPS_WAKEUP_L IO.set_TPS_WAKEUP_L()
#define TPS_PWRCOM_H IO.set_TPS_PWRCOM_H()
#define TPS_PWRCOM_L IO.set_TPS_PWRCOM_L()
#define TPS_PWRUP_H  IO.set_TPS_PWRUP_H()
#define TPS_PWRUP_L  IO.set_TPS_PWRUP_L()
#define AVT_RST_H    IO.set_AVT_RST_H()
#define AVT_RST_L    IO.set_AVT_RST_L()

#else

#define AVT_HRD_PIN       PA0
#define AVT_HDC_PIN       PA8
#define AVT_RDY_PIN       PA1
#define AVT_RST_PIN       PA9
#define AVT_HWE_PIN       PA3
#define AVT_HCS_PIN       PA10
//#define AVT_IRQ_PIN     NC
#define TPS_WAKEUP_PIN    PA11
#define TPS_PWRUP_PIN     PA15
#define TPS_PWRCOM_PIN    PA12

#define AVT_RST_H     GPIOA_BASE->BSRR = (0x1 << 9)
#define AVT_RST_L     GPIOA_BASE->BRR  = (0x1 << 9)
#define AVT_HDC_H     GPIOA_BASE->BSRR = (0x1 << 8)
#define AVT_HDC_L     GPIOA_BASE->BRR  = (0x1 << 8)
#define AVT_HCS_H     GPIOA_BASE->BSRR = (0x1 << 10)
#define AVT_HCS_L     GPIOA_BASE->BRR  = (0x1 << 10)
#define AVT_HWE_H     GPIOA_BASE->BSRR = (0x1 << 3)
#define AVT_HWE_L     GPIOA_BASE->BRR  = (0x1 << 3)
#define AVT_HRD_H     GPIOA_BASE->BSRR = (0x1 << 0)
#define AVT_HRD_L     GPIOA_BASE->BRR  = (0x1 << 0)
#define AVT_RDY       GPIOA_BASE->IDR & (0x1 << 1)
#define TPS_WAKEUP_H  GPIOA_BASE->BSRR = (0x1 << 11)
#define TPS_WAKEUP_L  GPIOA_BASE->BRR  = (0x1 << 11)
#define TPS_PWRUP_H   GPIOA_BASE->BSRR = (0x1 << 15)
#define TPS_PWRUP_L   GPIOA_BASE->BRR  = (0x1 << 15)
#define TPS_PWRCOM_H  GPIOA_BASE->BSRR = (0x1 << 12)
#define TPS_PWRCOM_L  GPIOA_BASE->BRR  = (0x1 << 12)

#endif

#endif

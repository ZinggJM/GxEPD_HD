// DESP32T_wiring : IO port definitions for display IO component class for GDE060BA on my DESP32T (proto board) for TCon-11 for parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
// This DESP32T proto board uses a MCP23S17 SPI to 16 pin IO port expander for AVT6203A 16 bit bus
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html
//
// processor on proto board is ESP32-Wrover, e.g. https://www.aliexpress.com/item/4000165144198.html
//
// TCon-11 schematics: http://www.e-paper-display.com/download_detail/downloadsId=562.html
//
// warning: output connector J1 has pin 1 of schematics on bottom left, top (component) view!
//
// NOTE: the MCP23S17 supply can be taken from VCC 3.3V out (7,8) of J14 for minimum power use during hibernate
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
// 7  VCC 3.3V out     --  VCC MCP23S17
// 8  VCC 3.3V out     --  VCC MCP23S17
// 9  HDB0_AVT6203A    --  B0  MCP23S17
// 10 HDB1_AVT6203A    --  B1
// 11 HDB2_AVT6203A    --  B2
// 12 HDB3_AVT6203A    --  B3
// 13 HDB4_AVT6203A    --  B4
// 14 HDB5_AVT6203A    --  B5
// 15 HDB6_AVT6203A    --  B6
// 16 HDB7_AVT6203A    --  B7
// 17 HDB8_AVT6203A    --  A0
// 18 HDB9_AVT6203A    --  A1
// 19 HDB10_AVT6203A   --  A2
// 20 HDB11_AVT6203A   --  A3
// 21 HDB12_AVT6203A   --  A4
// 22 HDB13_AVT6203A   --  A5
// 23 HDB14_AVT6203A   --  A6
// 24 HDB15_AVT6203A   --  A7
// 25 PWR0_TPS65185    --  34 ESP32
// 26 PWRCOM_TPS65185  --   4
// 27 PWM_SDA_TPS65185 --  21
// 28 PWM_SCL_TPS65185 --  22
// 29 HRD_AVT6203A     --  12
// 30 HDC_AVT6203A     --  13
// 31 HRDY_AVT6203A    --  32
// 32 RESET_AVT6203A   --  15
// 33 HWE_AVT6203A     --  33
// 34 HCS_AVT6203A     --  14
// 35 HRQ_AVT6203A     --  25
// 36 WAKEUP_TPS65185  --  26
// 37 SYSWAKEUP        --  27
// 38 GND              -- GND
// 39 VIN 5V           -- VIN ESP32
// 40 VIN 5V           -- VIN ESP32
//
// wiring list of input side of MCP23S17
//
// A2      --  GND
// A1      --  GND
// A0      --  GND
// RESET   --  3V3 ESP32 or J14 VCC 3.3V out
// NC/SO   --  19  ESP32 MISO
// NC/CS   --   5  ESP32 SS
// SDA/SI  --  23  ESP32 MOSI
// SCL/SCK --  18  ESP32 SCK
// GND     --  GND
// VCC     -- 3V3 ESP32 or J14 VCC 3.3V out

#ifndef _DESP32T_wiring_H_
#define _DESP32T_wiring_H_

#define  DEBUG_USART

//#define TPS_SCL_PIN     22 // HW SCL
//#define TPS_SDA_PIN     21 // HW SDA
#define TPS_PWRUP_PIN     34
#define TPS_PWRCOM_PIN     4

#define SYS_WAKEUP_PIN    27

#define AVT_HRD_PIN       12

#define AVT_HDC_PIN       13 

#define AVT_RDY_PIN       32

#define AVT_RST_PIN       15

#define AVT_HWE_PIN       33

#define AVT_HCS_PIN       14

#define AVT_IRQ_PIN       25

#define TPS_WAKEUP_PIN    26

#define AVT_RST_H     digitalWrite(AVT_RST_PIN, HIGH)
#define AVT_RST_L     digitalWrite(AVT_RST_PIN, LOW)

#define AVT_HDC_H     digitalWrite(AVT_HDC_PIN, HIGH)
#define AVT_HDC_L     digitalWrite(AVT_HDC_PIN, LOW)

#define AVT_HCS_H     digitalWrite(AVT_HCS_PIN, HIGH)
#define AVT_HCS_L     digitalWrite(AVT_HCS_PIN, LOW)

#define AVT_HWE_H     digitalWrite(AVT_HWE_PIN, HIGH)
#define AVT_HWE_L     digitalWrite(AVT_HWE_PIN, LOW)

#define AVT_HRD_H     digitalWrite(AVT_HRD_PIN, HIGH)
#define AVT_HRD_L     digitalWrite(AVT_HRD_PIN, LOW)

#define AVT_RDY       digitalRead(AVT_RDY_PIN)
#define AVT_IRQ       digitalRead(AVT_IRQ_PIN)

#define TPS_WAKEUP_H      digitalWrite(TPS_WAKEUP_PIN, HIGH)
#define TPS_WAKEUP_L      digitalWrite(TPS_WAKEUP_PIN, LOW)

#define TPS_PWRCOM_H      digitalWrite(TPS_PWRCOM_PIN, HIGH)
#define TPS_PWRCOM_L      digitalWrite(TPS_PWRCOM_PIN, LOW)

#define TPS_PWRUP_H     digitalWrite(TPS_PWRUP_PIN, HIGH)
#define TPS_PWRUP_L     digitalWrite(TPS_PWRUP_PIN, LOW)

#define TPS_SCL_H     digitalWrite(TPS_SCL_PIN, HIGH)
#define TPS_SCL_L     digitalWrite(TPS_SCL_PIN, LOW)

#define TPS_SDA_H     digitalWrite(TPS_SDA_PIN, HIGH)
#define TPS_SDA_L     digitalWrite(TPS_SDA_PIN, LOW)
#define TPS_SDA       digitalRead(TPS_SDA_PIN)

#define SYS_WAKEUP_H      digitalWrite(SYS_WAKEUP_PIN, HIGH)
#define SYS_WAKEUP_L      digitalWrite(SYS_WAKEUP_PIN, LOW)

#endif

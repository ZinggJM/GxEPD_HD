// DESTM32L1_board.h : IO port definitions for display IO component class for GDE060BA on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
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

// processor on demo board is STM32F103VET6

// suitable STM32 Arduino package is:
// STM32 Boards (STM32Duino.com)
// Board: "Generic STM32F103V series"
// Optimize: "Smallest (default)"
// Variant: "STM32F103VE"
// CPU Speed(MHz): "72MHz (Normal)"
// Upload method: "STLink"

// this package/board has matching SPI:
// from:
// C:\Users\xxx\Documents\Arduino\hardware\Arduino_STM32\STM32F1\variants\generic_stm32f103v\board\board.h
//#define BOARD_SPI1_NSS_PIN      PA4
//#define BOARD_SPI1_SCK_PIN      PA5
//#define BOARD_SPI1_MISO_PIN     PA6
//#define BOARD_SPI1_MOSI_PIN     PA7
// from:
// C:\Users\xxx\Documents\Arduino\hardware\Arduino_STM32\STM32F1\variants\generic_stm32f103v\variant.h
//static const uint8_t SS   = BOARD_SPI1_NSS_PIN;
//static const uint8_t SS1  = BOARD_SPI2_NSS_PIN;
//static const uint8_t MOSI = BOARD_SPI1_MOSI_PIN;
//static const uint8_t MISO = BOARD_SPI1_MISO_PIN;
//static const uint8_t SCK  = BOARD_SPI1_SCK_PIN;
// from:
// C:\Users\xxx\Documents\Arduino\hardware\Arduino_STM32\STM32F1\libraries\SPI\src\SPI.cpp
//SPIClass SPI(1);

// where to find types, declarations and #defines:
// C:\Users\xxx\Documents\Arduino\hardware\Arduino_STM32\STM32F1\system\libmaple\include\libmaple
// C:\Users\xxx\Documents\Arduino\hardware\Arduino_STM32\STM32F1\cores\maple e.g. io.h
#ifndef _DESTM32L1_board_H_
#define _DESTM32L1_board_H_

#if (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V))

#define GPIO_PIN_0 (0x1<<0)
#define GPIO_PIN_1 (0x1<<1)
#define GPIO_PIN_2 (0x1<<2)
#define GPIO_PIN_3 (0x1<<3)
#define GPIO_PIN_4 (0x1<<4)
#define GPIO_PIN_5 (0x1<<5)
#define GPIO_PIN_6 (0x1<<6)
#define GPIO_PIN_7 (0x1<<7)
#define GPIO_PIN_8 (0x1<<8)
#define GPIO_PIN_9 (0x1<<9)
#define GPIO_PIN_10 (0x1<<10)
#define GPIO_PIN_11 (0x1<<11)
#define GPIO_PIN_12 (0x1<<12)
#define GPIO_PIN_13 (0x1<<13)
#define GPIO_PIN_14 (0x1<<14)
#define GPIO_PIN_15 (0x1<<15)

#define GPIOA GPIOA_BASE
#define GPIOB GPIOB_BASE
#define GPIOC GPIOC_BASE
#define GPIOD GPIOD_BASE
#define GPIOE GPIOE_BASE

#endif

#define  DEBUG_USART

#define FLASH_PAGE_SIZE     2048

//#define FLASH_CS_PORT     GPIOA     //OUT - PP
//#define FLASH_CS_PIN      GPIO_PIN_4
//#define FLASH_CS_PP       PA4

#define LED_03_PORT       GPIOA     //OUT - PP    H:ON
#define LED_03_PIN        GPIO_PIN_8
#define LED_04_PORT       GPIOA     //OUT - PP    H:ON
#define LED_04_PIN        GPIO_PIN_11

#define TPS_SCL_PORT      GPIOB     //OUT - OD        yes
#define TPS_SCL_PIN       GPIO_PIN_15
#define TPS_SCL_PP        PB15
#define TPS_SDA_PORT      GPIOB     //OUT - OD        yes
#define TPS_SDA_PIN       GPIO_PIN_14
#define TPS_SDA_PP        PB14
#define TPS_PWRUP_PORT    GPIOB     //OUT - PP    H:EN          yes
#define TPS_PWRUP_PIN     GPIO_PIN_12
#define TPS_PWRUP_PP      PB12
#define TPS_PWRCOM_PORT   GPIOB     //OUT - PP    H:PWRCOM    yes   ?-?aPB3
#define TPS_PWRCOM_PIN    GPIO_PIN_13
#define TPS_PWRCOM_PP     PB13

#define SYS_WAKEUP_PORT   GPIOC     //OUT - PP    H:EN     SYSWAKEUP   yes   ?-?aPB4
#define SYS_WAKEUP_PIN    GPIO_PIN_6
#define SYS_WAKEUP_PP     PC6

#define LED_01_PORT       GPIOC     //OUT - PP    H:ON
#define LED_01_PIN        GPIO_PIN_8
#define LED_02_PORT       GPIOC     //OUT - PP    H:ON
#define LED_02_PIN        GPIO_PIN_9

#define AVT_HRD_PORT      GPIOD     //OUT - PP    L:EN       yes
#define AVT_HRD_PIN       GPIO_PIN_8
#define AVT_HRD_PP        PD8

#define AVT_HDC_PORT      GPIOD     //OUT - PP
#define AVT_HDC_PIN       GPIO_PIN_9  
#define AVT_HDC_PP        PD9

#define AVT_RDY_PORT      GPIOD     //IN - NOPULL   RDY           yes 
#define AVT_RDY_PIN       GPIO_PIN_10
#define AVT_RDY_PP        PD10

#define AVT_RST_PORT      GPIOD     //OUT - PP    L:Reset     yes 
#define AVT_RST_PIN       GPIO_PIN_11
#define AVT_RST_PP        PD11

#define AVT_HWE_PORT      GPIOD     //OUT - PP    L:EN    yes
#define AVT_HWE_PIN       GPIO_PIN_12
#define AVT_HWE_PP        PD12

#define AVT_HCS_PORT      GPIOD     //OUT - PP    L:EN    yes   ?-?aPA15
#define AVT_HCS_PIN       GPIO_PIN_13
#define AVT_HCS_PP        PD13

#define AVT_IRQ_PORT      GPIOD     //IN - NOPULL            yes
#define AVT_IRQ_PIN       GPIO_PIN_14
#define AVT_IRQ_PP        PD14

#define TPS_WAKEUP_PORT   GPIOD     //OUT - PP    H:EN       yes
#define TPS_WAKEUP_PIN    GPIO_PIN_15
#define TPS_WAKEUP_PP     PD15

#define AVT_DAT_PORT      GPIOE     //NC    2��?��
#define AVT_DAT_PIN       0xFFFF
#define AVT_DAT_PP0       PE0

//#define PORT_A_OUT_PP       FLASH_CS_PIN   
//#define PORT_B_OUT_PP       TPS_PWRCOM_PIN | TPS_PWRUP_PIN 
//#define PORT_B_OUT_OD       TPS_SCL_PIN|TPS_SDA_PIN
//#define PORT_C_OUT_PP       SYS_WAKEUP_PIN
//#define PORT_D_OUT_PP       AVT_HCS_PIN | AVT_HDC_PIN | AVT_HRD_PIN | AVT_RST_PIN | AVT_HWE_PIN | TPS_WAKEUP_PIN   
//#define PORT_D_IN_NOPULL    AVT_RDY_PIN | AVT_IRQ_PIN

#define USB_EN_H              (USB_EN_PORT->BSRR = USB_EN_PIN)  
#define USB_EN_L              (USB_EN_PORT->BRR  = USB_EN_PIN)  

#define LED_01_ON             (LED_01_PORT->BSRR  = LED_01_PIN)   
#define LED_01_OFF            (LED_01_PORT->BRR = LED_01_PIN)   

#define LED_02_ON             (LED_02_PORT->BSRR  = LED_02_PIN)   
#define LED_02_OFF            (LED_02_PORT->BRR = LED_02_PIN)

#define LED_03_ON             (LED_03_PORT->BSRR  = LED_03_PIN)   
#define LED_03_OFF            (LED_03_PORT->BRR = LED_03_PIN)   

#define LED_04_ON             (LED_04_PORT->BSRR  = LED_04_PIN)   
#define LED_04_OFF            (LED_04_PORT->BRR = LED_04_PIN)

#define FLASH_CS_H            (FLASH_CS_PORT->BSRR = FLASH_CS_PIN)  
#define FLASH_CS_L            (FLASH_CS_PORT->BRR  = FLASH_CS_PIN)  

#define AVT_RST_H     (AVT_RST_PORT->BSRR = AVT_RST_PIN)
#define AVT_RST_L     (AVT_RST_PORT->BRR =  AVT_RST_PIN)

#define AVT_HDC_H     (AVT_HDC_PORT->BSRR = AVT_HDC_PIN)
#define AVT_HDC_L     (AVT_HDC_PORT->BRR =  AVT_HDC_PIN)

#define AVT_HCS_H     (AVT_HCS_PORT->BSRR = AVT_HCS_PIN)
#define AVT_HCS_L     (AVT_HCS_PORT->BRR =  AVT_HCS_PIN)

#define AVT_HWE_H     (AVT_HWE_PORT->BSRR = AVT_HWE_PIN)
#define AVT_HWE_L     (AVT_HWE_PORT->BRR =  AVT_HWE_PIN)

#define AVT_HRD_H     (AVT_HRD_PORT->BSRR = AVT_HRD_PIN)
#define AVT_HRD_L     (AVT_HRD_PORT->BRR =  AVT_HRD_PIN)

#define AVT_RDY       (AVT_RDY_PORT->IDR & AVT_RDY_PIN)
#define AVT_IRQ       (AVT_IRQ_PORT->IDR & AVT_IRQ_PIN)

#define AVT_DAT_IN      (AVT_DAT_PORT->IDR)
#define AVT_DAT_OUT(d)      (AVT_DAT_PORT->ODR = d)
#define AVT_DAT_SETOUT      {AVT_DAT_PORT->CRL = 0x33333333; AVT_DAT_PORT->CRH = 0x33333333;}
#define AVT_DAT_SETIN     {AVT_DAT_PORT->CRL = 0x44444444; AVT_DAT_PORT->CRH = 0x44444444;}

#define TPS_WAKEUP_H      (TPS_WAKEUP_PORT->BSRR = TPS_WAKEUP_PIN)
#define TPS_WAKEUP_L      (TPS_WAKEUP_PORT->BRR =  TPS_WAKEUP_PIN)

#define TPS_PWRCOM_H      (TPS_PWRCOM_PORT->BSRR = TPS_PWRCOM_PIN)
#define TPS_PWRCOM_L      (TPS_PWRCOM_PORT->BRR =  TPS_PWRCOM_PIN)

#define TPS_PWRUP_H     (TPS_PWRUP_PORT->BSRR = TPS_PWRUP_PIN)
#define TPS_PWRUP_L     (TPS_PWRUP_PORT->BRR =  TPS_PWRUP_PIN)

#define TPS_SCL_H     (TPS_SCL_PORT->BSRR = TPS_SCL_PIN)
#define TPS_SCL_L     (TPS_SCL_PORT->BRR =  TPS_SCL_PIN)

#define TPS_SDA_H     (TPS_SDA_PORT->BSRR = TPS_SDA_PIN)
#define TPS_SDA_L     (TPS_SDA_PORT->BRR =  TPS_SDA_PIN)
#define TPS_SDA       (TPS_SDA_PORT->IDR &  TPS_SDA_PIN)

#define SYS_WAKEUP_H      (SYS_WAKEUP_PORT->BSRR = SYS_WAKEUP_PIN)
#define SYS_WAKEUP_L      (SYS_WAKEUP_PORT->BRR =  SYS_WAKEUP_PIN)

#endif

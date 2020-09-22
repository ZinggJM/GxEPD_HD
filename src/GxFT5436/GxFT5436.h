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

#ifndef _GxFT5436_H_
#define _GxFT5436_H_

#include <Arduino.h>
#include <Wire.h>

// defines taken from ft5x06.c of https://github.com/focaltech-systems/drivers-input-touchscreen-FTS_driver
/*

   FocalTech ft5x06 TouchScreen driver.

   Copyright (c) 2010  Focal tech Ltd.
   Copyright (c) 2012-2014, The Linux Foundation. All rights reserved.

   This software is licensed under the terms of the GNU General Public
   License version 2, as published by the Free Software Foundation, and
   may be copied, distributed, and modified under those terms.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

*/
#define FT_META_REGS    3
#define FT_ONE_TCH_LEN    6
#define FT_TCH_LEN(x)   (FT_META_REGS + FT_ONE_TCH_LEN * x)

#define CFG_MAX_TOUCH_POINTS  5 //10
#define FT_PRESS    0x7F
#define FT_MAX_ID   0x0F
#define FT_TOUCH_X_H_POS  3
#define FT_TOUCH_X_L_POS  4
#define FT_TOUCH_Y_H_POS  5
#define FT_TOUCH_Y_L_POS  6
#define FT_TD_STATUS    2
#define FT_TOUCH_EVENT_POS  3
#define FT_TOUCH_ID_POS   5
#define FT_TOUCH_DOWN   0
#define FT_TOUCH_CONTACT  2

#define POINT_READ_BUF  (3 + FT_ONE_TCH_LEN * CFG_MAX_TOUCH_POINTS)

/*register address*/
#define FT_REG_DEV_MODE   0x00
#define FT_DEV_MODE_REG_CAL 0x02
#define FT_REG_ID   0xA3
#define FT_REG_PMODE    0xA5
#define FT_REG_FW_VER   0xA6
#define FT_REG_FW_VENDOR_ID 0xA8
#define FT_REG_POINT_RATE 0x88
#define FT_REG_THGROUP    0x80
#define FT_REG_ECC    0xCC
#define FT_REG_RESET_FW   0x07
#define FT_REG_FW_MIN_VER 0xB2
#define FT_REG_FW_SUB_MIN_VER 0xB3

/* power register bits*/
#define FT_PMODE_ACTIVE   0x00
#define FT_PMODE_MONITOR  0x01
#define FT_PMODE_STANDBY  0x02
#define FT_PMODE_HIBERNATE  0x03
#define FT_FACTORYMODE_VALUE  0x40
#define FT_WORKMODE_VALUE 0x00
#define FT_RST_CMD_REG1   0xFC
#define FT_RST_CMD_REG2   0xBC
#define FT_READ_ID_REG    0x90
#define FT_ERASE_APP_REG  0x61
#define FT_ERASE_PANEL_REG  0x63
#define FT_FW_START_REG   0xBF

#define FT_STATUS_NUM_TP_MASK 0x0F

#define FT_VTG_MIN_UV   2600000
#define FT_VTG_MAX_UV   3300000
#define FT_I2C_VTG_MIN_UV 1800000
#define FT_I2C_VTG_MAX_UV 1800000

#define FT_COORDS_ARR_SIZE  4
#define MAX_BUTTONS   4

#define FT_8BIT_SHIFT   8
#define FT_4BIT_SHIFT   4
#define FT_FW_NAME_MAX_LEN  50

#define FT5X16_ID   0x0A
#define FT5X06_ID   0x55
#define FT6X06_ID   0x06
#define FT6X36_ID   0x36

#define FT_UPGRADE_AA   0xAA
#define FT_UPGRADE_55   0x55

#define FT_FW_MIN_SIZE    8
#define FT_FW_MAX_SIZE    (54 * 1024)
// end of defines taken from ft5x06.c of https://github.com/focaltech-systems/drivers-input-touchscreen-FTS_driver

#define FT5436_I2C_ADDR 0x38

class GxFT5436
{
  public:
    struct TouchInfo
    {
      uint8_t touch_count;
      uint16_t x[CFG_MAX_TOUCH_POINTS];
      uint16_t y[CFG_MAX_TOUCH_POINTS];
      TouchInfo();
      void clear();
      bool operator==(TouchInfo);
    };
  public:
    GxFT5436(int8_t rst);
    GxFT5436(int8_t sda, int8_t scl, int8_t rst);
    void init(Stream* pDiagnosticOutput = 0);
    // scans for touch(es) and returns the number (of fingers) and the coordinates of the first touch
    uint8_t scanSingleTouch(uint16_t& x, uint16_t& y);
    // scans for touches and returns the number and the coordinates
    TouchInfo scanMultipleTouch();
    // scans for touch(es) and returns the number (of fingers) and the coordinates of the first touch
    // returns zero if there is no change
    uint8_t newSingleTouch(uint16_t& x, uint16_t& y);
    // scans for touches and returns the number and the coordinates
    // returns zero if there is no change
    TouchInfo newMultipleTouch();
    // returns the touch(es) of the last scan (not useful)
    uint8_t lastSingleTouch(uint16_t& x, uint16_t& y);
    // returns the touches of the last scan; useful if scanSingleTouch() returned > 1
    TouchInfo lastMultipleTouch();
  protected:
    void scan();
    void check(const char text[], TouchInfo& touchinfo);
    void I2C_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
    uint8_t I2C_Read(uint8_t dev_addr, uint8_t reg_addr);
    void I2C_Read(uint8_t dev_addr, uint8_t reg_addr, uint8_t* data, uint8_t n);
  private:
    TwoWire I2C;
    int8_t _sda, _scl, _rst;
    uint8_t _registers[POINT_READ_BUF];
    TouchInfo _info[2];
    int16_t _act_idx, _prev_idx;
    Stream* _pDiagnosticOutput;
};

#endif

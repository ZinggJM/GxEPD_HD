// Class GxDESTM32T : display IO class for GDE060BA on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
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

#if defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)

#include "GxDESTM32T.h"
#include "DESTM32L1_board.h"
#include "picture800x600.h"
#include "picture1024x768.h"
//#include "BitmapExamples.h"
//#include "Bitmaps400x300.h"

#define SerialDiag if (_pDiagnosticOutput) (*_pDiagnosticOutput)

const uint8_t GxDESTM32T::bw2grey8[] =
{
  0b00000000, 0b00000011, 0b00001100, 0b00001111,
  0b00110000, 0b00110011, 0b00111100, 0b00111111,
  0b11000000, 0b11000011, 0b11001100, 0b11001111,
  0b11110000, 0b11110011, 0b11111100, 0b11111111,
};

const uint16_t GxDESTM32T::bw2grey16[] =
{
  0x0000, 0x0003, 0x000C, 0x000F, 0x0030, 0x0033, 0x003C, 0x003F, 0x00C0, 0x00C3, 0x00CC, 0x00CF, 0x00F0, 0x00F3, 0x00FC, 0x00FF,
  0x0300, 0x0303, 0x030C, 0x030F, 0x0330, 0x0333, 0x033C, 0x033F, 0x03C0, 0x03C3, 0x03CC, 0x03CF, 0x03F0, 0x03F3, 0x03FC, 0x03FF,
  0x0C00, 0x0C03, 0x0C0C, 0x0C0F, 0x0C30, 0x0C33, 0x0C3C, 0x0C3F, 0x0CC0, 0x0CC3, 0x0CCC, 0x0CCF, 0x0CF0, 0x0CF3, 0x0CFC, 0x0CFF,
  0x0F00, 0x0F03, 0x0F0C, 0x0F0F, 0x0F30, 0x0F33, 0x0F3C, 0x0F3F, 0x0FC0, 0x0FC3, 0x0FCC, 0x0FCF, 0x0FF0, 0x0FF3, 0x0FFC, 0x0FFF,
  0x3000, 0x3003, 0x300C, 0x300F, 0x3030, 0x3033, 0x303C, 0x303F, 0x30C0, 0x30C3, 0x30CC, 0x30CF, 0x30F0, 0x30F3, 0x30FC, 0x30FF,
  0x3300, 0x3303, 0x330C, 0x330F, 0x3330, 0x3333, 0x333C, 0x333F, 0x33C0, 0x33C3, 0x33CC, 0x33CF, 0x33F0, 0x33F3, 0x33FC, 0x33FF,
  0x3C00, 0x3C03, 0x3C0C, 0x3C0F, 0x3C30, 0x3C33, 0x3C3C, 0x3C3F, 0x3CC0, 0x3CC3, 0x3CCC, 0x3CCF, 0x3CF0, 0x3CF3, 0x3CFC, 0x3CFF,
  0x3F00, 0x3F03, 0x3F0C, 0x3F0F, 0x3F30, 0x3F33, 0x3F3C, 0x3F3F, 0x3FC0, 0x3FC3, 0x3FCC, 0x3FCF, 0x3FF0, 0x3FF3, 0x3FFC, 0x3FFF,
  0xC000, 0xC003, 0xC00C, 0xC00F, 0xC030, 0xC033, 0xC03C, 0xC03F, 0xC0C0, 0xC0C3, 0xC0CC, 0xC0CF, 0xC0F0, 0xC0F3, 0xC0FC, 0xC0FF,
  0xC300, 0xC303, 0xC30C, 0xC30F, 0xC330, 0xC333, 0xC33C, 0xC33F, 0xC3C0, 0xC3C3, 0xC3CC, 0xC3CF, 0xC3F0, 0xC3F3, 0xC3FC, 0xC3FF,
  0xCC00, 0xCC03, 0xCC0C, 0xCC0F, 0xCC30, 0xCC33, 0xCC3C, 0xCC3F, 0xCCC0, 0xCCC3, 0xCCCC, 0xCCCF, 0xCCF0, 0xCCF3, 0xCCFC, 0xCCFF,
  0xCF00, 0xCF03, 0xCF0C, 0xCF0F, 0xCF30, 0xCF33, 0xCF3C, 0xCF3F, 0xCFC0, 0xCFC3, 0xCFCC, 0xCFCF, 0xCFF0, 0xCFF3, 0xCFFC, 0xCFFF,
  0xF000, 0xF003, 0xF00C, 0xF00F, 0xF030, 0xF033, 0xF03C, 0xF03F, 0xF0C0, 0xF0C3, 0xF0CC, 0xF0CF, 0xF0F0, 0xF0F3, 0xF0FC, 0xF0FF,
  0xF300, 0xF303, 0xF30C, 0xF30F, 0xF330, 0xF333, 0xF33C, 0xF33F, 0xF3C0, 0xF3C3, 0xF3CC, 0xF3CF, 0xF3F0, 0xF3F3, 0xF3FC, 0xF3FF,
  0xFC00, 0xFC03, 0xFC0C, 0xFC0F, 0xFC30, 0xFC33, 0xFC3C, 0xFC3F, 0xFCC0, 0xFCC3, 0xFCCC, 0xFCCF, 0xFCF0, 0xFCF3, 0xFCFC, 0xFCFF,
  0xFF00, 0xFF03, 0xFF0C, 0xFF0F, 0xFF30, 0xFF33, 0xFF3C, 0xFF3F, 0xFFC0, 0xFFC3, 0xFFCC, 0xFFCF, 0xFFF0, 0xFFF3, 0xFFFC, 0xFFFF
};

GxDESTM32T::GxDESTM32T() : avt(tps), _pDiagnosticOutput(0)
{
  _width = 800;
  _height = 600;
  _vcom = 2000;
  _power_is_on = false;
  _hibernating = true;
}

void GxDESTM32T::init(GxEPD_HD::Panel panel, Stream* pDiagnosticOutput)
{
  _panel = panel;
  switch (panel)
  {
    case GxEPD_HD::GDE043A2:
      _width = 800;
      _height = 600;
      _vcom = 2000;
      break;
    case GxEPD_HD::GDE060BA:
      _width = 800;
      _height = 600;
      _vcom = 2000;
      break;
    case GxEPD_HD::GDEW080T5:
      _width = 1024;
      _height = 768;
      _vcom = 2200;
      break;
  }
  _pDiagnosticOutput = pDiagnosticOutput;
  avt.init(panel, pDiagnosticOutput);
  //if(pDiagnosticOutput) pDiagnosticOutput->println("GxDESTM32T::init()");
  avt.wf_mode = EPD_MODE_INIT;
  //#define PORT_B_OUT_PP        TPS_PWRCOM_PIN | TPS_PWRUP_PIN
  pinMode(TPS_PWRCOM_PP, OUTPUT);
  pinMode(TPS_PWRUP_PP, OUTPUT);
  //#define PORT_B_OUT_OD        TPS_SCL_PIN|TPS_SDA_PIN
  //  pinMode(TPS_SCL_PP, OUTPUT); // PB15
  //  pinMode(TPS_SDA_PP, OUTPUT); // PB14
  //  // need to change SCL & SDA to open drain
  //  GPIOB->CRH = (GPIOB->CRH & ~0xFF000000) | 0x77000000; // open drain 50MHz
  // suitable STM32 Arduino package is:
  // STM32 Boards (STM32Duino.com)
  // Board: "Generic STM32F103V series"
  pinMode(TPS_SCL_PP, OUTPUT_OPEN_DRAIN); // PB15
  pinMode(TPS_SDA_PP, OUTPUT_OPEN_DRAIN); // PB14
  //#define PORT_C_OUT_PP        SYS_WAKEUP_PIN
  pinMode(SYS_WAKEUP_PP, OUTPUT);
  //#define PORT_D_OUT_PP        AVT_HCS_PIN | AVT_HDC_PIN | AVT_HRD_PIN | AVT_RST_PIN | AVT_HWE_PIN | TPS_WAKEUP_PIN
  pinMode(AVT_HCS_PP, OUTPUT);
  pinMode(AVT_HDC_PP, OUTPUT);
  pinMode(AVT_HRD_PP, OUTPUT);
  pinMode(AVT_RST_PP, OUTPUT);
  pinMode(AVT_HWE_PP, OUTPUT);
  pinMode(TPS_WAKEUP_PP, OUTPUT);
  //#define PORT_D_IN_NOPULL    AVT_RDY_PIN | AVT_IRQ_PIN
  pinMode(AVT_RDY_PP, INPUT_PULLUP);
  pinMode(AVT_IRQ_PP, INPUT);
  // enable RCC clock on AVT_DAT_PORT
  pinMode(AVT_DAT_PP0, INPUT);
  SYS_WAKEUP_L;
  LED_01_OFF;
  LED_02_OFF;
  AVT_RST_L;
  AVT_HDC_L;
  AVT_HWE_H;
  AVT_HRD_H;
  AVT_HCS_H;
  AVT_DAT_SETIN;
  TPS_WAKEUP_L;
  TPS_PWRCOM_L;
  TPS_PWRUP_L;
  TPS_SCL_H;
  TPS_SDA_H;
  SYS_WAKEUP_H;
  _hibernating = false;
  delay(300);
  tps.tps_init(_vcom, pDiagnosticOutput);
  tps.tps_sleep_to_standby();
  _power_is_on = true;
  avt.AVT_CONFIG_check();      //���AVT�����������Ƿ���ȷ
  //if(pDiagnosticOutput) pDiagnosticOutput->println("AVT_CONFIG_check() done");
  delay(500);
  avt.avt_waveform_update();//���ز��α�
  avt.avt_init();
  avt.wf_mode = EPD_MODE_INIT;
  epd_draw_gray(0xff);
  avt.wf_mode = EPD_MODE_GC16;
  avt.avt_slp();
  //epd_draw_gray(0xff);//ȫ��ɫ
  //delay(2000);
  //if(pDiagnosticOutput) pDiagnosticOutput->println("GxDESTM32T::init() done");
}

void GxDESTM32T::clearScreen(uint8_t value)
{
  if (_hibernating) _wake_up();
  avt.wf_mode = EPD_MODE_GC16;
  epd_draw_gray(value);
}

void GxDESTM32T::writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  Debug_str("writeImage start...\r\n");
  if (_hibernating) _wake_up();
  if ((x >= _width) || (y >= _height)) return;
  if (x + w > _width) w = _width - x;
  if (y + h > _height) h = _height - y;
  uint32_t idx = 0;
  uint32_t n;
  uint16_t dw;
  switch (depth)
  {
    case 0: // corner test
      avt.avt_run_sys();
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      for (uint16_t y1 = 0; y1 < h; y1++)
      {
        for (uint16_t x1 = 0; x1 < w; x1 += 8)
        {
          uint16_t dw = 0xFFFF;
          if ((x1 < 32) && (y1 < 32)) dw = 0;
          if ((x1 >= w - 48) && (y1 < 48)) dw = 0;
          if ((x1 >= w - 64) && (y1 > h - 64)) dw = 0;
          if ((x1 < 80) && (y1 > h - 80)) dw = 0x0003;
          avt.avt_i80_write_dat(dw);
        }
      }
      break;
    case 1:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 8;
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb = idx < size ? bitmap[idx++] : 0xFF;
        dw = bw2grey16[sb];
        avt.avt_i80_write_dat(dw);
      }
      break;
    case 2:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 8;
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
        uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
        //dw = sb1 | (sb2 << 8);
        dw = sb2 | (sb1 << 8);
        avt.avt_i80_write_dat(dw);
      }
      break;
    case 4:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_4BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 4;
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
        uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
        dw = sb2 | (sb1 << 8);
        avt.avt_i80_write_dat(dw);
      }
      break;
    case 8:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_8BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 2;
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
        uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
        dw = sb2 | (sb1 << 8);
        avt.avt_i80_write_dat(dw);
      }
      break;
    default:
      return;
  }
  avt.avt_ld_img_end();
  Debug_str("writeImage end...\r\n");
}

void GxDESTM32T::writeImage(uint8_t (*get)(uint32_t), uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  Debug_str("writeImage start...\r\n");
  if (_hibernating) _wake_up();
  if ((x >= _width) || (y >= _height)) return;
  if (x + w > _width) w = _width - x;
  if (y + h > _height) h = _height - y;
  uint32_t idx = 0;
  uint32_t n;
  uint16_t dw;
  switch (depth)
  {
    case 0: // corner test
      avt.avt_run_sys();
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      for (uint16_t y1 = 0; y1 < h; y1++)
      {
        for (uint16_t x1 = 0; x1 < w; x1 += 8)
        {
          uint16_t dw = 0xFFFF;
          if ((x1 < 32) && (y1 < 32)) dw = 0;
          if ((x1 >= w - 48) && (y1 < 48)) dw = 0;
          if ((x1 >= w - 64) && (y1 > h - 64)) dw = 0;
          if ((x1 < 80) && (y1 > h - 80)) dw = 0x0003;
          avt.avt_i80_write_dat(dw);
        }
      }
      break;
    case 1:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 8;
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb = idx < size ? get(idx++) : 0xFF;
        dw = bw2grey16[sb];
        avt.avt_i80_write_dat(dw);
      }
      break;
    case 2:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 8;
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb1 = idx < size ? get(idx++) : 0xFF;
        uint8_t sb2 = idx < size ? get(idx++) : 0xFF;
        //dw = sb1 | (sb2 << 8);
        dw = sb2 | (sb1 << 8);
        avt.avt_i80_write_dat(dw);
      }
      break;
    case 4:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_4BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 4;
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb1 = idx < size ? get(idx++) : 0xFF;
        uint8_t sb2 = idx < size ? get(idx++) : 0xFF;
        dw = sb2 | (sb1 << 8);
        avt.avt_i80_write_dat(dw);
      }
      break;
    case 8:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_8BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 2;
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb1 = idx < size ? get(idx++) : 0xFF;
        uint8_t sb2 = idx < size ? get(idx++) : 0xFF;
        dw = sb2 | (sb1 << 8);
        avt.avt_i80_write_dat(dw);
      }
      break;
    default:
      return;
  }
  avt.avt_ld_img_end();
  Debug_str("writeImage end...\r\n");
}

void GxDESTM32T::writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                                uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx, uint16_t dy)
{
  Debug_str("writeImagePart start...\r\n");
  if (_hibernating) _wake_up();
  switch (depth)
  {
    case 1:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x + dx, y + dy, w, h);
      avt.avt_wr_reg_addr(0x0154);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 8) // pixels per loop
        {
          uint32_t idx = y1 * (width / 8) + x1 / 8; // pixels per byte
          uint8_t sb = idx < size ? bitmap[idx] : 0xFF;
          uint16_t dw = bw2grey16[sb];
          avt.avt_i80_write_dat(dw);
        }
      }
      break;
    case 2:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 8) // pixels per loop
        {
          uint32_t idx = y1 * (width / 4) + x1 / 4; // pixels per byte
          uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
          uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
          uint16_t dw = sb2 | (sb1 << 8);
          avt.avt_i80_write_dat(dw);
        }
      }
      break;
    case 4:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_4BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 4) // pixels per loop
        {
          uint32_t idx = y1 * (width / 2) + x1 / 2; // pixels per byte
          uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
          uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
          uint16_t dw = sb2 | (sb1 << 8);
          avt.avt_i80_write_dat(dw);
        }
      }
      break;
    case 8:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_8BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 2) // pixels per loop
        {
          uint32_t idx = y1 * (width / 1) + x1 / 1; // pixels per byte
          uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
          uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
          uint16_t dw = sb2 | (sb1 << 8);
          avt.avt_i80_write_dat(dw);
        }
      }
      break;
    default:
      return;
  }
  avt.avt_ld_img_end();
  Debug_str("writeImagePart end...\r\n");
}

void GxDESTM32T::writeImagePart(uint8_t (*get)(uint32_t), uint32_t size, uint8_t depth, uint32_t width,
                                uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx, uint16_t dy)
{
  Debug_str("writeImagePart start...\r\n");
  if (_hibernating) _wake_up();
  switch (depth)
  {
    case 1:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x + dx, y + dy, w, h);
      avt.avt_wr_reg_addr(0x0154);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 8) // pixels per loop
        {
          uint32_t idx = y1 * (width / 8) + x1 / 8; // pixels per byte
          uint8_t sb = idx < size ? get(idx) : 0xFF;
          uint16_t dw = bw2grey16[sb];
          avt.avt_i80_write_dat(dw);
        }
      }
      break;
    case 2:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 8) // pixels per loop
        {
          uint32_t idx = y1 * (width / 4) + x1 / 4; // pixels per byte
          uint8_t sb1 = idx < size ? get(idx++) : 0xFF;
          uint8_t sb2 = idx < size ? get(idx++) : 0xFF;
          uint16_t dw = sb2 | (sb1 << 8);
          avt.avt_i80_write_dat(dw);
        }
      }
      break;
    case 4:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_4BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 4) // pixels per loop
        {
          uint32_t idx = y1 * (width / 2) + x1 / 2; // pixels per byte
          uint8_t sb1 = idx < size ? get(idx++) : 0xFF;
          uint8_t sb2 = idx < size ? get(idx++) : 0xFF;
          uint16_t dw = sb2 | (sb1 << 8);
          avt.avt_i80_write_dat(dw);
        }
      }
      break;
    case 8:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_8BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 2) // pixels per loop
        {
          uint32_t idx = y1 * (width / 1) + x1 / 1; // pixels per byte
          uint8_t sb1 = idx < size ? get(idx++) : 0xFF;
          uint8_t sb2 = idx < size ? get(idx++) : 0xFF;
          uint16_t dw = sb2 | (sb1 << 8);
          avt.avt_i80_write_dat(dw);
        }
      }
      break;
    default:
      return;
  }
  avt.avt_ld_img_end();
  Debug_str("writeImagePart end...\r\n");
}

void GxDESTM32T::drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  Debug_str("drawImage start...\r\n");
  writeImage(bitmap, size, depth, x, y, w, h);
  refresh(x, y, w, h, (depth == 1));
  Debug_str("drawImage end...\r\n");
}

void GxDESTM32T::drawImage(uint8_t (*get)(uint32_t), uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  Debug_str("drawImage start...\r\n");
  writeImage(get, size, depth, x, y, w, h);
  refresh(x, y, w, h, (depth == 1));
  Debug_str("drawImage end...\r\n");
}

void GxDESTM32T::drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                               uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx, uint16_t dy)
{
  Debug_str("drawImagePart start...\r\n");
  writeImagePart(bitmap, size, depth, width, x, y, w, h, dx, dy);
  refresh(x + dx, y + dy, w, h, (depth == 1));
  Debug_str("drawImagePart end...\r\n");
}

void GxDESTM32T::drawImagePart(uint8_t (*get)(uint32_t), uint32_t size, uint8_t depth, uint32_t width,
                               uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx, uint16_t dy)
{
  Debug_str("drawImagePart start...\r\n");
  writeImagePart(get, size, depth, width, x, y, w, h, dx, dy);
  refresh(x + dx, y + dy, w, h, (depth == 1));
  Debug_str("drawImagePart end...\r\n");
}

void GxDESTM32T::refresh(bool partial_update_mode)
{
  refresh(0, 0, _width, _height, partial_update_mode);
}

void GxDESTM32T::refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode)
{
  if (_hibernating) _wake_up();
  uint8_t wf_mode = partial_update_mode ? EPD_MODE_DU : EPD_MODE_GC16;
  avt.avt_upd_full_area((wf_mode << 8), x, y, w, h);
  if (!_power_is_on) tps.tps_sleep_to_standby();
  _power_is_on = true;
  tps.tps_vcom_enable();
  avt.avt_wait_dspe_trg();
  avt.avt_wait_dspe_frend();
  tps.tps_vcom_disable();
  avt.avt_slp();
}

void GxDESTM32T::updateWindow(const uint8_t* bitmap, uint32_t size, uint32_t width, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  Debug_str("updateWindow start...\r\n");
  if (_hibernating) _wake_up();
  avt.avt_run_sys();
  avt.avt_wr_reg(0x0020, 0x2);  //big endian
  avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
  avt.avt_wr_reg_addr(0x0154);
  for (uint32_t y1 = y; y1 < y + h; y1++)
  {
    for (uint32_t x1 = x; x1 < x + w; x1 += 8)
    {
      uint32_t idx = y1 * (width / 8) + x1 / 8;
      uint8_t sb = idx < size ? bitmap[idx++] : 0xFF;
      uint16_t dw = bw2grey16[sb];
      avt.avt_i80_write_dat(dw);
    }
  }
  avt.avt_ld_img_end();
  //avt.avt_upd_full_area((avt.wf_mode << 8), x, y, w, h);
  avt.avt_upd_full_area((EPD_MODE_DU << 8), x, y, w, h);
  if (!_power_is_on) tps.tps_sleep_to_standby();
  _power_is_on = true;
  tps.tps_vcom_enable();
  avt.avt_wait_dspe_trg();
  avt.avt_wait_dspe_frend();
  tps.tps_vcom_disable();
  avt.avt_slp();
  Debug_str("updateWindow end...\r\n");
}

void GxDESTM32T::writeFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth, uint8_t value)
{
  //Debug_str("writeFilledRect start...\r\n");
  if (_hibernating) _wake_up();
  if ((x >= _width) || (y >= _height)) return;
  if (x + w > _width) w = _width - x;
  if (y + h > _height) h = _height - y;
  uint32_t n;
  uint16_t dw = value | (value << 8);
  switch (depth)
  {
    case 1:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 8;
      if (n < 1) n = 1;
      for (uint32_t i = 0; i < n; i++)
      {
        dw = bw2grey16[value];
        avt.avt_i80_write_dat(dw);
      }
      break;
    case 2:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 8;
      if (n < 1) n = 1;
      for (uint32_t i = 0; i < n; i++)
      {
        avt.avt_i80_write_dat(dw);
      }
      break;
    case 4:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_4BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 4;
      if (n < 1) n = 1;
      for (uint32_t i = 0; i < n; i++)
      {
        avt.avt_i80_write_dat(dw);
      }
      break;
    case 8:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_8BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 2;
      if (n < 1) n = 1;
      for (uint32_t i = 0; i < n; i++)
      {
        avt.avt_i80_write_dat(dw);
      }
      break;
    default:
      return;
  }
  avt.avt_ld_img_end();
  //Debug_str("writeFilledRect end...\r\n");
}

void GxDESTM32T::drawFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth, uint8_t value)
{
  //Debug_str("drawFilledRect start...\r\n");
  writeFilledRect(x, y, w, h, depth, value);
  refresh(x, y, w, h, (depth == 1));
  //Debug_str("drawFilledRect end...\r\n");
}

void GxDESTM32T::powerOff()
{
  tps.tps_vcom_disable();
  avt.avt_slp();
  tps.tps_standby_to_sleep();
  _power_is_on = false;
}

void GxDESTM32T::hibernate()
{
  //SerialDiag.println("GxDESP32T::hibernate()");
  powerOff();
  SYS_WAKEUP_L;
  _hibernating = true;
}

void GxDESTM32T::_wake_up()
{
  SYS_WAKEUP_H;
  _hibernating = false;
  delay(300);
  tps.tps_init(_vcom, _pDiagnosticOutput);
  tps.tps_sleep_to_standby();
  _power_is_on = true;
  //avt.AVT_CONFIG_check();
  //delay(500);
  //avt.avt_waveform_update();
  avt.avt_init();
  avt.wf_mode = EPD_MODE_INIT;
  epd_draw_gray(0xff);
  avt.wf_mode = EPD_MODE_GC16;
  avt.avt_slp();
}

void GxDESTM32T::epd_draw_gray(uint8_t gray)
{
  uint16_t i, j, dat;

  Debug_str("epd_draw_gray start...\r\n");
  avt.avt_run_sys();
  avt.avt_ld_img(EPD_DATA_8BPP);
  avt.avt_wr_reg_addr(0x0154);

  for (i = 0; i < _height; i++)
  {
    for (j = 0; j < (_width / 2); j++)
    {
      dat = gray;
      dat = ((dat << 8) & 0xff00) + gray;
      avt.avt_i80_write_dat(dat);
    }
  }

  avt.avt_ld_img_end();
  avt.avt_upd_full((avt.wf_mode << 8));

  if (!_power_is_on) tps.tps_sleep_to_standby();
  _power_is_on = true;
  tps.tps_vcom_enable();
  avt.avt_wait_dspe_trg();
  avt.avt_wait_dspe_frend();
  tps.tps_vcom_disable();

  avt.avt_slp();
  Debug_str("epd_draw_gray end...\r\n");
}

void GxDESTM32T::Debug_str(const char *s)
{
  if (_pDiagnosticOutput) _pDiagnosticOutput->print(s);
}

#endif

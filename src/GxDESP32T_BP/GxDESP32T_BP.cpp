// Class GxDESP32T_BP : display IO class for GDE060BA on my DESP32T_BP (proto board) for TCon-11 parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html

#if defined(ARDUINO_ARCH_ESP32)

#include "GxDESP32T_BP.h"
#include "DESP32T_BP_wiring.h"

#define SerialDiag if (_pDiagnosticOutput) (*_pDiagnosticOutput)

const uint8_t GxDESP32T_BP::bw2grey8[] =
{
  0b00000000, 0b00000011, 0b00001100, 0b00001111,
  0b00110000, 0b00110011, 0b00111100, 0b00111111,
  0b11000000, 0b11000011, 0b11001100, 0b11001111,
  0b11110000, 0b11110011, 0b11111100, 0b11111111,
};

const uint16_t GxDESP32T_BP::bw2grey16[] =
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

GxDESP32T_BP::GxDESP32T_BP() : tps(IO), avt(tps, IO), _pDiagnosticOutput(0)
{
  _width = 800;
  _height = 600;
  _vcom = 2000;
  _power_is_on = false;
  _hibernating = true;
}

void GxDESP32T_BP::init(GxEPD_HD::Panel panel, Stream* pDiagnosticOutput)
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
  IO.init();
  avt.init(panel, pDiagnosticOutput);
  //if(pDiagnosticOutput) pDiagnosticOutput->println("GxDESP32T_BP::init()");
  avt.wf_mode = EPD_MODE_INIT;
  SYS_WAKEUP_L;
  pinMode(SYS_WAKEUP_PIN, OUTPUT);
  TPS_WAKEUP_L;
  TPS_PWRCOM_L;
  TPS_PWRUP_L;
  AVT_RST_L;
  SYS_WAKEUP_H;
  _hibernating = false;
  delay(300);
  tps.tps_init(_vcom, pDiagnosticOutput);
  tps.tps_sleep_to_standby();
  _power_is_on = true;
  avt.AVT_CONFIG_check();
  //if(pDiagnosticOutput) pDiagnosticOutput->println("AVT_CONFIG_check() done");
  delay(500);
  avt.avt_waveform_update();
  avt.avt_init();
  avt.wf_mode = EPD_MODE_INIT;
  epd_draw_gray(0xff);
  avt.wf_mode = EPD_MODE_GC16;
  avt.avt_slp();
  //if(pDiagnosticOutput) pDiagnosticOutput->println("GxDESP32T_BP::init() done");
}

void GxDESP32T_BP::clearScreen(uint8_t value)
{
  if (_hibernating) _wake_up();
  avt.wf_mode = EPD_MODE_GC16;
  epd_draw_gray(value);
}

void GxDESP32T_BP::writeImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  Debug_str("writeImage start...\r\n");
  if (_hibernating) _wake_up();
  uint32_t start = micros();
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
      n = uint32_t(w) * uint32_t(h) / 8;
      avt.startTransfer(n);
      for (uint16_t y1 = 0; y1 < h; y1++)
      {
        for (uint16_t x1 = 0; x1 < w; x1 += 8)
        {
          uint16_t dw = 0xFFFF;
          if ((x1 < 32) && (y1 < 32)) dw = 0;
          if ((x1 >= w - 48) && (y1 < 48)) dw = 0;
          if ((x1 >= w - 64) && (y1 > h - 64)) dw = 0;
          if ((x1 < 80) && (y1 > h - 80)) dw = 0x0003;
          avt.transfer16(dw);
        }
      }
      avt.endTransfer();
      break;
    case 1:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 8;
      avt.startTransfer(n);
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb = idx < size ? bitmap[idx++] : 0xFF;
        dw = bw2grey16[sb];
        avt.transfer16(dw);
      }
      avt.endTransfer();
      break;
    case 2:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 8;
      avt.startTransfer(n);
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
        uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
        //dw = sb1 | (sb2 << 8);
        dw = sb2 | (sb1 << 8);
        avt.transfer16(dw);
      }
      avt.endTransfer();
      break;
    case 4:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_4BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 4;
      avt.startTransfer(n);
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
        uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
        dw = sb2 | (sb1 << 8);
        avt.transfer16(dw);
      }
      avt.endTransfer();
      break;
    case 8:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_8BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 2;
      avt.startTransfer(n);
      for (uint32_t i = 0; i < n; i++)
      {
        uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
        uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
        dw = sb2 | (sb1 << 8);
        avt.transfer16(dw);
      }
      avt.endTransfer();
      break;
  }
  uint32_t trf = micros() - start;
  avt.avt_ld_img_end();
  Debug_str("writeImage end...\r\n");
  uint32_t total = micros() - start;
  //SerialDiag.print("trf : "); SerialDiag.print(trf); SerialDiag.print(" total : "); SerialDiag.println(total);
  (void) trf; (void) total; // (void) warning unused
}

void GxDESP32T_BP::writeImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                                  uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx, uint16_t dy)
{
  Debug_str("writeImagePart start...\r\n");
  if (_hibernating) _wake_up();
  uint32_t n;
  switch (depth)
  {
    case 1:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x + dx, y + dy, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 8;
      avt.startTransfer(n);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 8) // pixels per loop
        {
          uint32_t idx = y1 * (width / 8) + x1 / 8; // pixels per byte
          uint8_t sb = idx < size ? bitmap[idx] : 0xFF;
          uint16_t dw = bw2grey16[sb];
          avt.transfer16(dw);
        }
      }
      avt.endTransfer();
      break;
    case 2:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 8;
      avt.startTransfer(n);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 8) // pixels per loop
        {
          uint32_t idx = y1 * (width / 4) + x1 / 4; // pixels per byte
          uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
          uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
          uint16_t dw = sb2 | (sb1 << 8);
          avt.transfer16(dw);
        }
      }
      avt.endTransfer();
      break;
    case 4:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_4BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 4;
      avt.startTransfer(n);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 4) // pixels per loop
        {
          uint32_t idx = y1 * (width / 2) + x1 / 2; // pixels per byte
          uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
          uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
          uint16_t dw = sb2 | (sb1 << 8);
          avt.transfer16(dw);
        }
      }
      avt.endTransfer();
      break;
    case 8:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_8BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 2;
      avt.startTransfer(n);
      for (uint32_t y1 = y; y1 < y + h; y1++)
      {
        for (uint32_t x1 = x; x1 < x + w; x1 += 2) // pixels per loop
        {
          uint32_t idx = y1 * (width / 1) + x1 / 1; // pixels per byte
          uint8_t sb1 = idx < size ? bitmap[idx++] : 0xFF;
          uint8_t sb2 = idx < size ? bitmap[idx++] : 0xFF;
          uint16_t dw = sb2 | (sb1 << 8);
          avt.transfer16(dw);
        }
      }
      avt.endTransfer();
      break;
  }
  avt.avt_ld_img_end();
  Debug_str("writeImagePart end...\r\n");
}

void GxDESP32T_BP::drawImage(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  Debug_str("drawImage start...\r\n");
  writeImage(bitmap, size, depth, x, y, w, h);
  refresh(x, y, w, h, (depth == 1));
  Debug_str("drawImage end...\r\n");
}

void GxDESP32T_BP::drawImagePart(const uint8_t* bitmap, uint32_t size, uint8_t depth, uint32_t width,
                                 uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t dx, uint16_t dy)
{
  Debug_str("drawImagePart start...\r\n");
  writeImagePart(bitmap, size, depth, width, x, y, w, h, dx, dy);
  refresh(x + dx, y + dy, w, h, (depth == 1));
  Debug_str("drawImagePart end...\r\n");
}

void GxDESP32T_BP::refresh(bool partial_update_mode)
{
  refresh(0, 0, _width, _height, partial_update_mode);
}

void GxDESP32T_BP::refresh(int16_t x, int16_t y, int16_t w, int16_t h, bool partial_update_mode)
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

void GxDESP32T_BP::updateWindow(const uint8_t* bitmap, uint32_t size, uint32_t width, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  Debug_str("updateWindow start...\r\n");
  if (_hibernating) _wake_up();
  avt.avt_run_sys();
  avt.avt_wr_reg(0x0020, 0x2);  //big endian
  avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
  avt.avt_wr_reg_addr(0x0154);
  uint32_t n = uint32_t(w) * uint32_t(h) / 8;
  avt.startTransfer(n);
  for (uint32_t y1 = y; y1 < y + h; y1++)
  {
    for (uint32_t x1 = x; x1 < x + w; x1 += 8)
    {
      uint32_t idx = y1 * (width / 8) + x1 / 8;
      uint8_t sb = idx < size ? bitmap[idx++] : 0xFF;
      uint16_t dw = bw2grey16[sb];
      avt.transfer16(dw);
    }
  }
  avt.endTransfer();
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

void GxDESP32T_BP::writeFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth, uint8_t value)
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
      avt.startTransfer(n);
      for (uint32_t i = 0; i < n; i++)
      {
        dw = bw2grey16[value];
        avt.transfer16(dw);
      }
      avt.endTransfer();
      break;
    case 2:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_2BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 8;
      if (n < 1) n = 1;
      avt.startTransfer(n);
      for (uint32_t i = 0; i < n; i++)
      {
        avt.transfer16(dw);
      }
      avt.endTransfer();
      break;
    case 4:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_4BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 4;
      if (n < 1) n = 1;
      avt.startTransfer(n);
      for (uint32_t i = 0; i < n; i++)
      {
        avt.transfer16(dw);
      }
      avt.endTransfer();
      break;
    case 8:
      avt.avt_run_sys();
      avt.avt_wr_reg(0x0020, 0x2);  //big endian
      avt.avt_ld_img_area(EPD_DATA_8BPP, x, y, w, h);
      avt.avt_wr_reg_addr(0x0154);
      n = uint32_t(w) * uint32_t(h) / 2;
      if (n < 1) n = 1;
      avt.startTransfer(n);
      for (uint32_t i = 0; i < n; i++)
      {
        avt.transfer16(dw);
      }
      avt.endTransfer();
      break;
  }
  avt.avt_ld_img_end();
  //Debug_str("writeFilledRect end...\r\n");
}

void GxDESP32T_BP::drawFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t depth, uint8_t value)
{
  //Debug_str("drawFilledRect start...\r\n");
  writeFilledRect(x, y, w, h, depth, value);
  refresh(x, y, w, h, (depth == 1));
  //Debug_str("drawFilledRect end...\r\n");
}

void GxDESP32T_BP::powerOff()
{
  tps.tps_vcom_disable();
  avt.avt_slp();
  tps.tps_standby_to_sleep();
  _power_is_on = false;
}

void GxDESP32T_BP::hibernate()
{
  //SerialDiag.println("GxDESP32T_BP::hibernate()");
  powerOff();
  tps.tps_end();
  IO.end();
  SYS_WAKEUP_L;
  _hibernating = true;
}

void GxDESP32T_BP::_wake_up()
{
  SYS_WAKEUP_H;
  _hibernating = false;
  IO.init();
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

void GxDESP32T_BP::epd_draw_gray(uint8_t gray)
{
  uint16_t i, j, dat;

  uint32_t start = micros();

  Debug_str("epd_draw_gray start...\r\n");
  avt.avt_run_sys();
  avt.avt_ld_img(EPD_DATA_8BPP);
  avt.avt_wr_reg_addr(0x0154);

  uint32_t n = uint32_t(_width) * uint32_t(_height) / 2;
  avt.startTransfer(n);
  for (i = 0; i < _height; i++)
  {
    for (j = 0; j < (_width / 2); j++)
    {
      dat = gray;
      dat = ((dat << 8) & 0xff00) + gray;
      avt.transfer16(dat);
    }
  }
  avt.endTransfer();

  uint32_t trf = micros() - start;

  avt.avt_ld_img_end();
  avt.avt_upd_full((avt.wf_mode << 8));

  tps.tps_vcom_enable();
  avt.avt_wait_dspe_trg();
  avt.avt_wait_dspe_frend();
  tps.tps_vcom_disable();

  avt.avt_slp();
  Debug_str("epd_draw_gray end...\r\n");
  uint32_t total = micros() - start;
  SerialDiag.print("trf : "); SerialDiag.print(trf); SerialDiag.print(" total : "); SerialDiag.println(total);
}

void GxDESP32T_BP::Debug_str(const char *s)
{
  if (_pDiagnosticOutput) _pDiagnosticOutput->print(s);
}

#endif

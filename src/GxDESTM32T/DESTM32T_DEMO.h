// Class DESTM32T_DEMO : DEMO class for GDE060BA on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
#ifndef _DESTM32T_DEMO_H_
#define _DESTM32T_DEMO_H_

#include "GxDESTM32T.h"

class DESTM32T_DEMO
{
  public:
    DESTM32T_DEMO(GxDESTM32T& io) : IO(io) {};
    void demo800x600();
    void demo1024x768();
  private:
    void epd_draw_pic_start(void);
    void epd_draw_pic_buff(uint8_t* buff, uint16_t len);
    void epd_draw_pic_end(void);
    void epd_draw_pic_from_spiflash(uint32_t addr);

    void epd_draw_gray_level_horizontal(uint8_t div);
    void epd_draw_gray_level_vertical(uint8_t div);
    void epd_draw_gray_part(uint8_t gray, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void epd_draw_gray_part_lut(uint8_t gray, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void epd_draw_pic_part_from_rom(uint8_t* ptr, uint8_t bpp, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void avt_lut_demo(uint8_t gray);
    uint8_t SpiFlash_ReadWriteByte(uint8_t TxData);
    void SpiFlash_EraseBlock32K(uint32_t addr);
    void SpiFlash_PageProgram(uint32_t addr, uint8_t *data);
    uint16_t SpiFlash_ReadID(void);
    void SpiFlash_StatusWait(void);
    void SpiFlash_ReadData(uint32_t addr, uint8_t *data);
    void Debug_str(const char *s);
  private:
    GxDESTM32T & IO;
};

#endif

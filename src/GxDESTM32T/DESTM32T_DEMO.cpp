// Class DESTM32T_DEMO : DEMO class for GDE060BA on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
#if (defined(ARDUINO_ARCH_STM32F1) && defined(ARDUINO_GENERIC_STM32F103V)) || (defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_GENERIC_F103VE))

#include "DESTM32T_DEMO.h"
#include "DESTM32L1_board.h"
#include "picture800x600.h"
#include "picture1024x768.h"
#include <SPI.h>

#define FLASH_CS_PORT     GPIOA     //OUT - PP
#define FLASH_CS_PIN      GPIO_PIN_4
#define FLASH_CS_PP       PA4

void DESTM32T_DEMO::demo800x600()
{
#if 1    //16���Ҷ���ʾ
  delay(2000);

  IO.avt.wf_mode = EPD_MODE_GC16;
  epd_draw_gray_level_horizontal(16);
  delay(2000);

  epd_draw_gray_level_vertical(16);
  delay(2000);

  epd_draw_gray_level_horizontal(8);
  delay(2000);

  epd_draw_gray_level_vertical(8);
  delay(2000);

  IO.epd_draw_gray(0xff);
#endif

#if 1
  //������ʾ��˾LOGO
  IO.avt.wf_mode = EPD_MODE_DU;
  IO.avt.avt_run_sys();   //�ֲ�ˢ��ͼƬ
  avt_lut_demo(0); //LOGO������ʾ
  avt_lut_demo(255);//LOGO������ʧ
  IO.avt.avt_slp();
  delay(200);//2
  //ȫ��
#endif
  IO.avt.wf_mode = EPD_MODE_GC16;
  IO.epd_draw_gray(0xff);  //ȫ��
  delay(100);  //1

  epd_draw_pic_part_from_rom((uint8_t*)gImage_01, EPD_DATA_2BPP, 0, 0, 800, 133); //�ֲ�ͼ1,�ֱ���800*133
  delay(1000);//10
  epd_draw_pic_part_from_rom((uint8_t*)gImage800x600, EPD_DATA_2BPP, 0, 0, 800, 600);//ȫ��ͼ1���ֱ���800*600
  delay(3000);//��ʱ3s

  for (int i = 0; i < 2; i++)
  {
    ////////////////�ֲ���ʾͼ/////////////////
    IO.avt.wf_mode = EPD_MODE_GC16;
    epd_draw_pic_part_from_rom((uint8_t*)gImage_01, EPD_DATA_2BPP, 0, 0, 800, 133); //�ֲ�ͼ1,�ֱ���800*133
    delay(1000);//10
    epd_draw_pic_part_from_rom((uint8_t*)gImage_02, EPD_DATA_2BPP, 0, 133, 800, 105);//�ֲ�ͼ2,�ֱ���800*105
    delay(1000);//10
    epd_draw_pic_part_from_rom((uint8_t*)gImage_03, EPD_DATA_2BPP, 0, 238, 800, 175);//�ֲ�ͼ3,�ֱ���800*175
    delay(1000);//10
    epd_draw_pic_part_from_rom((uint8_t*)gImage_04, EPD_DATA_2BPP, 0, 413, 800, 187);//�ֲ�ͼ4,�ֱ���800*187
    delay(1000);//10
    //ȫ��
    IO.avt.wf_mode = EPD_MODE_GC16;
    IO.epd_draw_gray(0xff);  //ȫ��
    delay(100);  //1
    ////////////////ȫ����ʾͼ/////////////////
    epd_draw_pic_part_from_rom((uint8_t*)gImage800x600, EPD_DATA_2BPP, 0, 0, 800, 600);//ȫ��ͼ1���ֱ���800*600
    delay(3000);//��ʱ3s
    epd_draw_pic_part_from_rom((uint8_t*)gImage_2, EPD_DATA_2BPP, 0, 0, 800, 600);//ȫ��ͼ2���ֱ���800*600
    delay(3000);//��ʱ3s
    //ȫ��
    IO.avt.wf_mode = EPD_MODE_GC16;
    IO.epd_draw_gray(0xff);  //ȫ��
    delay(100);  //1
  }
}

void DESTM32T_DEMO::demo1024x768()
{
#if 1    //16���Ҷ���ʾ
  delay(2000); //2

  IO.avt.wf_mode = EPD_MODE_GC16;
  epd_draw_gray_level_horizontal(16);
  delay(2000); //2

  epd_draw_gray_level_vertical(16);
  delay(2000); //2

  epd_draw_gray_level_horizontal(8);
  delay(2000); //2

  epd_draw_gray_level_vertical(8);
  delay(2000); //2

  IO.epd_draw_gray(0xff);

#endif
  //ȫ��
  IO.avt.wf_mode = EPD_MODE_GC16;
  IO.epd_draw_gray(0xff);  //ȫ��
  delay(1000);  //1

  for (int i = 0; i < 2; i++)
  {
    IO.avt.wf_mode = EPD_MODE_GC16;
    //5~1 �ֲ���ʾ
    epd_draw_pic_part_from_rom((uint8_t*)gImage_num1, EPD_DATA_2BPP, 100 , 300, 48, 104); //�ֲ�ͼ1,�ֱ���48*104
    epd_draw_pic_part_from_rom((uint8_t*)gImage_num2, EPD_DATA_2BPP, 300 , 300, 48, 104); //�ֲ�ͼ1,�ֱ���48*104
    epd_draw_pic_part_from_rom((uint8_t*)gImage_num3, EPD_DATA_2BPP, 500 , 300, 48, 104); //�ֲ�ͼ1,�ֱ���48*104
    epd_draw_pic_part_from_rom((uint8_t*)gImage_num4, EPD_DATA_2BPP, 700 , 300, 48, 104); //�ֲ�ͼ1,�ֱ���48*104
    epd_draw_pic_part_from_rom((uint8_t*)gImage_num5, EPD_DATA_2BPP, 900 , 300, 48, 104); //�ֲ�ͼ1,�ֱ���48*104
    //ȫ��
    IO.avt.wf_mode = EPD_MODE_GC16;
    IO.epd_draw_gray(0xff);  //ȫ��
    delay(1000);  //1
    //4��ȫ����ʾ
    epd_draw_pic_part_from_rom((uint8_t*)gImage1024x768, EPD_DATA_2BPP, 0 , 0, 1024, 758); //�ֲ�ͼ1,�ֱ���1024*758
  }
  delay(1000);  //1
  IO.avt.wf_mode = EPD_MODE_GC16;
  IO.epd_draw_gray(0xff);  //ȫ��
  delay(100);  //1
}

void DESTM32T_DEMO::epd_draw_pic_start(void)
{
  IO.avt.avt_run_sys();
  IO.avt.avt_ld_img(EPD_DATA_8BPP);
  IO.avt.avt_wr_reg_addr(0x0154);
  Debug_str("epd_draw_pic_start start...\r\n");
}

void DESTM32T_DEMO::epd_draw_pic_buff(uint8_t* buff, uint16_t len)
{
  uint16_t i, dat;

  for (i = 0; i < len; i++)
  {
    dat = buff[i] & 0xf0;
    dat = (dat << 8) & 0xff00;
    dat = dat + ((buff[i] << 4) & 0xf0);
    IO.avt.avt_i80_write_dat(dat);
  }
}

void DESTM32T_DEMO::epd_draw_pic_end(void)
{
  IO.avt.avt_ld_img_end();
  IO.avt.avt_upd_full((IO.avt.wf_mode << 8));

  IO.tps.tps_vcom_enable();
  IO.avt.avt_wait_dspe_trg();
  IO.avt.avt_wait_dspe_frend();
  IO.tps.tps_vcom_disable();

  IO.avt.avt_slp();
  Debug_str("epd_draw_pic_start end...\r\n");
}

void DESTM32T_DEMO::epd_draw_pic_from_spiflash(uint32_t addr)
{
  uint8_t addr3, addr2, addr1, value;
  uint16_t i, j, dat;

  Debug_str("epd_draw_pic_from_spiflash start...\r\n");
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4); // 18MHz
  pinMode(FLASH_CS_PP, OUTPUT);
  addr1 = addr;
  addr2 = (addr >> 8);
  addr3 = (addr >> 16);

  FLASH_CS_L;
  SpiFlash_ReadWriteByte(0x03);
  SpiFlash_ReadWriteByte(addr3);
  SpiFlash_ReadWriteByte(addr2);
  SpiFlash_ReadWriteByte(addr1);

#if 1  //EPD_DATA_8BPP
  IO.avt.avt_run_sys();
  IO.avt.avt_ld_img(EPD_DATA_8BPP);
  IO.avt.avt_wr_reg_addr(0x0154);

  for (i = 0; i < IO._height; i++)
  {
    for (j = 0; j < IO._width / 2; j++)
    {
      value = SpiFlash_ReadWriteByte(0xff);
      dat = value & 0x0f;
      dat = (dat << 12) & 0xf000;
      dat = dat + (value & 0xf0);
      IO.avt.avt_i80_write_dat(dat);
    }
  }
#else //EPD_DATA_4BPP
  IO.avt.avt_run_sys();
  IO.avt.avt_wr_reg(0x0020, 0x2);  //big endian
  IO.avt.avt_ld_img(EPD_DATA_4BPP);
  IO.avt.avt_wr_reg_addr(0x0154);

  for (i = 0; i < IO._height; i++)
  {
    for (j = 0; j < IO._width / 4; j++)
    {
      value = SpiFlash_ReadWriteByte(0xff);
      dat = value;
      dat = (dat << 8) & 0xff00;
      value = SpiFlash_ReadWriteByte(0xff);
      //dat = dat + (((uint16_t)value<<8)&0xff00);
      dat = dat + value;
      IO.avt.avt_i80_write_dat(dat);
    }
  }
  IO.avt.avt_wr_reg(0x0020, 0x0); //little endian
#endif
  FLASH_CS_H;

  IO.avt.avt_ld_img_end();
  IO.avt.avt_upd_full((IO.avt.wf_mode << 8));

  IO.tps.tps_vcom_enable();
  IO.avt.avt_wait_dspe_trg();
  IO.avt.avt_wait_dspe_frend();
  IO.tps.tps_vcom_disable();

  IO.avt.avt_slp();
  SPI.end(); // release SPI pins
  pinMode(FLASH_CS_PP, INPUT); // CS also!
  Debug_str("epd_draw_pic_from_spiflash end...\r\n");
}

void DESTM32T_DEMO::epd_draw_gray_level_horizontal(uint8_t div)
{
  uint16_t h, i, j, dat;
  uint8_t v, gray;

  v = IO._height % div;
  Debug_str("epd_draw_gray_level_horizontal start...\r\n");
  IO.avt.avt_run_sys();
  IO.avt.avt_ld_img(EPD_DATA_8BPP);
  IO.avt.avt_wr_reg_addr(0x0154);

  gray = 0;
  for (i = 0; i < div; i++)
  {
    for (j = 0; j < (IO._height - v) / div; j++)
    {
      for (h = 0; h < (IO._width / 2); h++)
      {
        dat = gray;
        dat = ((dat << 8) & 0xff00) + gray;
        IO.avt.avt_i80_write_dat(dat);
      }
    }
    gray += 255 / (div - 1);
  }
  for (i = 0; i < v; i++)
  {
    for (h = 0; h < (IO._width / 2); h++)
    {
      dat = 255;
      dat = ((dat << 8) & 0xff00) + 255;
      IO.avt.avt_i80_write_dat(dat);
    }
  }

  IO.avt.avt_ld_img_end();
  IO.avt.avt_upd_full((IO.avt.wf_mode << 8));

  IO.tps.tps_vcom_enable();
  IO.avt.avt_wait_dspe_trg();
  IO.avt.avt_wait_dspe_frend();
  IO.tps.tps_vcom_disable();

  IO.avt.avt_slp();
  Debug_str("epd_draw_gray_level_horizontal end...\r\n");
}

void DESTM32T_DEMO::epd_draw_gray_level_vertical(uint8_t div)
{
  uint16_t h, i, j, dat;
  uint8_t v, gray;

  v = (IO._width / 2) % div;
  Debug_str("epd_draw_gray_level_vertical start...\r\n");
  IO.avt.avt_run_sys();
  IO.avt.avt_ld_img(EPD_DATA_8BPP);
  IO.avt.avt_wr_reg_addr(0x0154);

  for (i = 0; i < IO._height; i++)
  {
    gray = 0;
    for (j = 0; j < div; j++)
    {
      for (h = 0; h < ((IO._width / 2) - v) / div; h++)
      {
        dat = gray;
        dat = ((dat << 8) & 0xff00) + gray;
        IO.avt.avt_i80_write_dat(dat);
      }
      gray += 255 / (div - 1);
    }

    for (j = 0; j < v; j++)
    {
      dat = gray;
      dat = ((dat << 8) & 0xff00) + gray;
      IO.avt.avt_i80_write_dat(dat);
    }
  }

  IO.avt.avt_ld_img_end();
  IO.avt.avt_upd_full((IO.avt.wf_mode << 8));

  IO.tps.tps_vcom_enable();
  IO.avt.avt_wait_dspe_trg();
  IO.avt.avt_wait_dspe_frend();
  IO.tps.tps_vcom_disable();

  IO.avt.avt_slp();
  Debug_str("epd_draw_gray_level_vertical end...\r\n");
}

void DESTM32T_DEMO::epd_draw_gray_part(uint8_t gray, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t i, j, dat;

  Debug_str("epd_draw_gray_part start...\r\n");
  IO.avt.avt_run_sys();
  IO.avt.avt_ld_img_area(EPD_DATA_8BPP, x, y, w, h);
  IO.avt.avt_wr_reg_addr(0x0154);

  for (i = 0; i < h; i++)
  {
    for (j = 0; j < (w / 2); j++)
    {
      dat = gray;
      dat = ((dat << 8) & 0xff00) + gray;
      IO.avt.avt_i80_write_dat(dat);
    }
  }

  IO.avt.avt_ld_img_end();
  IO.avt.avt_upd_full_area((IO.avt.wf_mode << 8), x, y, w, h);

  IO.tps.tps_vcom_enable();
  IO.avt.avt_wait_dspe_trg();
  IO.avt.avt_wait_dspe_frend();
  IO.tps.tps_vcom_disable();

  IO.avt.avt_slp();
  Debug_str("epd_draw_gray_part end...\r\n");

}

void DESTM32T_DEMO::epd_draw_pic_part_from_rom(uint8_t* ptr, uint8_t bpp, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t dat;
  uint32_t i, size, cnt;

  Debug_str("epd_draw_pic_part_from_rom start...\r\n");

  IO.avt.avt_run_sys();
  IO.avt.avt_ld_img_area(bpp, x, y, w, h);
  IO.avt.avt_wr_reg_addr(0x0154);

  cnt = 0;

  if (bpp == EPD_DATA_8BPP)
  {
    size = (uint32_t)w * h / 2;
    for (i = 0; i < size; i++)
    {
      dat = ptr[cnt++];
      dat = dat + (((uint16_t)ptr[cnt++] << 8) & 0xff00);
      IO.avt.avt_i80_write_dat(dat);
    }
  }
  else if (bpp == EPD_DATA_4BPP)
  {
    size = (uint32_t)w * h / 4;
    for (i = 0; i < size; i++)
    {
      dat = ptr[cnt++];
      dat = dat + (((uint16_t)ptr[cnt++] << 8) & 0xff00);
      IO.avt.avt_i80_write_dat(dat);
    }
  }
  else if (bpp == EPD_DATA_2BPP)
  {
    size = (uint32_t)w * h / 8;
    for (i = 0; i < size; i++)
    {
      dat = ptr[cnt++];
      dat = dat + (((uint16_t)ptr[cnt++] << 8) & 0xff00);
      IO.avt.avt_i80_write_dat(dat);
    }
  }

  IO.avt.avt_ld_img_end();
  IO.avt.avt_upd_full_area((IO.avt.wf_mode << 8), x, y, w, h);

  IO.tps.tps_vcom_enable();
  IO.avt.avt_wait_dspe_trg();
  IO.avt.avt_wait_dspe_frend();
  IO.tps.tps_vcom_disable();

  IO.avt.avt_slp();
  Debug_str("epd_draw_pic_part_from_rom end...\r\n");
}

void DESTM32T_DEMO::epd_draw_gray_part_lut(uint8_t gray, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t i, j, dat;

  Debug_str("epd_draw_gray_part_lut start...\r\n");

  IO.avt.avt_ld_img_area(EPD_DATA_8BPP, x, y, w, h);
  IO.avt.avt_wr_reg_addr(0x0154);

  for (i = 0; i < h; i++)
  {
    for (j = 0; j < (w / 2); j++)
    {
      dat = gray;
      dat = ((dat << 8) & 0xff00) + gray;
      IO.avt.avt_i80_write_dat(dat);
    }
  }

  IO.avt.avt_ld_img_end();
  IO.avt.avt_upd_full_area((IO.avt.wf_mode << 8), x, y, w, h);

  IO.tps.tps_vcom_enable();
  IO.avt.avt_wait_dspe_trg();
  IO.avt.avt_wait_dspe_lutfree();

  Debug_str("epd_draw_gray_part_lut end...\r\n");

}

void DESTM32T_DEMO::avt_lut_demo(uint8_t gray)
{
  uint16_t i, t;

  t = 30;

  //IO.avt.avt_run_sys();

  IO.avt.wf_mode = EPD_MODE_DU;

  for (i = 50; i < 750; i = i + 50)
  {
    epd_draw_gray_part_lut(gray, i, 50, 50, 50);
    delay(t);
  }

  for (i = 100; i < 550; i = i + 50)
  {
    epd_draw_gray_part_lut(gray, 700, i, 50, 50);
    delay(t);
  }

  for (i = 650; i > 0; i = i - 50)
  {
    epd_draw_gray_part_lut(gray, i, 500, 50, 50);
    delay(t);
  }

  for (i = 450; i > 100; i = i - 50)
  {
    epd_draw_gray_part_lut(gray, 50, i, 50, 50);
    delay(t);
  }

  for (i = 100; i < 650; i = i + 50)
  {
    epd_draw_gray_part_lut(gray, i, 150, 50, 50);
    delay(t);
  }


  epd_draw_gray_part_lut(gray, 600, 200, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 600, 250, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 600, 300, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 600, 350, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 600, 400, 50, 50);
  delay(t);

  epd_draw_gray_part_lut(gray, 550, 400, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 500, 400, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 450, 400, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 400, 400, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 350, 400, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 300, 400, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 250, 400, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 200, 400, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 150, 400, 50, 50);
  delay(t);

  epd_draw_gray_part_lut(gray, 150, 350, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 150, 300, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 150, 250, 50, 50);
  delay(t);

  epd_draw_gray_part_lut(gray, 200, 250, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 250, 250, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 300, 250, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 350, 250, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 400, 250, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 450, 250, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 500, 250, 50, 50);
  delay(t);

  epd_draw_gray_part_lut(gray, 500, 300, 50, 50);
  delay(t);

  epd_draw_gray_part_lut(gray, 450, 300, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 400, 300, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 350, 300, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 300, 300, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 250, 300, 50, 50);
  delay(t);
  epd_draw_gray_part_lut(gray, 200, 300, 50, 50);
  delay(t);


  //IO.avt.avt_slp();
}

uint8_t DESTM32T_DEMO::SpiFlash_ReadWriteByte(uint8_t TxData)
{
  return SPI.transfer(TxData);
}

uint16_t DESTM32T_DEMO::SpiFlash_ReadID(void)
{
  uint16_t Temp = 0;
  FLASH_CS_L;
  SpiFlash_ReadWriteByte(0x90);//���Ͷ�ȡID����
  SpiFlash_ReadWriteByte(0x00);
  SpiFlash_ReadWriteByte(0x00);
  SpiFlash_ReadWriteByte(0x00);
  Temp |= SpiFlash_ReadWriteByte(0xFF) << 8;
  Temp |= SpiFlash_ReadWriteByte(0xFF);
  FLASH_CS_H;
  return Temp;
}

void DESTM32T_DEMO::SpiFlash_StatusWait(void)
{
  uint8_t ret = 0;
  while (1)
  {
    FLASH_CS_L;
    SpiFlash_ReadWriteByte(0x05);//���Ͷ�ȡID����
    ret = SpiFlash_ReadWriteByte(0xFF);
    FLASH_CS_H;
    if ((ret & 0x1) == 0)
      break;
    Debug_str("w");
  }
}

void DESTM32T_DEMO::SpiFlash_EraseBlock32K(uint32_t addr)
{
  uint8_t addr3, addr2, addr1;
  addr1 = addr;
  addr2 = (addr >> 8);
  addr3 = (addr >> 16);
  FLASH_CS_L;
  SpiFlash_ReadWriteByte(0x06);//write enable
  FLASH_CS_H;
  FLASH_CS_L;
  SpiFlash_ReadWriteByte(0x52);
  SpiFlash_ReadWriteByte(addr3);
  SpiFlash_ReadWriteByte(addr2);
  SpiFlash_ReadWriteByte(addr1);
  FLASH_CS_H;
  SpiFlash_StatusWait();
  FLASH_CS_L;
  SpiFlash_ReadWriteByte(0x04);//write disable
  FLASH_CS_H;
}

void DESTM32T_DEMO::SpiFlash_PageProgram(uint32_t addr, uint8_t *data)
{
  uint8_t addr3, addr2, addr1;
  uint16_t i;
  addr1 = addr;
  addr2 = (addr >> 8);
  addr3 = (addr >> 16);
  FLASH_CS_L;
  SpiFlash_ReadWriteByte(0x06);//write enable
  FLASH_CS_H;
  FLASH_CS_L;
  SpiFlash_ReadWriteByte(0x02);
  SpiFlash_ReadWriteByte(addr3);
  SpiFlash_ReadWriteByte(addr2);
  SpiFlash_ReadWriteByte(addr1);
  for (i = 0; i < 256; i++)
  {
    SpiFlash_ReadWriteByte(data[i]);
  }
  FLASH_CS_H;
  SpiFlash_StatusWait();
  FLASH_CS_L;
  SpiFlash_ReadWriteByte(0x04);//write disable
  FLASH_CS_H;
}


void DESTM32T_DEMO::SpiFlash_ReadData(uint32_t addr, uint8_t *data)
{
  uint8_t addr3, addr2, addr1;
  addr1 = addr;
  addr2 = (addr >> 8);
  addr3 = (addr >> 16);
  FLASH_CS_L;
  SpiFlash_ReadWriteByte(0x03);
  SpiFlash_ReadWriteByte(addr3);
  SpiFlash_ReadWriteByte(addr2);
  SpiFlash_ReadWriteByte(addr1);
  FLASH_CS_H;
}

void DESTM32T_DEMO::Debug_str(const char *s)
{
  IO.Debug_str(s);
}

#endif //defined(ARDUINO_ARCH_STM32F1)

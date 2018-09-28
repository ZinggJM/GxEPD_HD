// Class AVT6203A : display IO component class for GDE06BA on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
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

#ifndef _AVT6203A_H_
#define _AVT6203A_H_

#include <Arduino.h>
#include "TPS65185.h"

#define GDE043A2 // GDE060BA or GDE043A2 800*600
//#define GDE060F3 // GDE060F3 6.0" 1024*758     

#define EPD_WF_ADDR				(0x3000)		//����ֽģ�����������ļ���SPI_FLASH����ʼ��ַ
#define EPD_IMG_ADDR			(0x200000)		//����ͼƬ������RAM����ʼ��ַ

#define EPD_MODE_INIT			0
#define EPD_MODE_DU				1
#define EPD_MODE_GC16			2

// Load Image Data Formats (ld_img, dfmt);
#define EPD_DATA_2BPP         	(0<<4)       	// 1����ռ2bit, ��1�ֽڰ���4����
#define EPD_DATA_4BPP           (2<<4)       	// 1����ռ4bit, ��1�ֽڰ���2����
#define EPD_DATA_8BPP           (3<<4)       	// 1����ռ8bit, ��1�ֽڰ���1����

#if 1	//800x600 85Hz
#define  tcon_init_hsize        800
#define  tcon_init_vsize        600
#define  tcon_init_fslen        4
#define  tcon_init_fblen        4
#define  tcon_init_felen        10
#define  tcon_init_lslen        10
#define  tcon_init_lblen        4
#define  tcon_init_lelen        13
#define  tcon_init_pixclkdiv    3
#define  tcon_init_sdrv_cfg     (100 | (1 << 8) | (1 << 9))
#ifdef  GDE043A2 // GDE060BA or GDE043A2 800*600
#define  tcon_init_gdrv_cfg     0x00
#else            // GDE060BA
#define  tcon_init_gdrv_cfg     0x02
#endif
#define  tcon_init_lutidxfmt    (4 | (1 << 7))
#endif

#if 0	//800x600 50Hz
#define  tcon_init_hsize        800
#define  tcon_init_vsize        600
#define  tcon_init_fslen        4
#define  tcon_init_fblen        4
#define  tcon_init_felen        10
#define  tcon_init_lslen        10
#define  tcon_init_lblen        4
#define  tcon_init_lelen        44
#define  tcon_init_pixclkdiv    5
#define  tcon_init_sdrv_cfg     (100 | (1 << 8) | (1 << 9))
#define  tcon_init_gdrv_cfg     0x02
#define  tcon_init_lutidxfmt    (4 | (1 << 7))
#endif
#if 0	//1024x576 85Hz
#define  tcon_init_hsize        1024
#ifdef GDE060F3  //6" 1024*758
#define  tcon_init_vsize        758 
#else           //8" 1024*768
#define  tcon_init_vsize        768 
#endif
#define  tcon_init_fslen        13
#define  tcon_init_fblen        4
#define  tcon_init_felen        10
#define  tcon_init_lslen        10
#define  tcon_init_lblen        4
#define  tcon_init_lelen        42
#define  tcon_init_pixclkdiv    2
#define  tcon_init_sdrv_cfg     (128 | (1 << 8) | (1 << 9))
#define  tcon_init_gdrv_cfg     0x00
#define  tcon_init_lutidxfmt    (4 | (1 << 7))

#endif

class AVT6203A
{
  public:
    uint8_t wf_mode;
  public:
    AVT6203A(TPS65185& _tps) : tps(_tps), _pDiagnosticOutput(0) {};
    void avt_delay(void);
    void avt_reset();
    void avt_busy_wait(void);
    uint16_t avt_i80_read_dat(void);
    void avt_i80_write_dat(uint16_t dat);
    void avt_i80_write_cmd(uint16_t cmd);
    void avt_init_cmd_set(uint16_t SPI_CFG, uint32_t SFM);
    void avt_init_pll_stby(uint16_t PLL_CFG0, uint16_t PLL_CFG1, uint16_t PLL_CFG2);
    void avt_run_sys(void);
    void avt_stby(void);
    void avt_slp(void);
    void avt_init_sys_run(void);
    void avt_init_sys_stby(void);
    void avt_init_sdram(uint16_t SDRAM_CFG0, uint16_t SDRAM_CFG1, uint16_t SDRAM_CFG2, uint16_t SDRAM_CFG3);
    void avt_init_dspe_cfg(uint16_t HSIZE, uint16_t VSIZE, uint16_t SDRVCFG, uint16_t GDRVCFG, uint16_t IDX_FMT_CFG);
    void avt_init_dspe_tmg(uint16_t FSYNC_CFG, uint16_t F_BEGIN_END, uint16_t LSYNC_CFG, uint16_t L_BEGIN_END, uint16_t PIXEL_CLK_CFG);
    void avt_init_rotmode(uint16_t ROTMODE);
    uint16_t avt_rd_reg(uint16_t REG_ADDR);
    void avt_wr_reg(uint16_t REG_ADDR, uint16_t REG_DATA);
    void avt_wr_reg_addr(uint16_t REG_ADDR);
    void avt_rd_sfm(void);
    void avt_wr_sfm(uint16_t SFM_DATA);
    void avt_end_sfm(void);
    void avt_bst_rd_sdr(uint16_t MA0, uint16_t MA1, uint16_t BC0, uint16_t BC1);
    void avt_bst_wr_sdr(uint16_t MA0, uint16_t MA1, uint16_t BC0, uint16_t BC1);
    void avt_bst_end_sdr(void);
    void avt_ld_img(uint16_t ARG);
    void avt_ld_img_area(uint16_t ARG, uint16_t XSTART, uint16_t YSTART, uint16_t WIDTH, uint16_t HEIGHT);
    void avt_ld_img_end(void);
    void avt_ld_img_wait(void);
    void avt_ld_img_setadr(uint32_t MA);
    void avt_ld_img_dspeadr(void);
    void avt_wait_dspe_trg(void);
    void avt_wait_dspe_frend(void);
    void avt_wait_dspe_lutfree(void);
    void avt_wait_dspe_mlutfree(uint16_t LUTMASK);
    void avt_rd_wfm_info(uint32_t MA);
    void avt_upd_init(void);
    void avt_upd_full(uint16_t ARG);
    void avt_upd_full_area(uint16_t ARG, uint16_t XSTART, uint16_t YSTART, uint16_t WIDTH, uint16_t HEIGHT);
    void avt_upd_part(uint16_t ARG);
    void avt_upd_part_area(uint16_t ARG, uint16_t XSTART, uint16_t YSTART, uint16_t WIDTH, uint16_t HEIGHT);
    void avt_upd_gdrv_clr(void);
    void avt_upd_set_imgadr(uint32_t ADR);
    void avt_check_soft_ready(void);
    void avt_spi_flash_idle_wait(void);
    void avt_spi_flash_write_byte(uint8_t dat);
    uint8_t avt_spi_flash_read_byte(void);
    uint8_t avt_spi_flash_read_status(void);
    void avt_spi_flash_deal_wait(void);
    void avt_spi_flash_write_enable(void);
    void avt_spi_flash_write_disable(void);
    void avt_spi_flash_earase();
    void avt_spi_flash_sector_earase(uint32_t addr);     //4K earase
    void avt_spi_flash_write_page(uint32_t addr, uint8_t *data); //256bytes write
    void avt_spi_flash_write_buff(uint32_t addr, uint32_t len, uint8_t *buff);
    void avt_spi_flash_read_buff(uint32_t addr, uint32_t len, uint8_t *buff);
    void avt_spi_flash_read_id(void);
    void avt_spi_flash_write_waveform_deal(void);
    void avt_spi_flash_write_waveform_start(void);
    void avt_spi_flash_write_waveform_end(void);
    void avt_waveform_update(void);
    void AVT_CONFIG_check(void);
    void avt_init(Stream* pDiagnosticOutput);
    uint8_t SpiFlash_ReadWriteByte(uint8_t TxData);
    void SpiFlash_EraseBlock32K(uint32_t addr);
    void SpiFlash_PageProgram(uint32_t addr, uint8_t *data);
    uint16_t SpiFlash_ReadID(void);
    void SpiFlash_StatusWait(void);
    void SpiFlash_ReadData(uint32_t addr, uint8_t *data);
    void Debug_str(const char *s);
    void Debug_hex(unsigned int dat);
    void Debug_dec(unsigned int dat);
  private:
    TPS65185& tps;
    Stream* _pDiagnosticOutput;
    uint16_t Reg0x0204Save;
};

#endif


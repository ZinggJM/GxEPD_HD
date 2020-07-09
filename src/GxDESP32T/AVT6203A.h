// Class AVT6203A : display IO component class for GDE060BA on my DESP32T (proto board) for TCon-11 parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html

#ifndef _AVT6203A_H_
#define _AVT6203A_H_

#include <Arduino.h>
#include "../GxEPD_HD_EPD.h"
#include "TPS65185.h"
#include "MCP23S17.h"

#define EPD_WF_ADDR				(0x3000)
//#define EPD_IMG_ADDR      (0x200000)    // original, @2MB, midlle of RAM, gets partly overwritten for GxGDE060F3, GxGDEW080T5
//#define EPD_IMG_ADDR      (0x240000)    // @2.25MB, minimum for GxGDE060F3, GxGDEW080T5
#define EPD_IMG_ADDR      (0x280000)    // @2.5MB, ok for GxGDE060F3, GxGDEW080T5

#define EPD_MODE_INIT			0
#define EPD_MODE_DU				1
#define EPD_MODE_GC16			2

// Load Image Data Formats (ld_img, dfmt);
#define EPD_DATA_2BPP         	(0<<4)
#define EPD_DATA_4BPP           (2<<4)
#define EPD_DATA_8BPP           (3<<4)

class AVT6203A
{
  public:
    uint8_t wf_mode;
  public:
    AVT6203A(TPS65185& _tps) : tps(_tps), _pDiagnosticOutput(0) {};
    void init(GxEPD_HD::Panel panel, Stream* pDiagnosticOutput); // (pDiagnosticOutput = 0) : disabled
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
    void avt_init();
    void startTransfer();
    void transfer16(uint16_t value);
    void endTransfer();
    void Debug_str(const char *s);
    void Debug_hex(unsigned int dat);
    void Debug_dec(unsigned int dat);
  private:
    TPS65185& tps;
    MCP23S17 pinio16;
    Stream* _pDiagnosticOutput;
    uint16_t Reg0x0204Save;
    uint16_t tcon_init_hsize;
    uint16_t tcon_init_vsize;
    uint16_t tcon_init_fslen;
    uint16_t tcon_init_fblen;
    uint16_t tcon_init_felen;
    uint16_t tcon_init_lslen;
    uint16_t tcon_init_lblen;
    uint16_t tcon_init_lelen;
    uint16_t tcon_init_pixclkdiv;
    uint16_t tcon_init_sdrv_cfg;
    uint16_t tcon_init_gdrv_cfg;
    uint16_t tcon_init_lutidxfmt;
};

#endif

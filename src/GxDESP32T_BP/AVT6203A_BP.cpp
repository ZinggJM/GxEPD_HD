// Class AVT6203A_BP : display IO component class for GDE060BA on my DESP32T_BP (proto board) for TCon-11 parallel interface e-paper display from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for DESTM32-T board with DESTM32-Tcon-11.
//
// The e-paper display and demo board is available from:
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_10571_10574&product_id=57650
// or https://www.aliexpress.com/store/product/6-inch-HD-Interface-High-resolution-electronic-paper-display-e-ink-epaper-with-TCON-Demo-Kit/600281_32838449413.html

#if defined(ARDUINO_ARCH_ESP32)

#include "AVT6203A_BP.h"
#include "DESP32T_BP_wiring.h"
#include "waveform_table.h"
#include "AVT_CONFIG.h"

#define AVT_SPI_FLASH_INIT

#if 1
#define avt_info_str(s)		{Debug_str(s);}
#define avt_info_hex(s)		{Debug_hex(s);}
#else
#define avt_info_str(s)
#define avt_info_hex(s)
#endif

void AVT6203A_BP::init(GxEPD_HD::Panel panel, Stream* pDiagnosticOutput)
{
  _pDiagnosticOutput = pDiagnosticOutput;
  switch (panel)
  {
    case GxEPD_HD::GDE043A2:
      tcon_init_hsize = 800;
      tcon_init_vsize = 600;
      tcon_init_fslen = 4;
      tcon_init_fblen = 4;
      tcon_init_felen = 10;
      tcon_init_lslen = 10;
      tcon_init_lblen = 4;
      tcon_init_lelen = 13;
      tcon_init_pixclkdiv = 3;
      tcon_init_sdrv_cfg = (100 | (1 << 8) | (1 << 9));
      tcon_init_gdrv_cfg = 0x00;
      tcon_init_lutidxfmt = (4 | (1 << 7));
      break;
    case GxEPD_HD::GDE060BA:
      tcon_init_hsize = 800;
      tcon_init_vsize = 600;
      tcon_init_fslen = 4;
      tcon_init_fblen = 4;
      tcon_init_felen = 10;
      tcon_init_lslen = 10;
      tcon_init_lblen = 4;
      tcon_init_lelen = 13;
      tcon_init_pixclkdiv = 3;
      tcon_init_sdrv_cfg = (100 | (1 << 8) | (1 << 9));
      tcon_init_gdrv_cfg = 0x00;
      tcon_init_lutidxfmt = (4 | (1 << 7));
      break;
    case GxEPD_HD::GDE060F3:
      tcon_init_hsize = 1024;
      tcon_init_vsize = 758;
      tcon_init_fslen = 13;
      tcon_init_fblen = 4;
      tcon_init_felen = 10;
      tcon_init_lslen = 10;
      tcon_init_lblen = 4;
      tcon_init_lelen = 42;
      tcon_init_pixclkdiv = 2;
      tcon_init_sdrv_cfg = (128 | (1 << 8) | (1 << 9));
      tcon_init_gdrv_cfg = 0x00;
      tcon_init_lutidxfmt = (4 | (1 << 7));
      break;
    case GxEPD_HD::GDEW080T5:
      tcon_init_hsize = 1024;
      tcon_init_vsize = 768;
      tcon_init_fslen = 13;
      tcon_init_fblen = 4;
      tcon_init_felen = 10;
      tcon_init_lslen = 10;
      tcon_init_lblen = 4;
      tcon_init_lelen = 42;
      tcon_init_pixclkdiv = 2;
      tcon_init_sdrv_cfg = (128 | (1 << 8) | (1 << 9));
      tcon_init_gdrv_cfg = 0x00;
      tcon_init_lutidxfmt = (4 | (1 << 7));
      break;
    default:
      tcon_init_hsize = 800;
      tcon_init_vsize = 600;
      tcon_init_fslen = 4;
      tcon_init_fblen = 4;
      tcon_init_felen = 10;
      tcon_init_lslen = 10;
      tcon_init_lblen = 4;
      tcon_init_lelen = 13;
      tcon_init_pixclkdiv = 3;
      tcon_init_sdrv_cfg = (100 | (1 << 8) | (1 << 9));
      tcon_init_gdrv_cfg = 0x00;
      tcon_init_lutidxfmt = (4 | (1 << 7));
  }
  //?pinio16.init(SS);
}

void AVT6203A_BP::avt_reset()
{
  AVT_RST_L;
  delay(20);
  AVT_RST_H;
  avt_busy_wait();
}

void AVT6203A_BP::avt_busy_wait(void)
{
  while (1)
  {
    if (IO.getAVT_RDY()) break;
  }
}

uint16_t AVT6203A_BP::avt_i80_read_dat(void)
{
  delayMicroseconds(2);
  return IO.readData();
}

void AVT6203A_BP::avt_i80_write_dat(uint16_t dat)
{
  IO.writeData(dat);
}

void AVT6203A_BP::avt_i80_write_cmd(uint16_t cmd)
{
  avt_busy_wait();
  IO.writeCommand(cmd);
}

//System Commands

void AVT6203A_BP::avt_init_cmd_set(uint16_t SPI_CFG, uint32_t SFM)
{
  avt_i80_write_cmd(0x0000);
  avt_i80_write_dat(SPI_CFG);		//REG_0204[7:0]
  avt_i80_write_dat(SFM);			//SFM[15:0]
  avt_i80_write_dat(SFM >> 16);		//SFM[23:16]
}

void AVT6203A_BP::avt_init_pll_stby(uint16_t PLL_CFG0, uint16_t PLL_CFG1, uint16_t PLL_CFG2)
{
  avt_i80_write_cmd(0x0001);
  avt_i80_write_dat(PLL_CFG0); 	//REG_0010[5:0]
  avt_i80_write_dat(PLL_CFG1);  	//REG_0012[15:12]
  avt_i80_write_dat(PLL_CFG2); 	//REG_0014[7:3]
}

void AVT6203A_BP::avt_run_sys(void)
{
  tps.tps_source_gate_enable();
  avt_i80_write_cmd(0x0002);
  avt_busy_wait();
}

void AVT6203A_BP::avt_stby(void)
{
  avt_i80_write_cmd(0x0004);
  avt_busy_wait();
}

void AVT6203A_BP::avt_slp(void)
{
  avt_i80_write_cmd(0x0005);
  avt_busy_wait();
  tps.tps_vcom_disable();
  tps.tps_source_gate_disable();
}

void AVT6203A_BP::avt_init_sys_run(void)
{
  avt_i80_write_cmd(0x0006);
  avt_busy_wait();
  tps.tps_source_gate_enable();
}

void AVT6203A_BP::avt_init_sys_stby(void)
{
  avt_i80_write_cmd(0x0007);
}

void AVT6203A_BP::avt_init_sdram(uint16_t SDRAM_CFG0, uint16_t SDRAM_CFG1, uint16_t SDRAM_CFG2, uint16_t SDRAM_CFG3)
{
  avt_i80_write_cmd(0x0008);
  avt_i80_write_dat(SDRAM_CFG0); 	//REG_0100[15:4][2:0]
  avt_i80_write_dat(SDRAM_CFG1);  	//REG_0106[15:0]
  avt_i80_write_dat(SDRAM_CFG2); 	//REG_0108[5:4]
  avt_i80_write_dat(SDRAM_CFG3); 	//REG_010A[14:8][2:0]
}

void AVT6203A_BP::avt_init_dspe_cfg(uint16_t HSIZE, uint16_t VSIZE, uint16_t SDRVCFG, uint16_t GDRVCFG, uint16_t IDX_FMT_CFG)
{
  avt_i80_write_cmd(0x0009);
  avt_i80_write_dat(HSIZE); 		//REG_0306[12:0]
  avt_i80_write_dat(VSIZE);  		//REG_0300[12:0]
  avt_i80_write_dat(SDRVCFG); 		//REG_030C[15:0]
  avt_i80_write_dat(GDRVCFG); 		//REG_030E[15:3][1:0]
  avt_i80_write_dat(IDX_FMT_CFG); 	//REG_0330[15][7:6][2:0]
}

void AVT6203A_BP::avt_init_dspe_tmg(uint16_t FSYNC_CFG, uint16_t F_BEGIN_END, uint16_t LSYNC_CFG, uint16_t L_BEGIN_END, uint16_t PIXEL_CLK_CFG)
{
  avt_i80_write_cmd(0x000A);
  avt_i80_write_dat(FSYNC_CFG); 	//REG_0302[7:0]
  avt_i80_write_dat(F_BEGIN_END); 	//REG_0304[15:0]
  avt_i80_write_dat(LSYNC_CFG); 	//REG_0308[7:0]
  avt_i80_write_dat(L_BEGIN_END); 	//REG_030A[15:0]
  avt_i80_write_dat(PIXEL_CLK_CFG);//REG_0018[4:0]
}

void AVT6203A_BP::avt_init_rotmode(uint16_t ROTMODE)
{
  avt_i80_write_cmd(0x000B);
  avt_i80_write_dat(ROTMODE); 		//REG_032C[9:8]
}

//Register and Memory Access Commands

uint16_t AVT6203A_BP::avt_rd_reg(uint16_t REG_ADDR)
{
  uint16_t REG_DATA;

  avt_i80_write_cmd(0x0010);
  avt_i80_write_dat(REG_ADDR); 	//REG_ADDR[15:0]
  REG_DATA = avt_i80_read_dat();

  return REG_DATA;
}

void AVT6203A_BP::avt_wr_reg(uint16_t REG_ADDR, uint16_t REG_DATA)
{
  avt_i80_write_cmd(0x0011);
  avt_i80_write_dat(REG_ADDR); 	//REG_ADDR[15:0]
  avt_i80_write_dat(REG_DATA); 	//REG_DATA[15:0]
}

void AVT6203A_BP::avt_wr_reg_addr(uint16_t REG_ADDR)
{
  avt_i80_write_cmd(0x0011);
  avt_i80_write_dat(REG_ADDR); 	//REG_ADDR[15:0]
}

void AVT6203A_BP::avt_rd_sfm(void)
{
  avt_i80_write_cmd(0x0012);
}

void AVT6203A_BP::avt_wr_sfm(uint16_t SFM_DATA)
{
  avt_i80_write_cmd(0x0013);
  avt_i80_write_dat(SFM_DATA); 	//SFM_DATA[15:0]
}

void AVT6203A_BP::avt_end_sfm(void)
{
  avt_i80_write_cmd(0x0014);
}

//Burst Access Commands

void AVT6203A_BP::avt_bst_rd_sdr(uint16_t MA0, uint16_t MA1, uint16_t BC0, uint16_t BC1)
{
  avt_i80_write_cmd(0x001C);
  avt_i80_write_dat(MA0); 			//REG_0144[15:0]
  avt_i80_write_dat(MA1); 			//REG_0146[9:0]
  avt_i80_write_dat(BC0); 			//REG_0148[15:0]
  avt_i80_write_dat(BC1); 			//REG_014A[9:0]
  avt_busy_wait();
}

void AVT6203A_BP::avt_bst_wr_sdr(uint16_t MA0, uint16_t MA1, uint16_t BC0, uint16_t BC1)
{
  avt_i80_write_cmd(0x001D);
  avt_i80_write_dat(MA0); 			//REG_0144[15:0]
  avt_i80_write_dat(MA1); 			//REG_0146[7:0]
  avt_i80_write_dat(BC0); 			//REG_0148[15:0]
  avt_i80_write_dat(BC1); 			//REG_014A[9:0]
  avt_busy_wait();
}

void AVT6203A_BP::avt_bst_end_sdr(void)
{
  avt_i80_write_cmd(0x001E);
}

//Image Loading Commands

void AVT6203A_BP::avt_ld_img(uint16_t ARG)
{
  avt_i80_write_cmd(0x0020);
  avt_i80_write_dat(ARG); 			//REG_0140[5:4]
}

void AVT6203A_BP::avt_ld_img_area(uint16_t ARG, uint16_t XSTART, uint16_t YSTART, uint16_t WIDTH, uint16_t HEIGHT)
{
  avt_i80_write_cmd(0x0022);
  avt_i80_write_dat(ARG); 			//REG_0140[5:4]
  avt_i80_write_dat(XSTART); 		//REG_014C[11:0]
  avt_i80_write_dat(YSTART); 		//REG_014E[11:0]
  avt_i80_write_dat(WIDTH); 		//REG_0150[12:0]
  avt_i80_write_dat(HEIGHT); 		//REG_0152[12:0]
}

void AVT6203A_BP::avt_ld_img_end(void)
{
  avt_i80_write_cmd(0x0023);
}

void AVT6203A_BP::avt_ld_img_wait(void)
{
  avt_i80_write_cmd(0x0024);
}

void AVT6203A_BP::avt_ld_img_setadr(uint32_t MA)
{
  avt_i80_write_cmd(0x0025);
  avt_i80_write_dat(MA); 			//REG_0144[15:0]
  avt_i80_write_dat(MA >> 16); 		//REG_0146[9:0]
}

void AVT6203A_BP::avt_ld_img_dspeadr(void)
{
  avt_i80_write_cmd(0x0026);
}

//Polling Commands

void AVT6203A_BP::avt_wait_dspe_trg(void)
{
  avt_i80_write_cmd(0x0028);
}

void AVT6203A_BP::avt_wait_dspe_frend(void)
{
  avt_i80_write_cmd(0x0029);
  avt_busy_wait();
  //tps.tps_vcom_disable();
}

void AVT6203A_BP::avt_wait_dspe_lutfree(void)
{
  avt_i80_write_cmd(0x002A);
  avt_busy_wait();
}

void AVT6203A_BP::avt_wait_dspe_mlutfree(uint16_t LUTMASK)
{
  avt_i80_write_cmd(0x002B);
  avt_i80_write_dat(LUTMASK); 		//REG_032E[15:0]
}

//Waveform Update Commands

void AVT6203A_BP::avt_rd_wfm_info(uint32_t MA)
{
  avt_i80_write_cmd(0x0030);
  avt_i80_write_dat(MA); 			//REG_0350[15:0]
  avt_i80_write_dat(MA >> 16); 		//REG_0352[7:0]
}

void AVT6203A_BP::avt_upd_init(void)
{
  avt_i80_write_cmd(0x0032);
}

void AVT6203A_BP::avt_upd_full(uint16_t ARG)
{
  avt_i80_write_cmd(0x0033);
  avt_i80_write_dat(ARG);     		//REG_0334[14][11:4]
}

void AVT6203A_BP::avt_upd_full_area(uint16_t ARG, uint16_t XSTART, uint16_t YSTART, uint16_t WIDTH, uint16_t HEIGHT)
{
  avt_i80_write_cmd(0x0034);
  avt_i80_write_dat(ARG); 			//REG_0334[14][11:4]
  avt_i80_write_dat(XSTART); 		//REG_0340[11:0]
  avt_i80_write_dat(YSTART); 		//REG_0342[11:0]
  avt_i80_write_dat(WIDTH); 		//REG_0344[12:0]
  avt_i80_write_dat(HEIGHT); 		//REG_0346[12:0]
}

void AVT6203A_BP::avt_upd_part(uint16_t ARG)
{
  avt_i80_write_cmd(0x0035);
  avt_i80_write_dat(ARG);     		//REG_0334[14][11:4]
}

void AVT6203A_BP::avt_upd_part_area(uint16_t ARG, uint16_t XSTART, uint16_t YSTART, uint16_t WIDTH, uint16_t HEIGHT)
{
  avt_i80_write_cmd(0x0036);
  avt_i80_write_dat(ARG); 			//REG_0334[14][11:4]
  avt_i80_write_dat(XSTART); 		//REG_0340[11:0]
  avt_i80_write_dat(YSTART); 		//REG_0342[11:0]
  avt_i80_write_dat(WIDTH); 		//REG_0344[12:0]
  avt_i80_write_dat(HEIGHT); 		//REG_0346[12:0]
}

void AVT6203A_BP::avt_upd_gdrv_clr(void)
{
  avt_i80_write_cmd(0x0037);
}

void AVT6203A_BP::avt_upd_set_imgadr(uint32_t ADR)
{
  avt_i80_write_cmd(0x0038);
  avt_i80_write_dat(ADR); 			//REG_0310[15:0]
  avt_i80_write_dat(ADR >> 16); 		//REG_0312[15:0]
}

void AVT6203A_BP::avt_check_soft_ready(void)
{
  //while(avt_rd_reg(0x000a)&32)		//check bit 5 of SYSTEM STATUS REGISTER
  while (avt_rd_reg(0x000a) & 0x24)	//
  {
    ;//Debug_str("=");
  }
}


void AVT6203A_BP::avt_spi_flash_idle_wait(void)
{
  uint16_t regval;

  while (1)
  {
    regval = avt_rd_reg(0x0206);
    if ((regval & 0x8) == 0)
      break;

    Debug_str("-");
  }
}

void AVT6203A_BP::avt_spi_flash_write_byte(uint8_t dat)
{
  uint16_t v;

  v = dat;
  v =	v | 0x100;
  avt_wr_reg(0x0202, v);
  avt_spi_flash_idle_wait();
}

uint8_t AVT6203A_BP::avt_spi_flash_read_byte(void)
{
  uint16_t v;

  avt_wr_reg(0x0202, 0);
  avt_spi_flash_idle_wait();
  v = avt_rd_reg(0x0200);
  return (v & 0xFF);
}

uint8_t AVT6203A_BP::avt_spi_flash_read_status(void)
{
  uint8_t status;

  avt_wr_reg(0x0208, 0x1);
  avt_spi_flash_write_byte(0x05);
  status = avt_spi_flash_read_byte();
  avt_wr_reg(0x0208, 0x0);
  return status;
}

void AVT6203A_BP::avt_spi_flash_deal_wait(void)
{
  while (1)
  {
    if ((avt_spi_flash_read_status() & 0x1) == 0)
      break;
    Debug_str("+");
  }
}

void AVT6203A_BP::avt_spi_flash_write_enable(void)
{
  avt_wr_reg(0x0208, 0x1);
  avt_spi_flash_write_byte(0x06);
  avt_wr_reg(0x0208, 0x0);
}

void AVT6203A_BP::avt_spi_flash_write_disable(void)
{
  avt_wr_reg(0x0208, 0x1);
  avt_spi_flash_write_byte(0x04);
  avt_wr_reg(0x0208, 0x0);
}

void AVT6203A_BP::avt_spi_flash_earase()
{
  avt_spi_flash_write_enable();
  avt_wr_reg(0x0208, 0x1);
  avt_spi_flash_write_byte(0xc7);
  avt_wr_reg(0x0208, 0x0);
  avt_spi_flash_deal_wait();
  avt_spi_flash_write_disable();
}

void AVT6203A_BP::avt_spi_flash_sector_earase(uint32_t addr)			//4K earase
{
  uint8_t addr1, addr2, addr3;

  addr1 = (addr & 0xff);
  addr2 = ((addr >> 8) & 0xff);
  addr3 = ((addr >> 16) & 0xff);

  avt_spi_flash_write_enable();
  avt_wr_reg(0x0208, 0x1);
  avt_spi_flash_write_byte(0x20);
  avt_spi_flash_write_byte(addr3);
  avt_spi_flash_write_byte(addr2);
  avt_spi_flash_write_byte(addr1);
  avt_wr_reg(0x0208, 0x0);
  avt_spi_flash_deal_wait();
  avt_spi_flash_write_disable();
}

void AVT6203A_BP::avt_spi_flash_write_page(uint32_t addr, uint8_t *data)	//256bytes write
{
  uint16_t i;
  uint8_t addr1, addr2, addr3;

  addr1 = (addr & 0xff);
  addr2 = ((addr >> 8) & 0xff);
  addr3 = ((addr >> 16) & 0xff);

  avt_spi_flash_write_enable();

  avt_wr_reg(0x0208, 0x1);
  avt_spi_flash_write_byte(0x02);
  avt_spi_flash_write_byte(addr3);
  avt_spi_flash_write_byte(addr2);
  avt_spi_flash_write_byte(addr1);


  for (i = 0; i < 256; i++)
  {
    avt_spi_flash_write_byte(data[i]);
  }
  avt_wr_reg(0x0208, 0x0);

  avt_spi_flash_deal_wait();
  avt_spi_flash_write_disable();
}

void AVT6203A_BP::avt_spi_flash_write_buff(uint32_t addr, uint32_t len, uint8_t *buff)
{
  uint32_t i, pagenum;
  uint8_t *buffptr;

  buffptr = buff;
  pagenum = len / 256 + 1;

  for (i = 0; i < pagenum; i++)
  {
    Debug_str("writing spiflash page:");
    Debug_dec(i);
    avt_spi_flash_write_page(addr, buffptr);
    buffptr = buffptr + 256;
    addr = addr + 256;
    Debug_str("\r\n");
  }
}

void AVT6203A_BP::avt_spi_flash_read_buff(uint32_t addr, uint32_t len, uint8_t *buff)
{
  uint32_t i;
  uint8_t addr1, addr2, addr3;

  addr1 = (addr & 0xff);
  addr2 = ((addr >> 8) & 0xff);
  addr3 = ((addr >> 16) & 0xff);

  avt_wr_reg(0x0208, 0x1);
  avt_spi_flash_write_byte(0x03);
  avt_spi_flash_write_byte(addr3);
  avt_spi_flash_write_byte(addr2);
  avt_spi_flash_write_byte(addr1);

  Debug_str("start to read data from the flash\r\n");

  for (i = 0; i < len; i++)
  {
    buff[i] = avt_spi_flash_read_byte();
  }
  avt_wr_reg(0x0208, 0x0);
}

void AVT6203A_BP::avt_spi_flash_read_id(void)
{
  uint8_t temp1, temp2;


  Debug_str("avt_spi_flash_read_id:");

  avt_wr_reg(0x0208, 0x1);

  avt_spi_flash_write_byte(0x90);
  avt_spi_flash_write_byte(0x00);
  avt_spi_flash_write_byte(0x00);
  avt_spi_flash_write_byte(0x00);

  temp1 = avt_spi_flash_read_byte();
  temp2 = avt_spi_flash_read_byte();

  avt_wr_reg(0x0208, 0x0);

  Debug_hex(temp1);
  Debug_hex(temp2);
  Debug_str("\r\n");
}

void AVT6203A_BP::avt_spi_flash_write_waveform_deal(void)
{
  uint32_t size, i, sector, addr;
  //uint8_t rbuf[2];

  if (Reg0x0204Save != 0x99)
  {
    Debug_str("avt_waveform_update -> 0x0204 value error, cann't update waveform ...\r\n");
    return;
  }

  size = sizeof(waveform_table);
  sector = size / 4096;			//1 sector = 4096bytes
  if ((size % 4096) != 0)		//������������ȼ������sectorҪ��һЩ����
    sector++;

  Debug_str("avt_waveform_update -> size:");
  Debug_hex(size >> 16);
  Debug_hex(size);
  Debug_str("\r\navt_waveform_update -> earase sector:");
  Debug_dec(sector);
  Debug_str("\r\n");

  addr = EPD_WF_ADDR;
  for (i = 0; i < sector; i++)
  {
    avt_spi_flash_sector_earase(addr);
    addr += 4096;
  }

  Debug_str("\r\navt_waveform_update -> waveform write begin ...\r\n");
  avt_spi_flash_write_buff(EPD_WF_ADDR, size, (uint8_t*)waveform_table);
  Debug_str("avt_waveform_update -> waveform  write finish !\r\n\r\n");

}

void AVT6203A_BP::avt_spi_flash_write_waveform_start(void)
{

  Reg0x0204Save = avt_rd_reg(0x0204);
  Debug_str("avt_waveform_update -> 0x0204 curvalue:");
  Debug_hex(Reg0x0204Save);
  Debug_str("\r\n");
  if (Reg0x0204Save != 0x99)
  {
    Debug_str("avt_waveform_update -> read 0x0204 error !\r\n");
    return;
  }
  avt_wr_reg(0x0204, 0x19);
  avt_spi_flash_read_id();
}

void AVT6203A_BP::avt_spi_flash_write_waveform_end(void)
{
  avt_wr_reg(0x0204, Reg0x0204Save);
}

void AVT6203A_BP::avt_waveform_update(void)
{
  avt_spi_flash_write_waveform_start();
  avt_spi_flash_write_waveform_deal();
  avt_spi_flash_write_waveform_end();
}


void AVT6203A_BP::AVT_CONFIG_check(void)
{
  uint16_t sector, checksum, checksumorg;
  uint32_t addr, i, size;

  avt_reset();

  Reg0x0204Save = avt_rd_reg(0x0204);
  Debug_str("AVT_CONFIG_check -> 0x0204 curvalue:");
  Debug_hex(Reg0x0204Save);
  Debug_str("\r\n");
  if (Reg0x0204Save != 0x99)
  {
    Debug_str("AVT_CONFIG_check -> read 0x0204 error !\r\n");
    while (1)
    {
      //Debug_str("AVT_CONFIG_check -> read 0x0204 error !\r\n");
      delay(1000);
    }
  }
  avt_wr_reg(0x0204, 0x19);

  checksumorg = 0;
  checksum = 0;
  size = sizeof(AVT_CONFIG);
  for (i = 0; i < size; i++)
    checksumorg += AVT_CONFIG[i];

  avt_wr_reg(0x0208, 0x1);
  avt_spi_flash_write_byte(0x03);				//read data from addr:0x000000
  avt_spi_flash_write_byte(0);
  avt_spi_flash_write_byte(0);
  avt_spi_flash_write_byte(0);
  for (i = 0; i < size; i++)
  {
    checksum += avt_spi_flash_read_byte();	//read data from addr:0x000000
  }
  avt_wr_reg(0x0208, 0x0);

  Debug_str("AVT_CONFIG_check -> size:");
  Debug_hex(size >> 16);
  Debug_hex(size);
  Debug_str(", checksum:");
  Debug_hex(checksum);
  Debug_str("[ ");
  Debug_hex(checksumorg);
  Debug_str("]\r\n");

#if 1
  if (checksum == checksumorg)
  {
    avt_wr_reg(0x0204, Reg0x0204Save);
    Debug_str("AVT_CONFIG_check -> config checksum OK !\r\n");
    return;
  }
#endif

  sector = size / 4096;			//1 sector = 4096bytes
  if ((size % 4096) != 0)		//������������ȼ������sectorҪ��һЩ����
    sector++;

  Debug_str("AVT_CONFIG_check -> earase sector:");
  Debug_dec(sector);
  Debug_str("\r\n");

  addr = 0;
  for (i = 0; i < sector; i++)
  {
    avt_spi_flash_sector_earase(addr);
    addr += 4096;
  }

  Debug_str("\r\nAVT_CONFIG_check -> config write begin ...\r\n");
  avt_spi_flash_write_buff(0, size, (uint8_t*)AVT_CONFIG);
  Debug_str("AVT_CONFIG_check -> config write finish !\r\n");

  checksum = 0;
  avt_wr_reg(0x0208, 0x1);
  avt_spi_flash_write_byte(0x03);				//read data from addr:0x000000 again
  avt_spi_flash_write_byte(0);
  avt_spi_flash_write_byte(0);
  avt_spi_flash_write_byte(0);
  for (i = 0; i < size; i++)
  {
    checksum += avt_spi_flash_read_byte();	//read data from addr:0x000000
  }
  avt_wr_reg(0x0208, 0x0);

  if (checksum != checksumorg)
  {
    while (1)
    {
      Debug_str("AVT_CONFIG_check -> config write error !\r\n");
      delay(1000);
    }
  }

  avt_wr_reg(0x0204, Reg0x0204Save);
  Debug_str("AVT_CONFIG_check -> config write and checksum OK !\r\n\r\n");
}


//avt init

void AVT6203A_BP::avt_init()
{
  avt_reset();
  //delay(100);
  Reg0x0204Save = avt_rd_reg(0x0204);
  avt_init_sys_run();

  avt_init_dspe_cfg(tcon_init_hsize, tcon_init_vsize, tcon_init_sdrv_cfg, tcon_init_gdrv_cfg, tcon_init_lutidxfmt);
  avt_info_str("INIT_DSPE_CFG is sent!");
  avt_info_str("\r\nCheck Register (HSIZE) updated by INIT_DSPE_CFG: addr 0x0306, value ");
  avt_info_hex(avt_rd_reg(0x0306));
  avt_info_str("\r\nCheck Register (VSIZE) updated by INIT_DSPE_CFG: addr 0x0300, value ");
  avt_info_hex(avt_rd_reg(0x0300));
  avt_info_str("\r\nCheck Register (SDRVCFG) updated by INIT_DSPE_CFG: addr 0x030C, value ");
  avt_info_hex(avt_rd_reg(0x030C));
  avt_info_str("\r\nCheck Register (GDRVCFG) updated by INIT_DSPE_CFG: addr 0x030E, value ");
  avt_info_hex(avt_rd_reg(0x030E));
  avt_info_str("\r\nCheck Register (LUT IDX FMT CFG) updated by INIT_DSPE_CFG: addr 0x0330, value ");
  avt_info_hex(avt_rd_reg(0x0330));

  avt_init_dspe_tmg(tcon_init_fslen, (tcon_init_felen << 8) | tcon_init_fblen, tcon_init_lslen, (tcon_init_lelen << 8) | tcon_init_lblen, tcon_init_pixclkdiv);
  avt_info_str("\r\nCheck Register (Frame Sync CFG) updated by INIT_DSPE_TMG: addr 0x0302, value ");
  avt_info_hex(avt_rd_reg(0x0302));
  avt_info_str("\r\nCheck Register (Frame Begin/End CFG) updated by INIT_DSPE_TMG: addr 0x0304, value ");
  avt_info_hex(avt_rd_reg(0x0304));
  avt_info_str("\r\nCheck Register (Line Sync CFG) updated by INIT_DSPE_TMG: addr 0x0308, value ");
  avt_info_hex(avt_rd_reg(0x0308));
  avt_info_str("\r\nCheck Register (Line Begin/End CFG) updated by INIT_DSPE_TMG: addr 0x030A, value ");
  avt_info_hex(avt_rd_reg(0x030A));
  avt_info_str("\r\nCheck Register (Pixel Clock CFG) updated by INIT_DSPE_TMG: addr 0x018, value ");
  avt_info_hex(avt_rd_reg(0x018));

  avt_rd_wfm_info(EPD_WF_ADDR);
  avt_info_str("\r\nRD_WFM_INFO is sent!");
  avt_info_str("\r\nCheck Register MA[15:0] updated by RD_WFM_INFO: addr 0x0350, value ");
  avt_info_hex(avt_rd_reg(0x0350));
  avt_info_str("\r\nCheck Register MA[23:16] updated by RD_WFM_INFO: addr 0x0352, value ");
  avt_info_hex(avt_rd_reg(0x0352));

  avt_ld_img_setadr(EPD_IMG_ADDR);
  avt_info_str("\r\nCheck Register MA[15:0] updated by LD_IMG_SETADR: addr 0x0144, value ");
  avt_info_hex(avt_rd_reg(0x0144));
  avt_info_str("\r\nCheck Register MA[25:16] updated by LD_IMG_SETADR: addr 0x0146, value ");
  avt_info_hex(avt_rd_reg(0x0146));

  avt_upd_set_imgadr(EPD_IMG_ADDR);
  avt_info_str("\r\nCheck Register MA[15:0] updated by UPD_SET_SETADR: addr 0x0310, value ");
  avt_info_hex(avt_rd_reg(0x0310));
  avt_info_str("\r\nCheck Register MA[31:16] updated by UPD_SET_SETADR: addr 0x0312, value ");
  avt_info_hex(avt_rd_reg(0x0312));

  avt_ld_img(EPD_DATA_8BPP);
  avt_info_str("\r\nLD_IMG is sent!");
  avt_info_str("\r\nCheck Register ARG[15:0] updated by LD_IMG: addr 0x0140, value ");
  avt_info_hex(avt_rd_reg(0x0140));
  avt_info_str("\r\n");
  avt_info_str("\r\n");
}

void AVT6203A_BP::startTransfer(uint32_t length)
{
  IO.startTransfer(length);
}

void AVT6203A_BP::transfer16(uint16_t value)
{
  IO.transfer(value);
}

void AVT6203A_BP::endTransfer()
{
  IO.endTransfer();
}

void AVT6203A_BP::Debug_str(const char *s)
{
  if (_pDiagnosticOutput) _pDiagnosticOutput->print(s);
}

void AVT6203A_BP::Debug_hex(unsigned int dat)
{
  char buf[8];
  unsigned char i;

  if (!_pDiagnosticOutput) return;

  buf[0] = '0';
  buf[1] = 'x';
  i = (dat >> 12) & 0x0f;
  if (i > 9)
    buf[2] = 'A' - 10 + i;
  else
    buf[2] = '0' + i;

  i = (dat >> 8) & 0x0f;
  if (i > 9)
    buf[3] = 'A' - 10 + i;
  else
    buf[3] = '0' + i;

  i = (dat >> 4) & 0x0f;
  if (i > 9)
    buf[4] = 'A' - 10 + i;
  else
    buf[4] = '0' + i;

  i = dat & 0x0f;
  if (i > 9)
    buf[5] = 'A' - 10 + i;
  else
    buf[5] = '0' + i;

  buf[6] = ' ';
  buf[7] = 0;

  Debug_str(buf);
}

void AVT6203A_BP::Debug_dec(unsigned int dat)
{
  char buf[7];

  if (!_pDiagnosticOutput) return;

  buf[0] = '0' + dat / 10000;
  buf[1] = '0' + (dat % 10000) / 1000;
  buf[2] = '0' + (dat % 1000) / 100;
  buf[3] = '0' + (dat % 100) / 10;
  buf[4] = '0' + (dat % 10) ;
  buf[5] = ' ';
  buf[6] = 0;
  Debug_str(buf);
}

#endif

// Class TPS65185_BP : display IO component class for GDE060BA on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
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

//#if defined(ARDUINO_ARCH_STM32F1)
#if defined(ARDUINO_ARCH_ESP32)

#include "TPS65185_BP.h"
#include "DESP32T_BP_wiring.h"
#include <Wire.h>

#define TI_REG_TMST_VALUE	0x00	//Thermistor value read by ADC
#define TI_REG_ENABLE		0x01	//Enable/disable bits for regulators
#define TI_REG_VADJ			0x02	//VPOS/VNEG voltage adjustment
#define TI_REG_VCOM1		0x03	//Voltage settings for VCOM
#define TI_REG_VCOM2		0x04	//Voltage settings for VCOM + control
#define TI_REG_INT_EN1		0x05	//Interrupt enable group1
#define TI_REG_INT_EN2		0x06	//Interrupt enable group2
#define TI_REG_INT1			0x07	//Interrupt group1
#define TI_REG_INT2			0x08	//Interrupt group2
#define TI_REG_UPSEQ0		0x09	//Power-up strobe assignment
#define TI_REG_UPSEQ1		0x0A	//Power-up sequence delay times
#define TI_REG_DWNSEQ0		0x0B	//Power-down strobe assignment
#define TI_REG_DWNSEQ1		0x0C	//Power-down sequence delay times
#define TI_REG_TMST1		0x0D	//Thermistor configuration
#define TI_REG_TMST2		0x0E	//Thermistor hot temp set
#define TI_REG_PG			0x0F	//Power good status each rails
#define TI_REG_REVID		0x10	//Device revision ID information

//#define TPS65185_ADDR		0xd0 // shifted!
#define TPS65185_ADDR    0x68 // normal

#define TI_STROBE1			0
#define TI_STROBE2			1
#define TI_STROBE3			2
#define TI_STROBE4			3

#define TI_UPLY3MS			0
#define TI_UPLY6MS			1
#define TI_UPLY9MS			2
#define TI_UPLY12MS			3

#define TI_DWNLY6MS			0
#define TI_DWNLY12MS		1
#define TI_DWNLY24MS		2
#define TI_DWNLY48MS		3

void TPS65185_BP::I2C_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t dat)
{
  //Serial1.println("I2C_Write");
  Wire.beginTransmission(dev_addr);
  Wire.write(reg_addr);
  Wire.write(dat);
  Wire.endTransmission();
  //Serial1.println("I2C_Write done");
}

uint8_t TPS65185_BP::I2C_Read(uint8_t dev_addr, uint8_t reg_addr)
{
  uint8_t dat = 0;
  Wire.beginTransmission(dev_addr);
  Wire.write(reg_addr);
  Wire.endTransmission();
  Wire.requestFrom(dev_addr, uint8_t(1));
  if (Wire.available())
  {
    dat = Wire.read();
  }
  return dat;
}


void TPS65185_BP::I2C_Write_Frame(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t* dat)
{
  Wire.beginTransmission(dev_addr);
  Wire.write(reg_addr);
  Wire.write(dat, length);
  Wire.endTransmission();
}


void TPS65185_BP::I2C_Read_Frame(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t* dat)
{
  for (uint8_t i = 0; i < length; i++)
  {
    dat[i] = 0;
  }
  Wire.beginTransmission(dev_addr);
  Wire.write(reg_addr);
  Wire.endTransmission();
  Wire.requestFrom(dev_addr, length);
  while (Wire.available())
  {
    *dat++ = Wire.read();
  }
}

void TPS65185_BP::tps_power_sequence_set(void)
{
  uint8_t dat;

  dat = ((TI_STROBE3 << 6) | (TI_STROBE4 << 4) | (TI_STROBE1 << 2) | (TI_STROBE2 << 0));	//VGL->VNEG->VGH->VPOS
  I2C_Write(TPS65185_ADDR, TI_REG_UPSEQ0, dat);

  dat = ((TI_UPLY3MS << 6) | (TI_UPLY3MS << 4) | (TI_UPLY3MS << 2) | (TI_UPLY3MS << 0));
  I2C_Write(TPS65185_ADDR, TI_REG_UPSEQ1, dat);

  dat = ((TI_STROBE2 << 6) | (TI_STROBE1 << 4) | (TI_STROBE4 << 2) | (TI_STROBE3 << 0));	//VPOS->VGH->VNEG->VGL
  I2C_Write(TPS65185_ADDR, TI_REG_DWNSEQ0, dat);

  dat = ((TI_DWNLY24MS << 6) | (TI_DWNLY6MS << 4) | (TI_UPLY6MS << 2) | (0 << 1) | 0);
  I2C_Write(TPS65185_ADDR, TI_REG_DWNSEQ1, dat);
}

void TPS65185_BP::tps_vcom_set(uint16_t vcom)
{
  uint8_t dat;

  dat = ((vcom / 10) & 0x00FF);
  I2C_Write(TPS65185_ADDR, TI_REG_VCOM1, dat);

  dat = (((vcom / 10) >> 8) & 0x01);
  I2C_Write(TPS65185_ADDR, TI_REG_VCOM2, dat);
}

void TPS65185_BP::tps_vposvneg_set(void)
{
  I2C_Write(TPS65185_ADDR, TI_REG_VADJ, 0x23);	//15V
}

uint8_t TPS65185_BP::ti_read_int_status(void)
{
  uint8_t dat;

  dat = I2C_Read(TPS65185_ADDR, TI_REG_INT2);
  return dat;
}

void TPS65185_BP::tps_read_all_reg(void)
{
  uint8_t i;
  uint8_t buff[16];


  I2C_Read_Frame(TPS65185_ADDR, TI_REG_TMST_VALUE, 16, buff);

  Debug_str("tps read reg start ...\r\n");
  for (i = 0; i < 16; i++)
  {
    Debug_str("\r\nreg:");
    Debug_hex(i);
    Debug_str("->");
    Debug_hex(buff[i]);
  }
  Debug_str("\r\ntps read reg end\r\n\r\n");
}

void TPS65185_BP::tps_sleep_to_standby(void)
{
  TPS_WAKEUP_H;
  delay(10);

  tps_power_sequence_set();
  //tps_vposvneg_set();
  tps_vcom_set(_vcom);
}

void TPS65185_BP::tps_standby_to_sleep(void)
{
  TPS_PWRCOM_L;
  TPS_PWRUP_L;
  TPS_WAKEUP_L;
}

void TPS65185_BP::tps_source_gate_enable(void)
{
  TPS_PWRUP_H;
}

void TPS65185_BP::tps_source_gate_disable(void)
{
  TPS_PWRUP_L;
}

void TPS65185_BP::tps_vcom_enable(void)
{
  TPS_PWRCOM_H;
}

void TPS65185_BP::tps_vcom_disable(void)
{
  TPS_PWRCOM_L;
}

void TPS65185_BP::tps_init(uint16_t vcom, Stream* pDiagnosticOutput)
{
  _vcom = vcom;
  _pDiagnosticOutput = pDiagnosticOutput;
  TPS_WAKEUP_L;
  TPS_PWRCOM_L;
  TPS_PWRUP_L;
  delay(10);
  TPS_WAKEUP_H;
  delay(10);

  Wire.begin();

  tps_power_sequence_set();
  //tps_vposvneg_set();
  tps_vcom_set(vcom);

  tps_read_all_reg();
}

void TPS65185_BP::tps_end()
{
  //Wire.end(); // missing
  Wire.flush();
}

void TPS65185_BP::Debug_str(const char *s)
{
  if (_pDiagnosticOutput) _pDiagnosticOutput->print(s);
}

void TPS65185_BP::Debug_hex(unsigned int dat)
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

void TPS65185_BP::Debug_dec(unsigned int dat)
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

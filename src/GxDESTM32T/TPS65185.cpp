// Class TPS65185 : display IO component class for GDE06BA on DESTM32-T parallel interface e-paper display from Dalian Good Display Inc.
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

#include "TPS65185.h"
#include "DESTM32L1_board.h"

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

#define TPS65185_ADDR		0xd0

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


#define I2C_NACK			0
#define I2C_M2S_ACK			1
#define I2C_S2M_ACK			2

//void i2c_delayus(void)
//{
//	uint32_t nCount=5;
//    //uint16_t nCount = 2;
//
//  	for(; nCount != 0; nCount--);
//}

// with STM32 package for Arduino need to inhibit loop optimization for timing

static uint32_t t_delay;

void TPS65185::i2c_delay_x(uint32_t nCount)
{
  for (; nCount != 0; nCount--)
  {
    t_delay += nCount;
  }
}

void TPS65185::i2c_delayus(void)
{
  i2c_delay_x(8); // got 1.125 us with value 8
}

void TPS65185::i2c_delay500ns(void)
{
  i2c_delay_x(2); // got 937ns with value 2
}

void TPS65185::i2c_test_period()
{
  for (uint32_t i = 0; i < 100; i++)
  {
    TPS_SCL_H;
    i2c_delayus();
    TPS_SCL_L;
    i2c_delay500ns();
  }
}


void TPS65185::i2c_start(void)
{
  TPS_SDA_H;
  TPS_SCL_H;
  i2c_delayus();
  i2c_delayus();
  TPS_SDA_L;
  i2c_delayus();
  TPS_SCL_L;
  i2c_delayus();
}

void TPS65185::i2c_stop(void)
{
  TPS_SDA_L;
  TPS_SCL_H;
  i2c_delayus();
  TPS_SDA_H;
  i2c_delayus();
  i2c_delayus();
}

bool TPS65185::i2c_ack(void)
{
  unsigned char i = 0;
  bool ret = false;

  TPS_SDA_H;
  i2c_delayus();
  i2c_delayus();
  TPS_SCL_H;
  i2c_delayus();
  while (1)
  {
    if (TPS_SDA == 0)
    {
      ret = true;
      break;
    }
    if (i > 200)	//��ʱ�˳�
    {
      Debug_str("NoAck\n");
      break;
    }
    i++;
  }
  TPS_SCL_L;
  i2c_delayus();
  return ret;
}

void TPS65185::i2c_sendack(void)
{

  TPS_SDA_L;
  TPS_SCL_H;
  i2c_delayus();
  TPS_SCL_L;
  TPS_SDA_H;
}

void TPS65185::i2c_send_byte(unsigned char sdata)
{
  unsigned char i;
  unsigned char temp = 0x80;
  for (i = 0; i < 8; i++, temp >>= 1)
  {
    if (sdata & temp)
      TPS_SDA_H;
    else
      TPS_SDA_L;
    TPS_SCL_H;
    i2c_delayus();
    TPS_SCL_L;
    i2c_delay500ns();
  }
}

unsigned char TPS65185::i2c_read_byte(void)
{
  unsigned char usData = 0;
  unsigned char i = 0;

  TPS_SDA_H;
  for (i = 0; i < 8; i++)
  {
    TPS_SCL_H;
    usData = usData << 1;
    i2c_delayus();

    if (TPS_SDA)
      usData |= 0x01;

    TPS_SCL_L;
  }

  return usData;
}

void TPS65185::I2C_Write(unsigned char dev_addr, unsigned char reg_addr, unsigned char dat)
{
  //Serial1.println("I2C_Write");
  i2c_start();
  i2c_send_byte(dev_addr);
  i2c_ack();
  i2c_send_byte(reg_addr);
  i2c_ack();
  i2c_send_byte(dat);
  i2c_ack();
  i2c_stop();
  //Serial1.println("I2C_Write done");
}

unsigned char TPS65185::I2C_Read(unsigned char dev_addr, unsigned char reg_addr)
{
  unsigned char dat;

  i2c_start();
  i2c_send_byte(dev_addr);
  i2c_ack();
  i2c_send_byte(reg_addr);
  i2c_ack();

  i2c_start();
  i2c_send_byte(dev_addr | 0x01);
  i2c_ack();
  dat = i2c_read_byte();
  //i2c_sendack();
  i2c_stop();

  return dat;
}


void TPS65185::I2C_Write_Frame(unsigned char dev_addr, unsigned char reg_addr, unsigned char length, unsigned char* dat)
{
  unsigned char i;

  i2c_start();
  i2c_send_byte(dev_addr);
  i2c_ack();
  i2c_send_byte(reg_addr);
  i2c_ack();
  for (i = 0; i < length; i++)
  {
    i2c_send_byte(*(dat + i));
    i2c_ack();
  }
  i2c_stop();
}


void TPS65185::I2C_Read_Frame(unsigned char dev_addr, unsigned char reg_addr, unsigned char length, unsigned char* dat)
{
  unsigned char i;

  i2c_start();
  i2c_send_byte(dev_addr);
  i2c_ack();
  i2c_send_byte(reg_addr);
  i2c_ack();

  i2c_start();
  i2c_send_byte(dev_addr | 0x01);
  i2c_ack();
  for (i = 0; i < length; i++)
  {
    (*(dat + i)) = i2c_read_byte();
    if (i != (length - 1))
      i2c_sendack();
  }
  i2c_stop();
}

//�ϵ�˳��
void TPS65185::tps_power_sequence_set(void)
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

//VCOM˳��
void TPS65185::tps_vcom_set(uint16_t vcom)
{
  uint8_t dat;

  dat = ((vcom / 10) & 0x00FF);
  I2C_Write(TPS65185_ADDR, TI_REG_VCOM1, dat);

  dat = (((vcom / 10) >> 8) & 0x01);
  I2C_Write(TPS65185_ADDR, TI_REG_VCOM2, dat);
}

//Source��ѹ����
void TPS65185::tps_vposvneg_set(void)
{
  I2C_Write(TPS65185_ADDR, TI_REG_VADJ, 0x23);	//15V
}

//��ȡ��ѹ״̬
uint8_t TPS65185::ti_read_int_status(void)
{
  uint8_t dat;

  dat = I2C_Read(TPS65185_ADDR, TI_REG_INT2);
  return dat;
}

//�鿴���мĴ���ֵ
void TPS65185::tps_read_all_reg(void)
{
  uint8_t i;
  uint8_t buff[16];


  I2C_Read_Frame(TPS65185_ADDR, TI_REG_TMST_VALUE, 16, buff);

  Debug_str("tps read reg start ...\r\n");
  for (i = 0; i < 17; i++)
  {
    Debug_str("\r\nreg:");
    Debug_hex(i);
    Debug_str("->");
    Debug_hex(buff[i]);
  }
  Debug_str("\r\ntps read reg end\r\n\r\n");
}

void TPS65185::tps_sleep_to_standby(void)
{
  TPS_WAKEUP_H;
  delay(10);

  tps_power_sequence_set();
  //tps_vposvneg_set();
  tps_vcom_set(2000);			//VCOM����Ϊ-2.00V
}

void TPS65185::tps_standby_to_sleep(void)
{
  TPS_PWRCOM_L;
  TPS_PWRUP_L;
  TPS_WAKEUP_L;
}

void TPS65185::tps_source_gate_enable(void)
{
  TPS_PWRUP_H;
}

void TPS65185::tps_source_gate_disable(void)
{
  TPS_PWRUP_L;
}

void TPS65185::tps_vcom_enable(void)
{
  TPS_PWRCOM_H;
}

void TPS65185::tps_vcom_disable(void)
{
  TPS_PWRCOM_L;
}


//��ʼ��
void TPS65185::tps_init(Stream* pDiagnosticOutput)
{
  _pDiagnosticOutput = pDiagnosticOutput;
  TPS_WAKEUP_L;
  TPS_SDA_H;
  TPS_SCL_H;
  TPS_PWRCOM_L;
  TPS_PWRUP_L;
  delay(10);
  TPS_WAKEUP_H; \
  delay(10);

  //i2c_test_period();

  tps_power_sequence_set();
  //tps_vposvneg_set();
  tps_vcom_set(2000);			//VCOM����Ϊ-2.00V

  tps_read_all_reg();
}



//VCOM �Զ�����
#if 0
uint16_t tps_vcom_measure(void)
{
  uint16_t i, value, value_sum;
  uint8_t reg_vcom2_old, reg_vcom2_cur;


  value_sum = 0;
  reg_vcom2_old = I2C_Read(TPS65185_ADDR, TI_REG_VCOM2);

  reg_vcom2_cur = reg_vcom2_old | 0x20;			//VCOM pin is in HIZ state
  reg_vcom2_cur = reg_vcom2_cur | 0x18;			//AVG->8x(00:12.4ms  01:22ms  10:41ms  11:82ms)
  I2C_Write(TPS65185_ADDR, TI_REG_VCOM2, reg_vcom2_cur);

  for (i = 0; i < 8; i++)								//����8���
  {
    //���������ҪԼ82ms
    I2C_Write(TPS65185_ADDR, TI_REG_VCOM2, (reg_vcom2_cur | 0x80));	//start A/D conversion
    delay(10);

    while (1)
    {
      if (I2C_Read(TPS65185_ADDR, TI_REG_INT1) & 0x02)				//wait for ACQC interrupt
        break;
    }

    value = I2C_Read(TPS65185_ADDR, TI_REG_VCOM2);
    value = (value << 8) & 0x0100;
    value += I2C_Read(TPS65185_ADDR, TI_REG_VCOM1);

    value_sum += value;
  }

  I2C_Write(TPS65185_ADDR, TI_REG_VCOM2, reg_vcom2_old);

  value = value_sum / 8;
  return value;
}
#endif

void TPS65185::Debug_str(const char *s)
{
  if (_pDiagnosticOutput) _pDiagnosticOutput->print(s);
}

void TPS65185::Debug_hex(unsigned int dat)
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

void TPS65185::Debug_dec(unsigned int dat)
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


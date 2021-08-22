/**
	******************************************************************************
	* Copyright (C), 2021 -2023, 01studio Tech. Co., Ltd.http://bbs.01studio.org/
	* File Name 				 :	lcd43g.c
	* Author						 :	Folktale
	* Version 					 :	v1.0
	* date							 :	2021/3/18
	* Description 			 :	
	******************************************************************************
**/

#include "py/runtime.h"
#include "py/mphal.h"
#include "softtimer.h"
#include "bufhelper.h"

#include <math.h>

#include "py/builtin.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "i2c.h"

#include "py/obj.h"

#include "extmod/vfs.h"
#include "extmod/vfs_fat.h"
#include "extmod/vfs_lfs.h"

#include "pin.h"
#include "pin_static_af.h"
#include "systick.h"
#include "font.h"


#if (MICROPY_ENABLE_TFTLCD && MICROPY_HW_LCD43G)
	
#include "HAL_conf.h"

#include "lcd43g.h"

#if MICROPY_PY_PICLIB
#include "piclib.h" 
#include "global.h" 
#endif

_lcd_dev lcddev;
//==============================================================================

//==============================================================================
static void LCD_WriteByteSPI(unsigned char byte)
{
	unsigned char n;
	for(n=0; n<8; n++)			
	{  
		if(byte&0x80) 
			mp_hal_pin_high(MICROPY_HW_SPI_SDA);
		else 
			mp_hal_pin_low(MICROPY_HW_SPI_SDA);
		byte<<= 1;
		mp_hal_pin_low(MICROPY_HW_SPI_CLK);
		mp_hal_pin_high(MICROPY_HW_SPI_CLK);
	}
}
static void SPI_WriteComm(uint16_t CMD)//3线9bit 串行接口
{			
	mp_hal_pin_low(MICROPY_HW_SPI_CS);
	mp_hal_pin_low(MICROPY_HW_SPI_SDA);
	mp_hal_pin_low(MICROPY_HW_SPI_CLK);
	mp_hal_pin_high(MICROPY_HW_SPI_CLK);
	LCD_WriteByteSPI(CMD);
	mp_hal_pin_high(MICROPY_HW_SPI_CS);
}
static void SPI_WriteData(uint16_t tem_data)
{			
	mp_hal_pin_low(MICROPY_HW_SPI_CS);
	mp_hal_pin_high(MICROPY_HW_SPI_SDA);
	mp_hal_pin_low(MICROPY_HW_SPI_CLK);
	mp_hal_pin_high(MICROPY_HW_SPI_CLK);
	LCD_WriteByteSPI(tem_data);
	mp_hal_pin_high(MICROPY_HW_SPI_CS);
}
void LCD_Reset(void)
{
	mp_hal_pin_low(MICROPY_HW_LTDC_RST);
	mp_hal_delay_ms(100);					   
	mp_hal_pin_high(MICROPY_HW_LTDC_RST);	 	 
	mp_hal_delay_ms(100);
}
static void LCD_init_code(void)//液晶屏初始化代码
{
	
	LCD_Reset();
	mp_hal_pin_high(MICROPY_HW_SPI_CS);	
	mp_hal_delay_ms(200);
	mp_hal_pin_low(MICROPY_HW_SPI_CS);	
	
	
#if 0
 SPI_WriteComm(0xc0);  
 SPI_WriteData(0x01);
 SPI_WriteData(0x11);
 
 SPI_WriteComm(0x20);  

 SPI_WriteComm(0x36);  
 //SPI_WriteData(0x88);//BRG
 SPI_WriteData(0x80);//RGB

 SPI_WriteComm(0x3a);  
 SPI_WriteData(0x77);//16/18/24bit
 
 SPI_WriteComm(0x35);  
 SPI_WriteData(0x00);
  
 SPI_WriteComm(0xb1);  
 SPI_WriteData(0x06);
 SPI_WriteData(0x03);
 SPI_WriteData(0x00);

 SPI_WriteComm(0xb2);            
 SPI_WriteData(0x00);
 SPI_WriteData(0xc8);

 SPI_WriteComm(0xb3);            
 SPI_WriteData(0x01);
 
 SPI_WriteComm(0xb4);            
 SPI_WriteData(0x04);
 
 SPI_WriteComm(0xb5);            
 SPI_WriteData(0x10);
 SPI_WriteData(0x30);
 SPI_WriteData(0x30);
 SPI_WriteData(0x00);
 SPI_WriteData(0x00);
 
 SPI_WriteComm(0xb6);  //
 SPI_WriteData(0x0b);  //0b
 SPI_WriteData(0x0f);
 SPI_WriteData(0x3c);
 SPI_WriteData(0x13);
 SPI_WriteData(0x13);
 SPI_WriteData(0xe8);
 
 SPI_WriteComm(0xb7);  
 SPI_WriteData(0x46);
 SPI_WriteData(0x06);
 SPI_WriteData(0x0c);
 SPI_WriteData(0x00);
 SPI_WriteData(0x00);
 
SPI_WriteComm(0xc0); //Internal Oscillator Setting 
 SPI_WriteData(0x01);
 SPI_WriteData(0x15);
 
 SPI_WriteComm(0xc3); //Power Control 3 
 SPI_WriteData(0x07);
 SPI_WriteData(0x03);
 SPI_WriteData(0x04);
 SPI_WriteData(0x04);
 SPI_WriteData(0x04);
 mp_hal_delay_ms(40);

 SPI_WriteComm(0xc4); //Power Control 4 
 SPI_WriteData(0x12);//11
 SPI_WriteData(0x24);//23
 SPI_WriteData(0x12);//12   16 
 SPI_WriteData(0x12);//12   16
 SPI_WriteData(0x02);//05   
 SPI_WriteData(0x6b);//6d  49   //6A
 mp_hal_delay_ms(20);

 SPI_WriteComm(0xc5); //Power Control 5 
 SPI_WriteData(0x69);  //69
 mp_hal_delay_ms(10);

 SPI_WriteComm(0xc6); //Power Control 6  
 SPI_WriteData(0x41);//41 40
 SPI_WriteData(0x63);
 mp_hal_delay_ms(10);

 SPI_WriteComm(0xd0); //Positive Gamma Curve for Red 
 SPI_WriteData(0x01);
 SPI_WriteData(0x26);
 SPI_WriteData(0x71);
 SPI_WriteData(0x16);
 SPI_WriteData(0x04);
 SPI_WriteData(0x03);
 SPI_WriteData(0x51);
 SPI_WriteData(0x15);
 SPI_WriteData(0x04);

 SPI_WriteComm(0xd1); //Negative Gamma Curve for Red 
 SPI_WriteData(0x01);
 SPI_WriteData(0x26);
 SPI_WriteData(0x71);
 SPI_WriteData(0x16);
 SPI_WriteData(0x04);
 SPI_WriteData(0x03);
 SPI_WriteData(0x51);
 SPI_WriteData(0x15);
 SPI_WriteData(0x04);

 SPI_WriteComm(0xd2); //Positive Gamma Curve for Green 
 SPI_WriteData(0x01);
 SPI_WriteData(0x26);
 SPI_WriteData(0x71);
 SPI_WriteData(0x16);
 SPI_WriteData(0x04);
 SPI_WriteData(0x03);
 SPI_WriteData(0x51);
 SPI_WriteData(0x15);
 SPI_WriteData(0x04);
 
 SPI_WriteComm(0xd3); //Negative Gamma Curve for Green 
 SPI_WriteData(0x01);
 SPI_WriteData(0x26);
 SPI_WriteData(0x71);
 SPI_WriteData(0x16);
 SPI_WriteData(0x04);
 SPI_WriteData(0x03);
 SPI_WriteData(0x51);
 SPI_WriteData(0x15);
 SPI_WriteData(0x04);
 
 SPI_WriteComm(0xd4);//Positive Gamma Curve for Blue  
 SPI_WriteData(0x01);
 SPI_WriteData(0x26);
 SPI_WriteData(0x71);
 SPI_WriteData(0x16);
 SPI_WriteData(0x04);
 SPI_WriteData(0x03);
 SPI_WriteData(0x51);
 SPI_WriteData(0x15);
 SPI_WriteData(0x04);

 SPI_WriteComm(0xd5);//Negative Gamma Curve for Blue  
 SPI_WriteData(0x01);
 SPI_WriteData(0x26);
 SPI_WriteData(0x71);
 SPI_WriteData(0x16);
 SPI_WriteData(0x04);
 SPI_WriteData(0x03);
 SPI_WriteData(0x51);
 SPI_WriteData(0x15);
 SPI_WriteData(0x04);
 
 SPI_WriteComm(0x11); //Sleep Out 
 mp_hal_delay_ms(20);

 SPI_WriteComm(0x29);//Display On	
	mp_hal_delay_ms(10);
#else

 //****************************************************************************//
//****************************** Page 1 Command ******************************//
//****************************************************************************//
SPI_WriteComm(0xFF); // Change to Page 1
SPI_WriteData(0xFF); 
SPI_WriteData(0x98); 
SPI_WriteData(0x06);
SPI_WriteData(0x04); 
SPI_WriteData(0x01);

SPI_WriteComm(0X08);SPI_WriteData(0X10);                 // output SDA

SPI_WriteComm(0X21);SPI_WriteData(0X01);                 // DE = 1 Active

SPI_WriteComm(0X30);SPI_WriteData(0X01);                 // 480 X 854

SPI_WriteComm(0X31);SPI_WriteData(0X02);                 // 2-dot Inversion

SPI_WriteComm(0X40);SPI_WriteData(0X18);                // DDVDH/L BT 17 -10 5.94 -5.47 -2.3 1.6 43.4 21.88 0.17

SPI_WriteComm(0X41);SPI_WriteData(0X66);              // DDVDH/L CLAMP 

SPI_WriteComm(0X42);SPI_WriteData(0X02);                 // VGH/VGL 

SPI_WriteComm(0X43);SPI_WriteData(0X0A);                 // VGH/VGL 

SPI_WriteComm(0X50);SPI_WriteData(0X78);                 // VGMP 4.6 

SPI_WriteComm(0X51);SPI_WriteData(0X78);                 // VGMN 4.6 

SPI_WriteComm(0X52);SPI_WriteData(0X00);                 // Flicker 

SPI_WriteComm(0X53);SPI_WriteData(0X1F);                 // Flicker 

SPI_WriteComm(0X57);SPI_WriteData(0X50);

SPI_WriteComm(0X60);SPI_WriteData(0X07);                 // SDTI

SPI_WriteComm(0X61);SPI_WriteData(0X01);                 // CRTI 00

SPI_WriteComm(0X62);SPI_WriteData(0X07);                 // EQTI 07

SPI_WriteComm(0X63);SPI_WriteData(0X00);                 // PCTI  00


////++++++++++++++++++ Gamma Setting ++++++++++++++++++//
//SPI_WriteComm(0xFF); // Change to Page 1
//SPI_WriteData(0xFF); 
//SPI_WriteData(0x98); 
//SPI_WriteData(0x06);
//SPI_WriteData(0x04); 
//SPI_WriteData(0x01);

SPI_WriteComm(0XA0);SPI_WriteData(0X00);  // Gamma 0 

SPI_WriteComm(0XA1);SPI_WriteData(0X0A);  // Gamma 4 

SPI_WriteComm(0XA2);SPI_WriteData(0X12);  // Gamma 8

SPI_WriteComm(0XA3);SPI_WriteData(0X0D);  // Gamma 16

SPI_WriteComm(0XA4);SPI_WriteData(0X07);  // Gamma 24

SPI_WriteComm(0XA5);SPI_WriteData(0X09);  // Gamma 52

SPI_WriteComm(0XA6);SPI_WriteData(0X06); // Gamma 80

SPI_WriteComm(0XA7);SPI_WriteData(0X03);  // Gamma 108

SPI_WriteComm(0XA8);SPI_WriteData(0X09);  // Gamma 147

SPI_WriteComm(0XA9);SPI_WriteData(0X0D);  // Gamma 175

SPI_WriteComm(0XAA);SPI_WriteData(0X15);  // Gamma 203

SPI_WriteComm(0XAB);SPI_WriteData(0X07);  // Gamma 231

SPI_WriteComm(0XAC);SPI_WriteData(0X0D);  // Gamma 239

SPI_WriteComm(0XAD);SPI_WriteData(0X0D);  // Gamma 247

SPI_WriteComm(0XAE);SPI_WriteData(0X06);  // Gamma 251

SPI_WriteComm(0XAF);SPI_WriteData(0X00);  // Gamma 255

///==============Nagitive
SPI_WriteComm(0XC0);SPI_WriteData(0X00);  // Gamma 0 

SPI_WriteComm(0XC1);SPI_WriteData(0X0A);  // Gamma 4

SPI_WriteComm(0XC2);SPI_WriteData(0X12);  // Gamma 8

SPI_WriteComm(0XC3);SPI_WriteData(0X0E);  // Gamma 16

SPI_WriteComm(0XC4);SPI_WriteData(0X07);  // Gamma 24

SPI_WriteComm(0XC5);SPI_WriteData(0X09);  // Gamma 52

SPI_WriteComm(0XC6);SPI_WriteData(0X07);  // Gamma 80

SPI_WriteComm(0XC7);SPI_WriteData(0X03);  // Gamma 108

SPI_WriteComm(0XC8);SPI_WriteData(0X09);  // Gamma 147

SPI_WriteComm(0XC9);SPI_WriteData(0X0C);  // Gamma 175

SPI_WriteComm(0XCA);SPI_WriteData(0X15);  // Gamma 203

SPI_WriteComm(0XCB);SPI_WriteData(0X07);  // Gamma 231

SPI_WriteComm(0XCC);SPI_WriteData(0X0C);  // Gamma 239

SPI_WriteComm(0XCD);SPI_WriteData(0X0D); // Gamma 247

SPI_WriteComm(0XCE);SPI_WriteData(0X06); // Gamma 251

SPI_WriteComm(0XCF);SPI_WriteData(0X00);  // Gamma 255


//****************************************************************************//
//****************************** Page 6 Command ******************************//
//****************************************************************************//
SPI_WriteComm(0xFF); // Change to Page 6
SPI_WriteData(0xFF); 
SPI_WriteData(0x98); 
SPI_WriteData(0x06);
SPI_WriteData(0x04); 
SPI_WriteData(0x06);

SPI_WriteComm(0X00);SPI_WriteData(0X21);

SPI_WriteComm(0X01);SPI_WriteData(0X06);

SPI_WriteComm(0X02);SPI_WriteData(0XA0 );   

SPI_WriteComm(0X03);SPI_WriteData(0X02);

SPI_WriteComm(0X04);SPI_WriteData(0X01);

SPI_WriteComm(0X05);SPI_WriteData(0X01);

SPI_WriteComm(0X06);SPI_WriteData(0X80);    

SPI_WriteComm(0X07);SPI_WriteData(0X04);

SPI_WriteComm(0X08);SPI_WriteData(0X00);

SPI_WriteComm(0X09);SPI_WriteData(0X80);  

SPI_WriteComm(0X0A);SPI_WriteData(0X00);   

SPI_WriteComm(0X0B);SPI_WriteData(0X00);    

SPI_WriteComm(0X0C);SPI_WriteData(0X2c);

SPI_WriteComm(0X0D);SPI_WriteData(0X2c);

SPI_WriteComm(0X0E);SPI_WriteData(0X1c);

SPI_WriteComm(0X0F);SPI_WriteData(0X00);

SPI_WriteComm(0X10);SPI_WriteData(0XFF);

SPI_WriteComm(0X11);SPI_WriteData(0XF0);

SPI_WriteComm(0X12);SPI_WriteData(0X00);

SPI_WriteComm(0X13);SPI_WriteData(0X00);

SPI_WriteComm(0X14);SPI_WriteData(0X00);

SPI_WriteComm(0X15);SPI_WriteData(0XC0);

SPI_WriteComm(0X16);SPI_WriteData(0X08);

SPI_WriteComm(0X17);SPI_WriteData(0X00);

SPI_WriteComm(0X18);SPI_WriteData(0X00);

SPI_WriteComm(0X19);SPI_WriteData(0X00);

SPI_WriteComm(0X1A);SPI_WriteData(0X00);

SPI_WriteComm(0X1B);SPI_WriteData(0X00);

SPI_WriteComm(0X1C);SPI_WriteData(0X00);

SPI_WriteComm(0X1D);SPI_WriteData(0X00);


SPI_WriteComm(0X20);SPI_WriteData(0X01);

SPI_WriteComm(0X21);SPI_WriteData(0X23);

SPI_WriteComm(0X22);SPI_WriteData(0X45);

SPI_WriteComm(0X23);SPI_WriteData(0X67);

SPI_WriteComm(0X24);SPI_WriteData(0X01);

SPI_WriteComm(0X25);SPI_WriteData(0X23);

SPI_WriteComm(0X26);SPI_WriteData(0X45);

SPI_WriteComm(0X27);SPI_WriteData(0X67);


SPI_WriteComm(0X30);SPI_WriteData(0X12);

SPI_WriteComm(0X31);SPI_WriteData(0X22);

SPI_WriteComm(0X32);SPI_WriteData(0X22);

SPI_WriteComm(0X33);SPI_WriteData(0X22);

SPI_WriteComm(0X34);SPI_WriteData(0X87);

SPI_WriteComm(0X35);SPI_WriteData(0X96);

SPI_WriteComm(0X36);SPI_WriteData(0XAA);

SPI_WriteComm(0X37);SPI_WriteData(0XDB);

SPI_WriteComm(0X38);SPI_WriteData(0XCC);

SPI_WriteComm(0X39);SPI_WriteData(0XBD);

SPI_WriteComm(0X3A);SPI_WriteData(0X78);

SPI_WriteComm(0X3B);SPI_WriteData(0X69);

SPI_WriteComm(0X3C);SPI_WriteData(0X22);

SPI_WriteComm(0X3D);SPI_WriteData(0X22);

SPI_WriteComm(0X3E);SPI_WriteData(0X22);

SPI_WriteComm(0X3F);SPI_WriteData(0X22);

SPI_WriteComm(0X40);SPI_WriteData(0X22);

SPI_WriteComm(0X52);SPI_WriteData(0X10);

SPI_WriteComm(0X53);SPI_WriteData(0X10);


//SPI_WriteComm(0xFF); // Change to Page 7
//SPI_WriteData(0xFF); 
//SPI_WriteData(0x98); 
//SPI_WriteData(0x06);
//SPI_WriteData(0x04); 
//SPI_WriteData(0x07);

////SPI_WriteComm(0X17);SPI_WriteData(0X22);

//SPI_WriteComm(0X18);SPI_WriteData(0X1D);

////SPI_WriteComm(0X02);SPI_WriteData(0X77);

////SPI_WriteComm(0XE1);SPI_WriteData(0X79);

////SPI_WriteComm(0X06);SPI_WriteData(0X01);


//****************************************************************************//
SPI_WriteComm(0xFF); // Change to Page 0
SPI_WriteData(0xFF); 
SPI_WriteData(0x98); 
SPI_WriteData(0x06);
SPI_WriteData(0x04); 
SPI_WriteData(0x00);

SPI_WriteComm(0X36);SPI_WriteData(0X00);
SPI_WriteComm(0X3A);SPI_WriteData(0X77);


SPI_WriteComm(0X11);              // Sleep-Out
mp_hal_delay_ms(120);
SPI_WriteComm(0X29);               // Display On
#endif
}
//==============================================================================

static void Set_LTDC_REG(LCD_FORM_TypeDef* LCD_FORM)
{
	uint32_t aHorStart;
	uint32_t aHorEnd;
	uint32_t aVerStart;
	uint32_t aVerEnd;

	aHorStart = LCD_FORM->blkHorEnd + 1;
	aHorEnd = aHorStart + LCD_FORM->aHorLen;  
	aVerStart = LCD_FORM->blkVerEnd + 1 ;
	aVerEnd = aVerStart + LCD_FORM->aVerLen;

	LTDC->P_HOR = aHorEnd;//总宽度
	LTDC->HSYNC = (LCD_FORM->sHsyncStart <<16 )|LCD_FORM->sHsyncEnd;//水平同步信号起始和结束，位于背景色中间
	LTDC->A_HOR = (aHorStart<<16)|aHorEnd;//水平激活起始和结束
	LTDC->A_HOR_LEN = LCD_FORM->aHorLen ;//水平激活域宽度
	LTDC->BLK_HOR = (0<<16)|LCD_FORM->blkHorEnd;//背景开始和结束宽度0~激活地址	
	LTDC->P_VER =  aVerEnd;
	LTDC->VSYNC = (LCD_FORM->sVsyncStart<<16)|LCD_FORM->sVsyncEnd;
	LTDC->A_VER = (aVerStart<<16)|aVerEnd;
	LTDC->A_VER_LEN = LCD_FORM->aVerLen ;
	LTDC->BLK_VER = (0<<16)|LCD_FORM->blkVerEnd;
}

static void Set_LCD_Timing_to_LTDC(void)//设置LCD的时序到LTDC寄存器中
{
	LCD_FORM_TypeDef LCD_FORM;
	
	LTDC->OUT_EN = 0;
	LTDC->DP_ADDR0 = (uint32_t)LTDC_Buf;
	LTDC->BLK_DATA = 0xFFFF;//背景色
//  LTDC->DP_ADDR1 = (u32)(LTDC_Buf + SDRAM_RGB_OFFSET);//?????

//{}  LTDC->BLK_DATA = 0x0000;//???

	LCD_FORM.sHsyncStart = 0x2;  //水平激活起始
	LCD_FORM.sHsyncEnd = 0x3;    //水平激活结束
	LCD_FORM.aHorLen = 480-1;  //水平分辨率
	LCD_FORM.blkHorEnd = 0xf;    //水平消隐
//{}    LCD_FORM.blkHorEnd = 0x3f;    //????

	LCD_FORM.sVsyncStart = 0x2;  //垂直激活起始
	LCD_FORM.sVsyncEnd = 0x8;    //垂直激活结束
//{}    LCD_FORM.sVsyncEnd = 0x3;    //??????
//{}	LCD_FORM.aVerLen= 800-1; 	 //垂直分辨率
    LCD_FORM.aVerLen= 854 - 1; 	 //?????
	LCD_FORM.blkVerEnd = 0xf;   //垂直消隐

	Set_LTDC_REG(&LCD_FORM);
	LTDC->VI_FORMAT = 0x00;

	LTDC->POL_CTL = 0xA;
//{}	LTDC->POL_CTL = 0x3+8;
	LTDC->OUT_EN |= 0x107;
}

void lcd43g_init(void)
{
	STATIC bool init_flag = false;
	
	lcddev.type = 2;
	if(init_flag) return;

	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE,  GPIO_Pin_All , GPIO_AF_LTDC); //GPIOE所有的IO全部复用为LTDC的数据线

	mp_hal_pin_config(MICROPY_HW_LTDC_DE, MP_HAL_PIN_MODE_ALT_OUTPUT, MP_HAL_PIN_PULL_NONE, 14);
	mp_hal_pin_config(MICROPY_HW_LTDC_PCLK, MP_HAL_PIN_MODE_ALT_OUTPUT, MP_HAL_PIN_PULL_NONE, 14);
	mp_hal_pin_config(MICROPY_HW_LTDC_HSYNC, MP_HAL_PIN_MODE_ALT_OUTPUT, MP_HAL_PIN_PULL_NONE, 14);
	mp_hal_pin_config(MICROPY_HW_LTDC_VSYNC, MP_HAL_PIN_MODE_ALT_OUTPUT, MP_HAL_PIN_PULL_NONE, 14);

	mp_hal_pin_config(MICROPY_HW_SPI_SDA, MP_HAL_PIN_MODE_OUTPUT, MP_HAL_PIN_PULL_NONE, 0);

	mp_hal_pin_config(MICROPY_HW_SPI_CLK, MP_HAL_PIN_MODE_OUTPUT, MP_HAL_PIN_PULL_NONE, 0);
	mp_hal_pin_config(MICROPY_HW_SPI_CS, MP_HAL_PIN_MODE_OUTPUT, MP_HAL_PIN_PULL_NONE, 0);
	mp_hal_pin_config(MICROPY_HW_LTDC_BL, MP_HAL_PIN_MODE_OUTPUT, MP_HAL_PIN_PULL_NONE, 0);
	mp_hal_pin_config(MICROPY_HW_LTDC_RST, MP_HAL_PIN_MODE_OUTPUT, MP_HAL_PIN_PULL_NONE, 0);

	LCD_init_code();

	RCC->AHB1ENR |= (1<<31);
	RCC->CR |= 1<<28;
	RCC->PLLDCKCFGR = 0x1<<16;  //分频系数 0~3 --> 2,4,6,8
	RCC->PLLLCDCFGR = 6<<6;   	//倍频系数

	Set_LCD_Timing_to_LTDC();//设置LCD的时序到LTDC寄存器中

	init_flag = true;
	
	lcddev.width 	= X_PIXEL; //480;
	lcddev.height = Y_PIXEL; //800;
	lcddev.backcolor = BLACK;
	
	for(uint32_t i=0; i < lcddev.width * lcddev.height; i++){
		LTDC_Buf[i] = 0x0U;
	}
	mp_hal_pin_high(MICROPY_HW_LTDC_BL);

}
uint32_t rgb888tobgr888(uint32_t color)
{
	uint32_t color_r=0,color_b = 0;
	color_r = ((color & 0xFF0000U) >> 16);
	color_b = ((color & 0xFFU) << 16);
	color &= 0xFF00U;
	color = (color | color_r | color_b);
	return color;
}
uint32_t bgr2rgb(uint32_t color)
{
	uint32_t color_r=0,color_b = 0;
	color_b = ((color & 0xFF0000U) >> 16);
	color_r = ((color & 0xFFU) << 16);
	color &= 0xFF00U;
	color = (color | color_r | color_b);
	return color;
}
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint32_t color)
{	   
	if(x >= lcddev.width || y >= lcddev.height) return;
	
	if(lcddev.type == 2)
	{
		switch (lcddev.dir)
		{
			case 2:
			LTDC_Buf[((lcddev.x_pixel*lcddev.y_pixel)-(x+(y)*lcddev.x_pixel))-1] = color;
			break;
			case 3:
			LTDC_Buf[(lcddev.x_pixel*lcddev.y_pixel)-((lcddev.x_pixel*(x+1))-y)] = color;
			break;
			case 4:
			LTDC_Buf[lcddev.x_pixel*y+x] = color;
			break;
			default:
			LTDC_Buf[((lcddev.x_pixel*(x+1))-(y)) - 1] = color;
			break;
		}
	}else if(lcddev.type == 3)  //7寸
	{
		color = rgb888tobgr888(color);
		switch (lcddev.dir)
		{
			case 2:
			LTDC_Buf[((lcddev.x_pixel*(x+1))-(y)) - 1] = color;
			break;
			case 3:
			LTDC_Buf[((lcddev.x_pixel*lcddev.y_pixel)-(x+(y)*lcddev.x_pixel))-1] = color;
			break;
			case 4:
			LTDC_Buf[(lcddev.x_pixel*lcddev.y_pixel)-((lcddev.x_pixel*(x+1))-y)] = color;
			break;
			default:
			LTDC_Buf[lcddev.x_pixel*y+x] = color;
			break;
		}
	}
}	 

uint32_t LCD_ReadPoint(uint16_t x , uint16_t y)
{
	if(x >= lcddev.width || y >= lcddev.height) return 0;
	if(lcddev.type == 2)
	{
		switch (lcddev.dir)
		{
			case 2:
			return (LTDC_Buf[((lcddev.x_pixel*lcddev.y_pixel)-(x+(y)*lcddev.x_pixel))-1]);
			break;
			case 3:
			return (LTDC_Buf[(lcddev.x_pixel*lcddev.y_pixel)-((lcddev.x_pixel*(x+1))-y)]);
			break;
			case 4:
			return (LTDC_Buf[lcddev.x_pixel*y+x]);
			break;
			default:
			//lcddev.dir = 1;
			return (LTDC_Buf[((lcddev.x_pixel*(x+1))-(y)) - 1]);
			break;
		}
	}else if(lcddev.type == 3)  //7寸
	{
		uint32_t color = 0;
		switch (lcddev.dir)
		{
			case 2:
			color = LTDC_Buf[((lcddev.x_pixel*(x+1))-(y)) - 1];
			break;
			case 3:
			color = LTDC_Buf[((lcddev.x_pixel*lcddev.y_pixel)-(x+(y)*lcddev.x_pixel))-1];
			break;
			case 4:
			color = LTDC_Buf[(lcddev.x_pixel*lcddev.y_pixel)-((lcddev.x_pixel*(x+1))-y)];
			break;
			default:
			color = LTDC_Buf[lcddev.x_pixel*y+x];
			break;
		}
		color = bgr2rgb(color);
		return color;
	}

	return 0;
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD43G_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; 
	else if(delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )
	{  
		LCD_Fast_DrawPoint(uRow,uCol ,color);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 
/**
 * @breif	带颜色画圆函数
 * @param   x1,x2 —— 圆心坐标
 * @param	r —— 半径
 * @param	color —— 颜色
 * @retval	none
 */
void LCD_Draw_ColorCircle(uint16_t x, uint16_t y, uint16_t r, uint32_t color)
{
  int16_t a = 0, b = r;
  //int16_t d = 3 - (r << 1);
	uint16_t net_r = 0;

	net_r = r;
	if((x - r) < 0){
		net_r = x;
	}else if((x+r) > lcddev.width){
		net_r = lcddev.width - x;
	}
	
	if((y - net_r) < 0){
		net_r = y;
	}else if((y+net_r) > lcddev.height){
		net_r = lcddev.height - y;
	}	
	
	/* 如果圆在屏幕可见区域外，直接退出 */
	if (x - net_r < 0 || x + net_r > lcddev.width || y - net_r < 0 || y + net_r > lcddev.height) 
	{
		return;
	}
	int16_t d = 3 - (net_r << 1);
	/* 开始画圆 */
	while(a <= b){
			LCD_Fast_DrawPoint(x - b, y - a, color);
			LCD_Fast_DrawPoint(x + b, y - a, color);
			LCD_Fast_DrawPoint(x - a, y + b, color);
			LCD_Fast_DrawPoint(x - b, y - a, color);
			LCD_Fast_DrawPoint(x - a, y - b, color);
			LCD_Fast_DrawPoint(x + b, y + a, color);
			LCD_Fast_DrawPoint(x + a, y - b, color);
			LCD_Fast_DrawPoint(x + a, y + b, color);
			LCD_Fast_DrawPoint(x - b, y + a, color);
			a++;
			if(d < 0)
			d += (a<<2) + 6;
			else{
					d += 10 + ((a - b)<<2);
					b--;
			}
			LCD_Fast_DrawPoint(x + a, y + b, color);
	}
}

//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t x,y;
	uint32_t color_dat = 0;
  for(y = sy; y < ey+sy; y++)
  {
		for(x=sx; x < ex+sx; x++)	
		{
			color_dat = *color++;
			LCD_Fast_DrawPoint(x,y,((color_dat & 0xF800)<<8) | ((color_dat & 0x7E0)<<5) | ((color_dat & 0x1F)<<3));
		}
		
  }
} 

void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color)
{
  uint16_t x,y;
  for(y = sy; y <= ey; y++)
  {
		for(x=sx; x <= ex; x++)	LCD_Fast_DrawPoint(x,y,color);
  }
}
//=============================================================================
void LCD_DisplayChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode ,uint32_t color)
{  							  
  uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=((size>>3)+((size%8)?1:0))*(size>>1);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{
		if(size==24)temp=asc2_2412[num][t];
		else if(size==32)temp=asc2_3216[num][t];	
		else if(size==48)temp=asc2_4824[num][t];	
		else temp=asc2_1608[num][t];	//调用1608字体
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,color);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,lcddev.backcolor);  //back color

			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	} 
}

uint32_t lcd_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 

void LCD_displayNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint32_t color)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/lcd_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_DisplayChar(x+(size/2)*t,y,' ',size,0,color);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_DisplayChar(x+(size/2)*t,y,temp+'0',size,0,color); 
	}
} 


void LCD_DisplayStr(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p , uint32_t color)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
	while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
	{       
		if(x>=width){
			x=x0;
			y+=size;
		}
		if(y>=height)break;//退出
		LCD_DisplayChar(x,y,*p,size,0 ,color);
		x+=(size>>1);
		p++;
	}  
}
//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint32_t color)
{
	uint32_t i = lcddev.width*lcddev.height;
	if(lcddev.type == 3) color = rgb888tobgr888(color);
	while(i--){
		LTDC_Buf[i] = color;
	}
}  

//设置LCD显示方向
void LCD_Display_Dir(uint8_t dir)
{
	lcddev.dir=dir;		//竖屏

	switch (dir)
		{
		case 2:
		lcddev.width=X_PIXEL; //480;
		lcddev.height=Y_PIXEL; //800;
		break;
		case 3:
		lcddev.width=Y_PIXEL; //800;
		lcddev.height=X_PIXEL; //480;
		break;
		case 4:
		lcddev.width=X_PIXEL; //480;
		lcddev.height=Y_PIXEL; //800;
		break;
		default:
		lcddev.width=Y_PIXEL; //800;
		lcddev.height=X_PIXEL; //480;
		break;
		}
}	 

//============================================================================== 
uint32_t get_rgb(uint8_t r_color, uint8_t g_color , uint8_t b_color)
{
	uint32_t color = (((uint32_t)r_color << 16) | ((uint32_t)g_color << 8) | b_color ) ;
	
	return (uint32_t)(color & 0xFFFFFF);
}
//==============================================================================================================
typedef struct _tftlcd_lcd43g_obj_t {
    mp_obj_base_t base;
    int16_t buf[2];
} tftlcd_lcd43g_obj_t;

STATIC tftlcd_lcd43g_obj_t lcd43g_obj;
//------------------------------------------------------------------------------------------------------

STATIC void tftlcd_lcd43g_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
	mp_printf(print, "LCD43R(portrait=%d),width:%u,height:%u,X_PIXEL:%u,Y_PIXEL:%u\n",lcddev.dir,lcddev.width,lcddev.height,
	lcddev.x_pixel,lcddev.y_pixel);
}

//------------------------------------------------------------------------------------------------------
STATIC mp_obj_t tftlcd_lcd43g_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {

	enum { ARG_portrait };
	static const mp_arg_t allowed_args[] = {
			{ MP_QSTR_portrait, MP_ARG_INT, {.u_int = 1} },
	};
	
	mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
	mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

	lcddev.dir = args[ARG_portrait].u_int;
	lcddev.backcolor = BLACK;
	
	lcddev.x_pixel = X_PIXEL; //480;
	lcddev.y_pixel = Y_PIXEL; //800;
	
	lcd43g_init();
	LCD_Display_Dir(lcddev.dir);

	lcd43g_obj.base.type = &tftlcd_lcd43g_type;

	return MP_OBJ_FROM_PTR(&lcd43g_obj);
}

//------------------------------------------------------------------------------------------------------
mp_obj_t tftlcd_lcd43g_clear(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)  {
	static const mp_arg_t clear_args[] = {
			{ MP_QSTR_fillcolor,    MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
	};
	mp_arg_val_t args[MP_ARRAY_SIZE(clear_args)];
	mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(clear_args), clear_args, args);

	if(args[0].u_obj !=MP_OBJ_NULL) 
	{
		size_t len;
		mp_obj_t *params;
		mp_obj_get_array(args[0].u_obj, &len, &params);
		if(len == 3){
			lcddev.backcolor = get_rgb(mp_obj_get_int(params[0]), mp_obj_get_int(params[1]), mp_obj_get_int(params[2]));
			LCD_Clear(lcddev.backcolor);
			lcddev.clercolor = lcddev.backcolor;
		}else{
			mp_raise_ValueError(MP_ERROR_TEXT("lcd fill parameter error \nCorrect call:fill((r,g,b))"));
		}
	}

  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(tftlcd_lcd43g_clear_obj, 1, tftlcd_lcd43g_clear);
//------------------------------------------------------------------------------------------------------

mp_obj_t tftlcd_lcd43g_drawp(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
	static const mp_arg_t drawp_args[] = {
			{ MP_QSTR_x,       MP_ARG_INT, {.u_int = 0} },
			{ MP_QSTR_y,       MP_ARG_INT, {.u_int = 0} },
			{ MP_QSTR_color,    MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
	};
	mp_arg_val_t args[MP_ARRAY_SIZE(drawp_args)];
	mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(drawp_args), drawp_args, args);

	if(args[2].u_obj !=MP_OBJ_NULL) 
	{
		size_t len;
		mp_obj_t *params;
		mp_obj_get_array(args[2].u_obj, &len, &params);
		if(len == 3){
			LCD_Fast_DrawPoint(args[0].u_int,args[1].u_int ,
			get_rgb(mp_obj_get_int(params[0]), mp_obj_get_int(params[1]), mp_obj_get_int(params[2])));
		}else{
			mp_raise_ValueError(MP_ERROR_TEXT("lcd drawPixel parameter error \nCorrect call:drawPixel(x,y,(r,g,b)"));
		}
	}

  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(tftlcd_lcd43g_drawp_obj, 1, tftlcd_lcd43g_drawp);
//------------------------------------------------------------------------------------------------------

mp_obj_t tftlcd_lcd43g_drawL(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
	static const mp_arg_t drawL_args[] = {
		{ MP_QSTR_x0,        	MP_ARG_INT, {.u_int = 0} },
		{ MP_QSTR_y0,       	MP_ARG_INT, {.u_int = 0} },
		{ MP_QSTR_x1,       	MP_ARG_INT, {.u_int = 0} },
		{ MP_QSTR_y1,       	MP_ARG_INT, {.u_int = 0} },
		{ MP_QSTR_color,   		MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
	};
	mp_arg_val_t args[MP_ARRAY_SIZE(drawL_args)];
	mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(drawL_args), drawL_args, args);

	if(args[4].u_obj !=MP_OBJ_NULL) 
	{
		size_t len;
		mp_obj_t *params;
		mp_obj_get_array(args[4].u_obj, &len, &params);
		if(len == 3){
			LCD43G_DrawLine(args[0].u_int ,args[1].u_int,args[2].u_int,args[3].u_int ,
			 get_rgb(mp_obj_get_int(params[0]), mp_obj_get_int(params[1]), mp_obj_get_int(params[2])));
		}else{
			mp_raise_ValueError(MP_ERROR_TEXT("lcd drawL parameter error \nCorrect call:drawPixel(x,y,(r,g,b)"));
		}
	}
	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(tftlcd_lcd43g_drawL_obj, 4, tftlcd_lcd43g_drawL);

//------------------------------------------------------------------------------------------------------

STATIC void dwRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t border, uint32_t color)
{
	for(uint16_t i=0 ; i < border; i++ )
	{
		LCD43G_DrawLine(x,y+i,x+width,y+i,color);
		LCD43G_DrawLine(x+i,y,x+i,y+height,color);
		LCD43G_DrawLine(x,y+height-i,x+width,y+height-i,color);
		LCD43G_DrawLine(x+width-i,y,x+width-i,y+height,color);
	}
}

mp_obj_t tftlcd_lcd43g_drawRect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

  STATIC const mp_arg_t Rect_args[] = {
		{ MP_QSTR_x,        		MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_y,        		MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_width,     		MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_height,    		MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_color,    		MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    { MP_QSTR_border,    		MP_ARG_INT, {.u_int = 1} }, 
    { MP_QSTR_fillcolor,   	MP_ARG_OBJ,{.u_obj = MP_OBJ_NULL} }, 
  };

  mp_arg_val_t args[MP_ARRAY_SIZE(Rect_args)];
  mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(Rect_args), Rect_args, args);
  
  if(args[4].u_obj !=MP_OBJ_NULL) 
  {
    size_t len;
    mp_obj_t *params;
    mp_obj_get_array(args[4].u_obj, &len, &params);
    if(len == 3){
     dwRect(args[0].u_int,args[1].u_int,args[2].u_int,args[3].u_int,args[5].u_int,
          get_rgb(mp_obj_get_int(params[0]), mp_obj_get_int(params[1]), mp_obj_get_int(params[2])));
    }else{
      mp_raise_ValueError(MP_ERROR_TEXT("lcd drawRect parameter error \n"));
    }
  }

    //MP_OBJ_NULL
  if(args[6].u_obj != mp_const_none && args[6].u_obj !=MP_OBJ_NULL) 
  {
    size_t len;
    mp_obj_t *params;
    mp_obj_get_array(args[6].u_obj, &len, &params);

    if (len != 3) { // Check params len
       mp_raise_ValueError(MP_ERROR_TEXT("lcd fillcolor parameter error"));
    }
    uint32_t color=get_rgb(mp_obj_get_int(params[0]), mp_obj_get_int(params[1]), mp_obj_get_int(params[2]));
    for(uint16_t i=0 ; i <= (args[3].u_int-(args[5].u_int*2)); i++ ) 
      LCD43G_DrawLine(args[0].u_int+args[5].u_int,args[1].u_int+args[5].u_int+i,args[0].u_int+args[2].u_int-args[5].u_int,args[1].u_int+args[5].u_int+i,color);
     
  }
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(tftlcd_lcd43g_drawRect_obj, 1, tftlcd_lcd43g_drawRect);

//------------------------------------------------------------------------------------------------------

mp_obj_t tftlcd_lcd43g_drawCircle(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
  STATIC const mp_arg_t tft_allowed_args[] = {
	{ MP_QSTR_x, 			 			MP_ARG_INT, {.u_int = 0} },
	{ MP_QSTR_y, 			 			MP_ARG_INT, {.u_int = 0} },
	{ MP_QSTR_radius, 	 		MP_ARG_INT, {.u_int = 0} },
	{ MP_QSTR_color,	  		MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
	{ MP_QSTR_border, 	 		MP_ARG_INT, {.u_int = 1} }, 
	{ MP_QSTR_fillcolor,	 	MP_ARG_OBJ,	{.u_obj = MP_OBJ_NULL} }, //7

  };

  mp_arg_val_t args[MP_ARRAY_SIZE(tft_allowed_args)];
  mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(tft_allowed_args), tft_allowed_args, args);
  
  uint32_t color=0;
	
//Circlecolor
  if(args[3].u_obj !=MP_OBJ_NULL) 
  {
    size_t len;
    mp_obj_t *params;
    mp_obj_get_array(args[3].u_obj, &len, &params);
    if(len == 3){
      color = get_rgb(mp_obj_get_int(params[0]), mp_obj_get_int(params[1]), mp_obj_get_int(params[2]));
        for(uint16_t i=0; i < args[4].u_int ;i++) {
					LCD_Draw_ColorCircle(args[0].u_int, args[1].u_int, args[2].u_int-i, color);
					
        }
    }else{
      mp_raise_ValueError(MP_ERROR_TEXT("lcd color parameter error \n"));
    }
  }
//fillcolor
  if(args[5].u_obj != mp_const_none && args[5].u_obj !=MP_OBJ_NULL) 
  {
    size_t len;
    mp_obj_t *params;
    mp_obj_get_array(args[5].u_obj, &len, &params);

    if (len != 3) { // Check params len
       mp_raise_ValueError(MP_ERROR_TEXT("lcd fillcolor parameter error"));
    }
    color = get_rgb(mp_obj_get_int(params[0]), mp_obj_get_int(params[1]), mp_obj_get_int(params[2]));
    for(uint16_t i=0 ; i <= (args[2].u_int-args[4].u_int); i++ ) {
      LCD_Draw_ColorCircle(args[0].u_int,args[1].u_int,args[2].u_int-args[4].u_int-i,color);
    }
  }

  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(tftlcd_lcd43g_drawCircle_obj, 1, tftlcd_lcd43g_drawCircle);
//------------------------------------------------------------------------------------------------------
mp_obj_t tftlcd_lcd43g_printStr(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

  STATIC const mp_arg_t tft_allowed_args[] = {
		{ MP_QSTR_text,     		MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    { MP_QSTR_x,        		MP_ARG_REQUIRED |MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_y,        		MP_ARG_REQUIRED |MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_color,    		MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    { MP_QSTR_backcolor,   	MP_ARG_OBJ,{.u_obj = MP_OBJ_NULL} }, 
    { MP_QSTR_size,      		MP_ARG_INT, {.u_int = 2} },
  };

  mp_arg_val_t args[MP_ARRAY_SIZE(tft_allowed_args)];
  mp_arg_parse_all(n_args-1, pos_args+1, kw_args, MP_ARRAY_SIZE(tft_allowed_args), tft_allowed_args, args);

  uint16_t text_size = args[5].u_int;
  uint32_t color = 0;
  //color
  if(args[3].u_obj !=MP_OBJ_NULL) 
  {
    size_t len;
    mp_obj_t *params;
    mp_obj_get_array(args[3].u_obj, &len, &params);
    if(len == 3){
      color = get_rgb(mp_obj_get_int(params[0]), mp_obj_get_int(params[1]), mp_obj_get_int(params[2]));
    }else{
      mp_raise_ValueError(MP_ERROR_TEXT("printStr color parameter error \n"));
    }
  }

  if(args[4].u_obj != mp_const_none && args[4].u_obj !=MP_OBJ_NULL) 
  {
    size_t len;
    mp_obj_t *params;
    mp_obj_get_array(args[4].u_obj, &len, &params);
  
    if (len != 3) { 
       mp_raise_ValueError(MP_ERROR_TEXT("lcd backolor parameter error"));
    }
    lcddev.backcolor = get_rgb(mp_obj_get_int(params[0]), mp_obj_get_int(params[1]), mp_obj_get_int(params[2])); 
  }
//
  if(args[0].u_obj !=MP_OBJ_NULL) 
  {
    mp_buffer_info_t bufinfo;
    if (mp_obj_is_int(args[0].u_obj)) {
      mp_raise_ValueError(MP_ERROR_TEXT("lcd text parameter error"));

    } else {
        mp_get_buffer_raise(args[0].u_obj, &bufinfo, MP_BUFFER_READ);
        char *str = bufinfo.buf;

        if(text_size == 1)  text_size = 16;
        else if(text_size == 2) text_size = 24;
        else if(text_size == 3) text_size = 32;
        else if(text_size == 4) text_size = 48;
        else mp_raise_ValueError(MP_ERROR_TEXT("lcd size parameter error"));
        LCD_DisplayStr(args[1].u_int, args[2].u_int, text_size* bufinfo.len, text_size , text_size,str ,color);
    }
  }
	else{
     mp_raise_ValueError(MP_ERROR_TEXT("lcd text parameter is empty"));
  }
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(tftlcd_lcd43g_printStr_obj, 1, tftlcd_lcd43g_printStr);
//------------------------------------------------------------------------------------------------------
#if MICROPY_PY_PICLIB
//显示成功返回0，其他失败
STATIC uint8_t display_cached(FATFS *fs, uint16_t x, uint16_t y, const char *filename)
{
	uint32_t readlen = 0;
	uint8_t *databuf;    		//数据读取存 
	uint8_t *hardbuf;    		//数据读取存 
	UINT br;
	IMAGE2LCD *image2lcd;
	uint16_t display_w,display_h;
	uint32_t *d_color;
	
	FIL* f_file;
	f_file=(FIL *)m_malloc(sizeof(FIL));
	hardbuf=(uint8_t*)m_malloc(8);

	uint8_t res = f_open(fs,f_file,filename,FA_READ);
	res = f_read(f_file,hardbuf,8,&br); //读取头信息
	
	if(res == 0){
		image2lcd = (IMAGE2LCD *)hardbuf;
		display_w = image2lcd->w;
		display_h = image2lcd->h;
		readlen = display_w * 4;
		databuf=(uint8_t*)m_malloc(readlen);		//开辟readlen字节的内存区域
		
		if(databuf == NULL)
		{
			m_free(databuf);
			res = 1;
			goto error;
		}else
		{
			for(uint16_t i=0; i < display_h; i++)
			{
				f_read(f_file,(uint8_t *)databuf,readlen,&br);
				d_color = (uint32_t *)&databuf[0];
				for(uint16_t j = 0; j < display_w; j++){
					LCD_Fast_DrawPoint(x+j, y+i, *d_color);
					d_color++;
				}
			}
		}

	}
	
error:
	f_close(f_file);
	
	m_free(f_file);
	m_free(hardbuf);
	
	return res;
}
mp_obj_t tftlcd_lcd43g_Picture(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

  STATIC const mp_arg_t tft_allowed_args[] = { 
    { MP_QSTR_x,       MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_y,       MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_file,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
		
		{ MP_QSTR_cached, MP_ARG_KW_ONLY | MP_ARG_BOOL, {.u_bool = true} },
  };

  uint8_t arg_num = MP_ARRAY_SIZE(tft_allowed_args);
  mp_arg_val_t args[arg_num];
  mp_arg_parse_all(n_args-1, pos_args+1, kw_args, arg_num, tft_allowed_args, args);

	uint8_t res=0;

  if(args[2].u_obj !=MP_OBJ_NULL) 
  {
    mp_buffer_info_t bufinfo;
    if (mp_obj_is_int(args[2].u_obj)) {
      mp_raise_ValueError(MP_ERROR_TEXT("picture parameter error"));
    } 
		else 
		{
			mp_get_buffer_raise(args[2].u_obj, &bufinfo, MP_BUFFER_READ);
			
			mp_obj_t tuple[2];
			const char *file_path = mp_obj_str_get_str(get_path(bufinfo.buf ,&res));
			const char *ftype = mp_obj_str_get_str(file_type(file_path));
			 mp_vfs_mount_t *vfs = MP_STATE_VM(vfs_mount_table);
			 if(res == 1){
				 vfs = vfs->next;
			 }
			fs_user_mount_t *vfs_fat = MP_OBJ_TO_PTR(vfs->obj);
//---------------------------------------------------------------
			if(args[3].u_bool == true){
				
				uint8_t file_len = strlen(file_path);
				char *file_buf = (char *)m_malloc(file_len+7);	

				memset(file_buf, '\0', file_len+7);
				sprintf(file_buf,"%s%s",file_path,".cache");

				res = display_cached(&vfs_fat->fatfs, args[0].u_int, args[1].u_int, (const char *)file_buf);
				m_free(file_buf);
				if(!res) return mp_const_none;
			}
//---------------------------------------------------------------

			 piclib_init();
			if(strncmp(ftype,"jpg",3) == 0 || strncmp(ftype,"jpeg",4) == 0)
				{
					jpg_decode(&vfs_fat->fatfs,file_path, args[0].u_int,args[1].u_int ,1);
				}
			else if(strncmp(ftype , "bmp" , 3) == 0)
				{
					stdbmp_decode(&vfs_fat->fatfs ,file_path, args[0].u_int, args[1].u_int) ;
				}
			else
				{
					mp_raise_ValueError(MP_ERROR_TEXT("picture file type error"));
					return mp_const_none;
				}
				
			tuple[0] = mp_obj_new_int(picinfo.S_Height);
			tuple[1] = mp_obj_new_int(picinfo.S_Width);
			return mp_obj_new_tuple(2, tuple);
    }
  }
	else{
      mp_raise_ValueError(MP_ERROR_TEXT("picture parameter is empty"));
  }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(tftlcd_lcd43g_Picture_obj, 1, tftlcd_lcd43g_Picture);

// cached file
mp_obj_t tftlcd_lcd43g_CachePicture(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

  STATIC const mp_arg_t tft_allowed_args[] = { 
    { MP_QSTR_file,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
		{ MP_QSTR_path,     MP_ARG_KW_ONLY 	| MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
		{ MP_QSTR_replace, 	MP_ARG_KW_ONLY 	| MP_ARG_BOOL,{.u_bool = false} },
  };

  uint8_t arg_num = MP_ARRAY_SIZE(tft_allowed_args);
  mp_arg_val_t args[arg_num];
  mp_arg_parse_all(n_args-1, pos_args+1, kw_args, arg_num, tft_allowed_args, args);
	
	uint8_t res=0;

	char *path_buf = (char *)m_malloc(50);  //最大支持50字符
	memset(path_buf, '\0', 50);

  if(args[0].u_obj !=MP_OBJ_NULL) 
  {
    mp_buffer_info_t bufinfo;
    if (mp_obj_is_int(args[0].u_obj)) {
      mp_raise_ValueError(MP_ERROR_TEXT("CachePicture parameter error,should is .cache"));
    } 
		else 
		{
			mp_get_buffer_raise(args[0].u_obj, &bufinfo, MP_BUFFER_READ);

			const char *file_path = mp_obj_str_get_str(get_path(bufinfo.buf ,&res));
			const char *ftype = mp_obj_str_get_str(file_type(file_path));
			 mp_vfs_mount_t *vfs = MP_STATE_VM(vfs_mount_table);
			 
			 if(res == 1){
				 vfs = vfs->next;
			 }
			fs_user_mount_t *vfs_fat = MP_OBJ_TO_PTR(vfs->obj);

			//test file
			FIL		*f_file;
			f_file=(FIL *)m_malloc(sizeof(FIL));
			if(f_file == NULL){
				mp_raise_ValueError(MP_ERROR_TEXT("malloc f_file error"));
			}
			sprintf(path_buf,"%s%s",file_path,".cache");
			res = f_open(&vfs_fat->fatfs,f_file,path_buf,FA_READ);
			f_close(f_file);
			f_sync(f_file);
			if(res == FR_OK && args[2].u_bool == false){
				return mp_const_none;
			}else{
				args[2].u_bool = true;
			}

		memset(path_buf, '\0', 50);

printf("start loading->%s\r\n",file_path);

			piclib_init();
			if(strncmp(ftype,"jpg",3) == 0 || strncmp(ftype,"jpeg",4) == 0)
			{
				res = jpg_decode(&vfs_fat->fatfs,file_path, 0, 0 ,1);
				if(res){
					printf("jpg_decode err:%d\r\n",res);
					return mp_const_none;
				}
			}
			else if(strncmp(ftype , "bmp" , 3) == 0)
			{
				res = stdbmp_decode(&vfs_fat->fatfs ,file_path, 0, 0) ;
				printf("bmp_decode err:%d\r\n",res);
				if(res)return mp_const_none;
			}
			else
			{
				mp_raise_ValueError(MP_ERROR_TEXT("picture file type error"));
				return mp_const_none;
			}
//-----------------------------------------------------------
			if(args[1].u_obj !=MP_OBJ_NULL)
			{
				mp_get_buffer_raise(args[1].u_obj, &bufinfo, MP_BUFFER_READ);
				const char *path = mp_obj_str_get_str(get_path(bufinfo.buf ,&res));
				const char *path_ftype = mp_obj_str_get_str(file_type(path));

				if(strncmp(path_ftype , "cache" , 5))
				{
					mp_raise_ValueError(MP_ERROR_TEXT("CachePicture path file type error"));
					return mp_const_none;
				}
				sprintf(path_buf,"%s",path);
			}else
			{
				sprintf(path_buf,"%s%s",file_path,".cache");
			}
//------------------------------------------------
			UINT bw;
			//FIL		*f_file;
			uint16_t display_w,display_h;
			uint32_t *r_buf;    		//数据读取存 
			uint16_t i=0,j = 0;
			uint8_t bar = 0;
			uint8_t last_bar = 0;
			// f_file=(FIL *)m_malloc(sizeof(FIL));
			// if(f_file == NULL){
				// mp_raise_ValueError(MP_ERROR_TEXT("malloc f_file error"));
			// }
			res = f_open(&vfs_fat->fatfs,f_file,path_buf,FA_READ);
			f_close(f_file);
			
			f_sync(f_file);
			
			if(args[2].u_bool == true || res != 0)
			{
				uint8_t hard_buf[8] = {0x00,0x20,0x00,0x00,0x00,0x00,0x00,0xE4};
				hard_buf[2] = (uint8_t)picinfo.S_Width;
				hard_buf[3] = (uint8_t)(picinfo.S_Width >> 8);
				hard_buf[4] = (uint8_t)picinfo.S_Height;
				hard_buf[5] = (uint8_t)(picinfo.S_Height >> 8);
				
				printf("start create cache->%s\r\n",path_buf);
				printf("loading:0%%\r\n");
				
				display_w = picinfo.S_Width;
				display_h = picinfo.S_Height;
				
				r_buf = (uint32_t *)m_malloc(display_w*4);
				if(r_buf == NULL){
					mp_raise_ValueError(MP_ERROR_TEXT("malloc r_buf error"));
				}

				res = f_open(&vfs_fat->fatfs,f_file,path_buf,FA_WRITE|FA_CREATE_ALWAYS);
				if(res != FR_OK){
					mp_raise_ValueError(MP_ERROR_TEXT("path_buf open file error"));
				}
				res=f_write(f_file,hard_buf,8,&bw);
				if(res != FR_OK){
					mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("write hard error(%d)"), (int)res);
				}
				
				for(i =0; i < display_h; i++)
				{
					for(j =0; j<display_w; j++){
						r_buf[j] = LCD_ReadPoint(j , i);
					}
					res=f_write(f_file,(uint8_t *)r_buf,display_w*4,&bw);
					if(res != FR_OK){
						LCD_DisplayStr(0,0,12*17,25,24,"Cache Error!     ",0xFF0000);
						mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("write file error(%d)"), (int)res);
					}
					bar = (i*100)/display_h;
					if((bar != last_bar) && !(bar%10)) printf("loading:%d%%\r\n",bar);
					
					if(i==25){
						LCD_DisplayStr(0,0,12*17,25,24,"Image Caching:00%",0xFF0000);
					}
					if((i >= 25) && (bar != last_bar)){
						LCD_displayNum(168,0,bar,2,24,0xFF0000);
					}
					last_bar = bar;
				}

				LCD_DisplayStr(0,0,12*17,25,24,"Cache Done!      ",0xFF0000);
				printf("cache done!\r\n");
printf("Cache size:%ld\r\n",f_size(f_file));
				f_close(f_file);
				m_free(r_buf);
			}
			
			f_sync(f_file);
			m_free(f_file);
			
    }
  }
	else{
      mp_raise_ValueError(MP_ERROR_TEXT("CachePicture parameter is empty"));
  }
	m_free(path_buf);
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(tftlcd_lcd43g_CachePicture_obj, 1, tftlcd_lcd43g_CachePicture);

#endif

STATIC mp_obj_t tftlcd_lcd43g_test(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{


	return mp_const_none;
}STATIC MP_DEFINE_CONST_FUN_OBJ_KW(tftlcd_lcd43g_test_obj,0, tftlcd_lcd43g_test);

//=======================================================================================================
STATIC const mp_rom_map_elem_t tftlcd_lcd43g_locals_dict_table[] = {
	// instance methods
	{ MP_ROM_QSTR(MP_QSTR_fill), MP_ROM_PTR(&tftlcd_lcd43g_clear_obj) },
	{ MP_ROM_QSTR(MP_QSTR_drawPixel), MP_ROM_PTR(&tftlcd_lcd43g_drawp_obj) },
	{ MP_ROM_QSTR(MP_QSTR_drawLine), MP_ROM_PTR(&tftlcd_lcd43g_drawL_obj) },
	{ MP_ROM_QSTR(MP_QSTR_drawRect), MP_ROM_PTR(&tftlcd_lcd43g_drawRect_obj) },
	{ MP_ROM_QSTR(MP_QSTR_drawCircle), MP_ROM_PTR(&tftlcd_lcd43g_drawCircle_obj) },
	{ MP_ROM_QSTR(MP_QSTR_printStr), MP_ROM_PTR(&tftlcd_lcd43g_printStr_obj) },
	#if MICROPY_PY_PICLIB
	{ MP_ROM_QSTR(MP_QSTR_Picture), MP_ROM_PTR(&tftlcd_lcd43g_Picture_obj) },
	{ MP_ROM_QSTR(MP_QSTR_CachePicture), MP_ROM_PTR(&tftlcd_lcd43g_CachePicture_obj) },
	#endif
	
	{ MP_ROM_QSTR(MP_QSTR_test), MP_ROM_PTR(&tftlcd_lcd43g_test_obj) },

};

MP_DEFINE_CONST_DICT(tftlcd_lcd43g_locals_dict, tftlcd_lcd43g_locals_dict_table);

const mp_obj_type_t tftlcd_lcd43g_type = {
    { &mp_type_type },
    .name = MP_QSTR_LCD43R,
    .print = tftlcd_lcd43g_print,
    .make_new = tftlcd_lcd43g_make_new,
    .locals_dict = (mp_obj_dict_t *)&tftlcd_lcd43g_locals_dict,
};


#endif






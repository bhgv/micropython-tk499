/**
	******************************************************************************
	* This file is part of the MicroPython project, http://micropython.org/
	* Copyright (C), 2021 -2023, 01studio Tech. Co., Ltd.http://bbs.01studio.org/
	* File Name 				 :	lcd43g.h
	* Author						 :	Folktale
	* Version 					 :	v1.0
	* date							 :	2021/3/18
	* Description 			 :	
	******************************************************************************
**/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD43G_H
#define __LCD43G_H

#ifdef __cplusplus
 extern "C" {
#endif 
#include "py/obj.h"

extern const mp_obj_type_t tftlcd_lcd43g_type;
extern const mp_obj_type_t tftlcd_lcd7r_type;


mp_obj_t tftlcd_lcd43g_clear(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
mp_obj_t tftlcd_lcd43g_drawp(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
mp_obj_t tftlcd_lcd43g_drawL(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
mp_obj_t tftlcd_lcd43g_drawRect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
mp_obj_t tftlcd_lcd43g_drawCircle(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
mp_obj_t tftlcd_lcd43g_printStr(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
#if MICROPY_PY_PICLIB
mp_obj_t tftlcd_lcd43g_Picture(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
mp_obj_t tftlcd_lcd43g_CachePicture(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
#endif
/* Includes ------------------------------------------------------------------*/  

 //画笔颜色
#define WHITE         	 	0xFFFFFF
#define BLACK         	 	0x000000	  
#define BLUE         	 		0x0000FF  
#define RED           	 	0xFF0000
#define GREEN         	 	0x00FF00

#define MICROPY_HW_LTDC_DE       		(pin_B4)
#define MICROPY_HW_LTDC_PCLK       	(pin_B5)
#define MICROPY_HW_LTDC_HSYNC       (pin_B6)
#define MICROPY_HW_LTDC_VSYNC       (pin_B7)

//#define MICROPY_HW_SPI_SDA     			(pin_B0)
#define MICROPY_HW_SPI_SDA     			(pin_E0) //(pin_B10)
#define MICROPY_HW_SPI_CLK     			(pin_B9)
#define MICROPY_HW_SPI_CS      			(pin_B11)

#define MICROPY_HW_LTDC_BL       		(pin_D8)
#define MICROPY_HW_LTDC_RST       	(pin_D6)

//LCD重要参数集
typedef struct  
{		 	 
	volatile uint16_t width;			//LCD 宽度
	volatile uint16_t height;			//LCD 高度
	volatile uint16_t id;				//LCD ID
	volatile uint8_t  dir;			//横屏还是竖屏控制：1，竖屏；0，横屏。		
	volatile uint8_t  type;			//1:MCU LCD,2:4.3RGB,3:7RGB
	volatile uint32_t backcolor; //默认字体背景色
	volatile uint32_t clercolor; 	//清屏颜色
	
	volatile uint16_t x_pixel;
	volatile uint16_t y_pixel;
	
}_lcd_dev;

//解码image2lcd数据
typedef struct
{
	uint8_t scan;
	uint8_t gray;
	uint16_t w;
	uint16_t h;
	uint8_t is565;
	uint8_t rgb;
}__attribute__((packed)) IMAGE2LCD; 


//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数


#define X_PIXEL 480
#define Y_PIXEL 854 //800

__attribute__ ((aligned (256))) uint32_t LTDC_Buf[X_PIXEL*Y_PIXEL/*+10*/];


extern void lcd43g_init(void);
extern void LCD_Display_Dir(uint8_t dir);
extern void LCD_DisplayStr(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint32_t color);

extern void LCD_Clear(uint32_t color);

extern void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color);
//extern void LCD_Fill_Pic(u16 x, u16 y, u16 pic_H, u16 pic_V, u32* pic);
extern void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);
extern uint32_t LCD_ReadPoint(uint16_t x , uint16_t y);
extern void LCD_Fast_DrawPoint(uint16_t x, uint16_t y, uint32_t color);
extern void LCD43G_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);

extern uint32_t get_rgb(uint8_t r_color, uint8_t g_color , uint8_t b_color);
extern void LCD_Reset(void);
/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif /* __LCD43G_H */


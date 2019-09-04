/**
  ******************************************************************************
  * @file           : tftlcd.h
  * @brief          : Header for tftlcd.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  * @brief
  * 此头文件为ILI9225 LCD （大小为176*220 2.2寸彩屏）专用头文件，函数API已经修改为HAL库接口，
  * 适用于F767ZI-NUCLEO开发板（连接CN10）
  ******************************************************************************
  */
#ifndef _tftlcd_H
#define _tftlcd_H	

#include "stm32f7xx_hal.h"

//===========TFTLCD地址结构体==========//
typedef struct {
	uint16_t LCD_CMD;
	uint16_t LCD_DATA;
} TFTLCD_TypeDef;

//-----------------LCD端口定义----------------//

#define	LCD_LED(n) 	  (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET))		//LCD背光
#define	LCD_RD(n)	  (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET))    //读数据
#define	LCD_RS(n)	  (n?HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_RESET))    //数据/命令
#define	LCD_WR(n)	  (n?HAL_GPIO_WritePin(GPIOF,GPIO_PIN_15,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOF,GPIO_PIN_15,GPIO_PIN_RESET))    //写数据
#define	LCD_RESER(n)  (n?HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_RESET))    //复位

//--------------PA0~15,作为数据线-------------//
#define DATAOUT(x) GPIOE->ODR=x; //数据输出
#define DATAIN     GPIOE->IDR;   //数据输入

//写数据函数
/*
 #define LCD_WR_DATA(data){\
LCD_RS_SET;\
DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
}
 */

//TFTLCD重要参数集
typedef struct {
	uint16_t width;			//LCD 宽度
	uint16_t height;		//LCD 高度
	uint16_t id;			//LCD ID
	uint8_t dir;        	//LCD 方向
} _tftlcd_data;

//---------------LCD参数-----------------//
extern _tftlcd_data tftlcd_data;//管理LCD重要参数
//----------LCD的前端颜色和背景色----------//
extern uint16_t GRAY;			//前端颜色 默认红色
extern uint16_t BACK_COLOR; 	//背景颜色.默认为白色

//画笔颜色
#define WHITE         	 0xFFFF //白色
#define BLACK         	 0x0000	//黑色
#define BLUE         	 0x001F //蓝色
#define BRED             0XF81F //黑红色
#define GRED 			 0XFFE0 //灰色
#define GBLUE			 0X07FF //吉蓝
#define RED           	 0xF800 //红色
#define MAGENTA       	 0xF81F //洋红色
#define GREEN         	 0x07E0 //绿色
#define CYAN          	 0x7FFF //蓝绿色
#define YELLOW        	 0xFFE0 //黄色
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色

void LCD_WriteCmd(uint16_t cmd);                  	//写寄存器函数,cmd:寄存器值

void LCD_WriteData(uint16_t data);                	//写数据,data:要写入的值

void LCD_WriteCmdData(uint16_t cmd, uint16_t data);	//向指定的寄存器(cmd)写值（data）

void LCD_WriteData_Color(uint16_t color);         	//写颜色值,color:要写入颜色值

uint16_t LCD_ReadData(void);                      	//读数据,返回值:读到的值

void TFTLCD_Init_ILI9225(void);                     //LCD初始化

void LCD_Display_Dir(uint8_t dir);                  //设置LCD显示方向,dir:0,竖屏；1,横屏

void LCD_Set_Window(uint16_t sx, uint16_t sy,
		uint16_t width, uint16_t height); 			//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy)

void LCD_Clear(uint16_t Color);	 					//清屏,color:要清屏的填充色

void LCD_Fill(uint16_t xState, uint16_t yState, uint16_t xEnd, uint16_t yEnd,
		uint16_t color);	 						//在指定区域内填充单个颜色

void LCD_DrawPoint(uint16_t x, uint16_t y);	 		//画点

void LCD_DrawFRONT_COLOR(uint16_t x, uint16_t y, uint16_t color);	 	//快速画点

uint16_t LCD_ReadPoint(uint16_t x, uint16_t y);	 	//读取个某点的颜色值

void LCD_DrawLine(uint16_t x1, uint16_t y1,
		uint16_t x2, uint16_t y2);	 				//画线

void LCD_DrawLine_Color(uint16_t x1, uint16_t y1,
		uint16_t x2, uint16_t y2,uint16_t color);	//画线

void LCD_DrowSign(uint16_t x, uint16_t y, uint16_t color);		// 画一个十字的标记

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2,
		uint16_t y2);											//画矩形

void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);	 	//在指定位置画一个指定大小的圆

void LCD_ShowChar(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t num,
		uint8_t size, uint8_t mode);	 						//在指定位置显示一个字符

uint32_t LCD_Pow(uint8_t m, uint8_t n);	 						//m^n函数

void LCD_ShowNum(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint32_t num,
		uint8_t len, uint8_t size);	 							//显示数字,高位为0,则不显示

void LCD_ShowxNum(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc,
		uint32_t num, uint8_t len, uint8_t size, uint8_t mode);	//显示数字,高位为0,还是显示

void LCD_ShowString(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc,
		uint16_t width, uint16_t height, uint8_t size, uint8_t *p);	 	//显示字符串

void LCD_ShowFontHZ_32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc,
		uint8_t *cn);											//显示汉字

void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t wide,
		uint16_t high,const unsigned char *pic);				//图片显示

void Lcd_SetXY(uint16_t Xpos, uint16_t Ypos);

void LCD_ShowFontHZ_24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc,
		uint8_t *cn);
void LCD_ShowFontHZ_16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc,
		uint8_t *cn);
void LCD_ShowFontHZ_12(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc,
		uint8_t *cn);
void Show_Str(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,
		uint8_t size, uint8_t mode);							// 显示字符串（中英文）

#endif  


/**
  ******************************************************************************
  * @file           : tftlcd.C
  * @brief          : tftlcd.c source file.
  ******************************************************************************
  * @attention
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  * @brief
  * 此头文件为ILI9225 LCD （大小为176*220 2.2寸彩屏）专用源文件，函数API已经修改为HAL库接口，
  * 适用于F767ZI-NUCLEO开发板（连接CN10）
  ******************************************************************************
  */
#include "tftlcd.h"
#include "stdlib.h"
#include "font.h" 

//LCD的画笔颜色和背景色
uint16_t FRONT_COLOR = BLUE;	//画笔颜色
uint16_t BACK_COLOR = BLACK;  	//背景色

_tftlcd_data tftlcd_data;

void TFTLCD_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOB_CLK_ENABLE()
	;   //使能GPIOB时钟
	__HAL_RCC_GPIOE_CLK_ENABLE()
	;   //使能GPIOE时钟
	__HAL_RCC_GPIOF_CLK_ENABLE()
	;   //使能GPIOF时钟

	//-------------------GPIO初始化设置--------------------//
	GPIO_Initure.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;   	//推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;        	//上拉
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;   	//高
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);   		//初始化

	//-------------------GPIO初始化设置-------------------//
	GPIO_Initure.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;   	//推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;        	//上拉
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;   	//高速
	HAL_GPIO_Init(GPIOF, &GPIO_Initure);   		//初始化

	//PB10,11
	GPIO_Initure.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);   		//初始化

	//PE8~15
	GPIO_Initure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
			| GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);   //初始化

	//PF13~15
	GPIO_Initure.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOF, &GPIO_Initure);   //初始化

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);	//PB10置1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);	//PB11置1
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);	//PF13置1
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_SET);	//PF14置1
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, GPIO_PIN_SET);	//PF15置1
}

//============写寄存器函数================//
//------------cmd:寄存器值---------------//
void LCD_WriteCmd(uint16_t cmd) {

	LCD_RS(0);	//写地址
// 	LCD_CS_CLR; 
	DATAOUT(cmd);
	LCD_WR(0);
	LCD_WR(1);

	DATAOUT(cmd << 8);
	LCD_WR(0);
	LCD_WR(1);
// 	LCD_CS_SET; 
}

//写数据
//data:要写入的值
void LCD_WriteData(uint16_t data) {

	LCD_RS(1);
//	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR(0);
	LCD_WR(1);

	DATAOUT(data << 8);
	LCD_WR(0);
	LCD_WR(1);
//	LCD_CS_SET
}

//向指定的寄存器(cmd)写值（data）
void LCD_WriteCmdData(uint16_t cmd, uint16_t data) {
	LCD_WriteCmd(cmd);
	LCD_WriteData(data);
}

//写颜色值,color:要写入颜色值
void LCD_WriteData_Color(uint16_t color) {

	LCD_RS(1);
//	LCD_CS_CLR;
	DATAOUT(color);
	LCD_WR(0);
	LCD_WR(1);

	DATAOUT(color << 8);
	LCD_WR(0);
	LCD_WR(1);
//	LCD_CS_SET
}

//读数据
//返回值:读到的值
uint16_t LCD_ReadData(void) {
//	//return ((TFTLCD->LCD_DATA<<8)|(TFTLCD->LCD_DATA));
//	return TFTLCD->LCD_DATA;
	uint16_t t;
// 	GPIOF->CRL=0X88888888; //PF0-7  上拉输入
//	GPIOF->CRH=0X88888888; //PF8-15 上拉输入
	GPIOB->ODR = 0X00;     //PB0-7输出0

	LCD_RS(1);
//	LCD_CS(0);
	//读取数据(读寄存器时,并不需要读2次)
//	LCD_RD_CLR;

	t = DATAIN
	;
//	LCD_RD(1);
//	LCD_CS(1); 

//	GPIOF->CRL=0X33333333; //PB0-7  上拉输出
//	GPIOF->CRH=0X33333333; //PB8-15 上拉输出
	GPIOF->ODR = 0XFF;    //PB0-7输出高
	return t;

}

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(uint8_t dir) {
	if (dir == 0)  //默认竖屏方向
			{
		LCD_WriteCmd(0x22);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x36);
		tftlcd_data.height = 220;
		tftlcd_data.width = 176;
		tftlcd_data.dir = 0;
	} else {
		LCD_WriteCmd(0x22);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x38);
		tftlcd_data.height = 176;
		tftlcd_data.width = 220;
		tftlcd_data.dir = 1;
	}
}

void TFTLCD_Init_ILI9225(void) {
	TFTLCD_GPIO_Init();
	HAL_Delay(50);		// delay 50 ms
	//************* Reset LCD Driver ****************//
	LCD_RESER(1);

	HAL_Delay(1);		// Delay 1ms
	LCD_RESER(0);

	HAL_Delay(10);		// Delay 10ms            // This delay time is necessary
	LCD_RESER(1);

	HAL_Delay(50);		// Delay 50 ms

//************* Start Initial Sequence **********// 
	LCD_WriteCmdData(0x00D0, 0x0003);
	LCD_WriteCmdData(0x00EB, 0x0B00);
	LCD_WriteCmdData(0x00EC, 0x004F); //0f
	LCD_WriteCmdData(0x00C7, 0x003F); //030f

	LCD_WriteCmdData(0x0001, 0x011C);
	LCD_WriteCmdData(0x0002, 0x0100); //0100 行翻转
	LCD_WriteCmdData(0x0003, 0x1030);
	LCD_WriteCmdData(0x0007, 0x0000);
	LCD_WriteCmdData(0x0008, 0x0808);
	LCD_WriteCmdData(0x000F, 0x0A01);
	HAL_Delay(10);

	LCD_WriteCmdData(0x0010, 0x0000);
	LCD_WriteCmdData(0x0011, 0x1B41); //The register setting is suitable for VCI=2.8V
	HAL_Delay(120);

	LCD_WriteCmdData(0x0012, 0x300E); //The register setting is suitable for VCI=2.8V
	LCD_WriteCmdData(0x0013, 0x0061); //The register setting is suitable for VCI=2.8V
	LCD_WriteCmdData(0x0014, 0x5569); //The register51etting is suitable for VCI=2.8V   5569  4e65  00-7F65  vcomh 高八位调水纹！
//-------------- Set GRAM area -----------------//
	LCD_WriteCmdData(0x0030, 0x0000);
	LCD_WriteCmdData(0x0031, 0x00DB);
	LCD_WriteCmdData(0x0032, 0x0000);
	LCD_WriteCmdData(0x0033, 0x0000);
	LCD_WriteCmdData(0x0034, 0x00DB);
	LCD_WriteCmdData(0x0035, 0x0000);
	LCD_WriteCmdData(0x0036, 0x00AF);
	LCD_WriteCmdData(0x0037, 0x0000);
	LCD_WriteCmdData(0x0038, 0x00DB);
	LCD_WriteCmdData(0x0039, 0x0000);
// ----------- Adjust the Gamma Curve ----------//     
	LCD_WriteCmdData(0x0050, 0x0100);
	LCD_WriteCmdData(0x0051, 0x0609);
	LCD_WriteCmdData(0x0052, 0x0c09);
	LCD_WriteCmdData(0x0053, 0x0205);
	LCD_WriteCmdData(0x0054, 0x090c);
	LCD_WriteCmdData(0x0055, 0x0906);
	LCD_WriteCmdData(0x0056, 0x0001);
	LCD_WriteCmdData(0x0057, 0x0502);
	LCD_WriteCmdData(0x0058, 0x0003);
	LCD_WriteCmdData(0x0059, 0x0300);
	LCD_WriteCmdData(0x0020, 0x0000);
	LCD_WriteCmdData(0x0021, 0x0000);
	LCD_WriteCmdData(0x0007, 0x1017);

	LCD_Display_Dir(0);		//0：竖屏  1：横屏  默认竖屏
	LCD_Clear(BACK_COLOR);

	HAL_Delay(50);

	LCD_LED(0);
}

/*************************************************
 函数名：LCD_Set_XY
 功能：设置lcd显示起始点
 入口参数：xy坐标
 返回值：无
 *************************************************/
void Lcd_SetXY(uint16_t X, uint16_t Y) {
	if (tftlcd_data.dir == 0)  //默认竖屏方向
			{
		LCD_WriteCmdData(0x20, X);
		LCD_WriteCmdData(0x21, Y);
	} else {
		LCD_WriteCmdData(0x21, X);
		LCD_WriteCmdData(0x20, Y);
	}
	LCD_WriteCmd(0x22);
}

///*************************************************
//函数名：LCD_Set_Window
//功能：设置lcd显示区域，在此区域写点数据自动换行
//入口参数：xy起点和终点
//返回值：无
//*************************************************/
////设置显示窗口
void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height) {
	if (tftlcd_data.dir == 1)  //默认竖屏方向
			{
		LCD_WriteCmdData(0x38, width);
		LCD_WriteCmdData(0x39, sx);
		LCD_WriteCmdData(0x36, height);
		LCD_WriteCmdData(0x37, sy);
		LCD_WriteCmdData(0x21, sx);
		LCD_WriteCmdData(0x20, sy);
	} else {
		LCD_WriteCmdData(0x36, width);
		LCD_WriteCmdData(0x37, sx);
		LCD_WriteCmdData(0x38, height);
		LCD_WriteCmdData(0x39, sy);
		LCD_WriteCmdData(0x20, sx);
		LCD_WriteCmdData(0x21, sy);
	}
	LCD_WriteCmd(0x22);
}

//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint16_t color) {
	uint16_t i, j;
	LCD_Set_Window(0, 0, tftlcd_data.width - 1, tftlcd_data.height - 1);//作用区域
	for (i = 0; i < tftlcd_data.width; i++) {
		for (j = 0; j < tftlcd_data.height; j++) {
			LCD_WriteData_Color(color);
		}
	}
}

//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Fill(uint16_t xState, uint16_t yState, uint16_t xEnd, uint16_t yEnd, uint16_t color) {
	uint16_t temp;

	if ((xState > xEnd) || (yState > yEnd)) {
		return;
	}
	LCD_Set_Window(xState, yState, xEnd - 1, yEnd - 1);
	xState = xEnd - xState + 1;
	yState = yEnd - yState + 1;

	while (xState--) {
		temp = yState;
		while (temp--) {
			LCD_WriteData_Color(color);
		}
	}
}

//画点
//x,y:坐标
//FRONT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x, uint16_t y) {
	LCD_Set_Window(x, y, x, y);  //设置点的位置
	LCD_WriteData_Color(FRONT_COLOR);
}

//快速画点
//x,y:坐标
//color:颜色
void LCD_DrawFRONT_COLOR(uint16_t x, uint16_t y, uint16_t color) {
	LCD_Set_Window(x, y, x, y);
	LCD_WriteData_Color(color);
}

//读取个某点的颜色值
//x,y:坐标
//返回值:此点的颜色
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y) {
	uint16_t r = 0, g = 0, b = 0;
	if (x >= tftlcd_data.width || y >= tftlcd_data.height)
		return 0;	//超过了范围,直接返回
	LCD_Set_Window(x, y, x, y);
	LCD_WriteCmd(0X22);	//发送读GRAM指令
	r = LCD_ReadData();				//dummy Read
	r = LCD_ReadData();								//实际坐标颜色
	//printf("r=%X\r\n",r);
	g = LCD_ReadData();
	b = LCD_ReadData();
	//printf("b=%X\r\n",b);
	//printf("g=%X\r\n",g);
	return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //设置单步方向
	else if (delta_x == 0)
		incx = 0; //垂直线
	else {
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //水平线
	else {
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; //选取基本增量坐标轴
	else
		distance = delta_y;
	for (t = 0; t <= distance + 1; t++) //画线输出
			{
		LCD_DrawPoint(uRow, uCol); //画点
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance) {
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance) {
			yerr -= distance;
			uCol += incy;
		}
	}
}

void LCD_DrawLine_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //设置单步方向
	else if (delta_x == 0)
		incx = 0; //垂直线
	else {
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //水平线
	else {
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; //选取基本增量坐标轴
	else
		distance = delta_y;
	for (t = 0; t <= distance + 1; t++) //画线输出
			{
		LCD_DrawFRONT_COLOR(uRow, uCol, color); //画点
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance) {
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance) {
			yerr -= distance;
			uCol += incy;
		}
	}
}

// 画一个十字的标记
// x：标记的X坐标
// y：标记的Y坐标
// color：标记的颜色
void LCD_DrowSign(uint16_t x, uint16_t y, uint16_t color) {
	uint8_t i;
	/* 画点 */
	LCD_Set_Window(x - 1, y - 1, x + 1, y + 1);
	for (i = 0; i < 9; i++) {
		LCD_WriteData_Color(color);
	}
	/* 画竖 */
	LCD_Set_Window(x - 4, y, x + 4, y);
	for (i = 0; i < 9; i++) {
		LCD_WriteData_Color(color);
	}
	/* 画横 */
	LCD_Set_Window(x, y - 4, x, y + 4);
	for (i = 0; i < 9; i++) {
		LCD_WriteData_Color(color);
	}
}

//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	LCD_DrawLine(x1, y1, x2, y1);
	LCD_DrawLine(x1, y1, x1, y2);
	LCD_DrawLine(x1, y2, x2, y2);
	LCD_DrawLine(x2, y1, x2, y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r) {
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1);             //判断下个点位置的标志
	while (a <= b) {
		LCD_DrawPoint(x0 + a, y0 - b);             //5
		LCD_DrawPoint(x0 + b, y0 - a);             //0
		LCD_DrawPoint(x0 + b, y0 + a);             //4
		LCD_DrawPoint(x0 + a, y0 + b);             //6
		LCD_DrawPoint(x0 - a, y0 + b);             //1
		LCD_DrawPoint(x0 - b, y0 + a);
		LCD_DrawPoint(x0 - a, y0 - b);             //2
		LCD_DrawPoint(x0 - b, y0 - a);             //7
		a++;
		//使用Bresenham算法画圆
		if (di < 0)
			di += 4 * a + 6;
		else {
			di += 10 + 4 * (a - b);
			b--;
		}
	}
}

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t num, uint8_t size, uint8_t mode) {
	uint8_t temp, t1, t;
	uint16_t y0 = y;
	uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);//得到字体一个字符对应点阵集所占的字节数
	num = num - ' ';		//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for (t = 0; t < csize; t++) {
		if (size == 12)
			temp = ascii_1206[num][t]; 	 	//调用1206字体
		else if (size == 16)
			temp = ascii_1608[num][t];	//调用1608字体
		else if (size == 24)
			temp = ascii_2412[num][t];	//调用2412字体
		else
			return;								//没有的字库
		for (t1 = 0; t1 < 8; t1++) {
			if (temp & 0x80)
				LCD_DrawFRONT_COLOR(x, y, fc);
			else if (mode == 0)
				LCD_DrawFRONT_COLOR(x, y, bc);
			temp <<= 1;
			y++;
			if (y >= tftlcd_data.height)
				return;		//超区域了
			if ((y - y0) == size) {
				y = y0;
				x++;
				if (x >= tftlcd_data.width)
					return;	//超区域了
				break;
			}
		}
	}
}
//m^n函数
//返回值:m^n次方.
uint32_t LCD_Pow(uint8_t m, uint8_t n) {
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}

//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);

void LCD_ShowNum(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint32_t num, uint8_t len, uint8_t size) {
	uint8_t t, temp;
	uint8_t enshow = 0;
	for (t = 0; t < len; t++) {
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1)) {
			if (temp == 0) {
				LCD_ShowChar(x + (size / 2) * t, y, fc, bc, ' ', size, 0);
				continue;
			} else
				enshow = 1;
		}
		LCD_ShowChar(x + (size / 2) * t, y, fc, bc, temp + '0', size, 0);
	}
}

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.

void LCD_ShowxNum(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint32_t num, uint8_t len, uint8_t size,
		uint8_t mode) {
	uint8_t t, temp;
	uint8_t enshow = 0;
	for (t = 0; t < len; t++) {
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1)) {
			if (temp == 0) {
				if (mode & 0X80)
					LCD_ShowChar(x + (size / 2) * t, y, fc, bc, '0', size,
							mode & 0X01);
				else
					LCD_ShowChar(x + (size / 2) * t, y, fc, bc, ' ', size,
							mode & 0X01);
				continue;
			} else
				enshow = 1;
		}
		LCD_ShowChar(x + (size / 2) * t, y, fc, bc, temp + '0', size,
				mode & 0X01);
	}
}

//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t width, uint16_t height,
		uint8_t size, uint8_t *p) {
	uint8_t x0 = x;
	width += x;
	height += y;
	while ((*p <= '~') && (*p >= ' '))	//判断是不是非法字符!
	{
		if (x >= width) {
			x = x0;
			y += size;
		}
		if (y >= height)
			break;	//退出
		LCD_ShowChar(x, y, fc, bc, *p, size, 0);
		x += size / 2;
		p++;
	}
}

/****************************************************************************
 *函数名：LCD_ShowFontHZ
 *输  入：x：汉字显示的X坐标
 *      * y：汉字显示的Y坐标
 *      * cn：要显示的汉字
 *      * wordColor：文字的颜色
 *      * backColor：背景颜色
 *输  出：
 *功  能：写二号楷体汉字
 ****************************************************************************/
#if 0
void LCD_ShowFontHZ_32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *cn)
{  
	uint8_t i, j, wordNum;
	uint16_t color;
	while (*cn != '\0')
	{
		LCD_Set_Window(x, y, x+31, y+28);
		for (wordNum=0; wordNum<20; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar32x29[wordNum].Index[0]==*cn)
			     &&(CnChar32x29[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<116; i++) 
				{	//MSK的位数
					color=CnChar32x29[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							LCD_WriteData_Color(fc); 						
						} 						
						else
						{
							LCD_WriteData_Color(bc); 
						} 
						color<<=1;
					}//for(j=0;j<8;j++)结束
				}    
			}
		} //for (wordNum=0; wordNum<20; wordNum++)结束
		cn += 2;
		x += 32;
	}
}
#endif

#if 1
void LCD_ShowFontHZ_32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *cn) {
	uint8_t i, j, wordNum;
	uint16_t color;
	uint16_t x0 = x;
	uint16_t y0 = y;
	while (*cn != '\0') {
		for (wordNum = 0; wordNum < 20; wordNum++) {	//wordNum扫描字库的字数
			if ((CnChar32x29[wordNum].Index[0] == *cn)
					&& (CnChar32x29[wordNum].Index[1] == *(cn + 1))) {
				for (i = 0; i < 116; i++) {	//MSK的位数
					color = CnChar32x29[wordNum].Msk[i];
					for (j = 0; j < 8; j++) {
						if ((color & 0x80) == 0x80) {
							LCD_DrawFRONT_COLOR(x, y, fc);
						} else {
							LCD_DrawFRONT_COLOR(x, y, bc);
						}
						color <<= 1;
						x++;
						if ((x - x0) == 32) {
							x = x0;
							y++;
							if ((y - y0) == 29) {
								y = y0;
							}
						}
					}	//for(j=0;j<8;j++)结束
				}
			}

		} //for (wordNum=0; wordNum<20; wordNum++)结束
		cn += 2;
		x += 32;
		x0 = x;
	}
}
#endif

/****************************************************************************
 *函数名：LCD_ShowFontHZ_24
 *输  入：x：汉字显示的X坐标
 *      * y：汉字显示的Y坐标
 *      * cn：要显示的汉字
 *      * wordColor：文字的颜色
 *      * backColor：背景颜色
 *输  出：
 *功  能：写二号楷体汉字
 ****************************************************************************/
#if 0
void LCD_ShowFontHZ_24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *cn)
{  
	uint8_t i, j, wordNum;
	uint16_t color;
	while (*cn != '\0')
	{
		LCD_Set_Window(x, y, x+23, y+23);
		for (wordNum=0; wordNum<20; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar24x24[wordNum].Index[0]==*cn)
			     &&(CnChar24x24[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<72; i++) 
				{	//MSK的位数
					color=CnChar24x24[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							LCD_WriteData_Color(fc); 						
						} 						
						else
						{
							LCD_WriteData_Color(bc); 
						} 
						color<<=1;
					}//for(j=0;j<8;j++)结束
				}    
			}
		} //for (wordNum=0; wordNum<20; wordNum++)结束
		cn += 2;
		x += 24;
	}
}
#endif

#if 1
void LCD_ShowFontHZ_24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *cn) {
	uint8_t i, j, wordNum;
	uint16_t color;
	uint16_t x0 = x;
	uint16_t y0 = y;
	while (*cn != '\0') {
		for (wordNum = 0; wordNum < 20; wordNum++) {	//wordNum扫描字库的字数
			if ((CnChar24x24[wordNum].Index[0] == *cn)
					&& (CnChar24x24[wordNum].Index[1] == *(cn + 1))) {
				for (i = 0; i < 72; i++) {	//MSK的位数
					color = CnChar24x24[wordNum].Msk[i];
					for (j = 0; j < 8; j++) {
						if ((color & 0x80) == 0x80) {
							LCD_DrawFRONT_COLOR(x, y, fc);
						} else {
							LCD_DrawFRONT_COLOR(x, y, bc);
						}
						color <<= 1;
						x++;
						if ((x - x0) == 24) {
							x = x0;
							y++;
							if ((y - y0) == 24) {
								y = y0;
							}
						}
					}	//for(j=0;j<8;j++)结束
				}
			}

		} //for (wordNum=0; wordNum<20; wordNum++)结束
		cn += 2;
		x += 24;
		x0 = x;
	}
}
#endif

/****************************************************************************
 *函数名：LCD_ShowFontHZ_16
 *输  入：x：汉字显示的X坐标
 *      * y：汉字显示的Y坐标
 *      * cn：要显示的汉字
 *      * wordColor：文字的颜色
 *      * backColor：背景颜色
 *输  出：
 *功  能：写二号楷体汉字
 ****************************************************************************/
#if 0
void LCD_ShowFontHZ_16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *cn)
{  
	uint8_t i, j, wordNum;
	uint16_t color;
	while (*cn != '\0')
	{
		LCD_Set_Window(x, y, x+15, y+15);
		for (wordNum=0; wordNum<20; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar16x16[wordNum].Index[0]==*cn)
			     &&(CnChar16x16[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<32; i++) 
				{	//MSK的位数
					color=CnChar16x16[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							LCD_WriteData_Color(fc); 						
						} 						
						else
						{
							LCD_WriteData_Color(bc); 
						} 
						color<<=1;
					}//for(j=0;j<8;j++)结束
				}    
			}
		} //for (wordNum=0; wordNum<20; wordNum++)结束
		cn += 2;
		x += 16;
	}
}
#endif

#if 1
void LCD_ShowFontHZ_16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *cn) {
	uint8_t i, j, wordNum;
	uint16_t color;
	uint16_t x0 = x;
	uint16_t y0 = y;
	while (*cn != '\0') {
		for (wordNum = 0; wordNum < 20; wordNum++) {	//wordNum扫描字库的字数
			if ((CnChar16x16[wordNum].Index[0] == *cn)
					&& (CnChar16x16[wordNum].Index[1] == *(cn + 1))) {
				for (i = 0; i < 32; i++) {	//MSK的位数
					color = CnChar16x16[wordNum].Msk[i];
					for (j = 0; j < 8; j++) {
						if ((color & 0x80) == 0x80) {
							LCD_DrawFRONT_COLOR(x, y, fc);
						} else {
							LCD_DrawFRONT_COLOR(x, y, bc);
						}
						color <<= 1;
						x++;
						if ((x - x0) == 16) {
							x = x0;
							y++;
							if ((y - y0) == 16) {
								y = y0;
							}
						}
					}	//for(j=0;j<8;j++)结束
				}
			}

		} //for (wordNum=0; wordNum<20; wordNum++)结束
		cn += 2;
		x += 16;
		x0 = x;
	}
}
#endif

/****************************************************************************
 *函数名：LCD_ShowFontHZ_12
 *输  入：x：汉字显示的X坐标
 *      * y：汉字显示的Y坐标
 *      * cn：要显示的汉字
 *      * wordColor：文字的颜色
 *      * backColor：背景颜色
 *输  出：
 *功  能：写二号楷体汉字
 ****************************************************************************/
#if 0
void LCD_ShowFontHZ_12(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *cn)
{  
	uint8_t i, j, wordNum;
	uint16_t color;
	while (*cn != '\0')
	{
		LCD_Set_Window(x, y, x+11, y+11);
		for (wordNum=0; wordNum<20; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar12x12[wordNum].Index[0]==*cn)
			     &&(CnChar12x12[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<24; i++) 
				{	//MSK的位数
					color=CnChar12x12[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							LCD_WriteData_Color(fc); 						
						} 						
						else
						{
							LCD_WriteData_Color(bc); 
						} 
						color<<=1;
					}//for(j=0;j<8;j++)结束
				}    
			}
		} //for (wordNum=0; wordNum<20; wordNum++)结束
		cn += 2;
		x += 12;
	}
}
#endif

#if 1
void LCD_ShowFontHZ_12(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *cn) {
	uint8_t i, j, wordNum;
	uint16_t color;
	uint16_t x0 = x;
	uint16_t y0 = y;
	while (*cn != '\0') {
		for (wordNum = 0; wordNum < 20; wordNum++) {	//wordNum扫描字库的字数
			if ((CnChar12x12[wordNum].Index[0] == *cn)
					&& (CnChar12x12[wordNum].Index[1] == *(cn + 1))) {
				for (i = 0; i < 32; i++) {	//MSK的位数
					color = CnChar12x12[wordNum].Msk[i];
					for (j = 0; j < 8; j++) {
						if ((color & 0x80) == 0x80) {
							LCD_DrawFRONT_COLOR(x, y, fc);
						} else {
							LCD_DrawFRONT_COLOR(x, y, bc);
						}
						color <<= 1;
						x++;
						if ((x - x0) == 12) {
							x = x0;
							y++;
							if ((y - y0) == 12) {
								y = y0;
							}
						}
					}	//for(j=0;j<8;j++)结束
				}
			}

		} //for (wordNum=0; wordNum<20; wordNum++)结束
		cn += 2;
		x += 12;
		x0 = x;
	}
}
#endif

void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t wide, uint16_t high, const unsigned char *pic) {
	uint16_t temp = 0;
	long tmp = 0, num = 0;
	LCD_Set_Window(x, y, x + wide - 1, y + high - 1);
	num = wide * high * 2;
	do {
		temp = pic[tmp + 1];
		temp = temp << 8;
		temp = temp | pic[tmp];
		LCD_WriteData_Color(temp); //逐点显示
		tmp += 2;
	} while (tmp < num);
}

//******************************************************************
//函数名：  Show_Str

//功能：    显示一个字符串,包含中英文显示
//输入参数：x,y :起点坐标
// 			fc:前置画笔颜色
//			bc:背景颜色
//			str :字符串
//			size:字体大小
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************    	   		   
void Show_Str(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str, uint8_t size, uint8_t mode) {
	uint16_t x0 = x;
	uint8_t bHz = 0;     //字符或者中文
	while (*str != 0)     //数据未结束
	{
		if (!bHz) {
			if (x > (tftlcd_data.width - size / 2)
					|| y > (tftlcd_data.height - size))
				return;
			if (*str > 0x80)
				bHz = 1;     				//中文
			else              				//字符
			{
				if (*str == 0x0D)           //换行符号
						{
					y += size;
					x = x0;
					str++;
				} else {
					if (size == 12 || size == 16) {
						LCD_ShowChar(x, y, fc, bc, *str, size, mode);
						x += size / 2; //字符,为全字的一半
					} else //字库中没有集成16X32的英文字体,用8X16代替
					{
						LCD_ShowChar(x, y, fc, bc, *str, 16, mode);
						x += 8; //字符,为全字的一半
					}
				}
				str++;

			}
		} else //中文
		{
			if (x > (tftlcd_data.width - size)
					|| y > (tftlcd_data.height - size))
				return;
			bHz = 0; //有汉字库
			if (size == 32)
				LCD_ShowFontHZ_32(x, y, fc, bc, str);
			else if (size == 24)
				LCD_ShowFontHZ_24(x, y, fc, bc, str);
			else if (size == 16)
				LCD_ShowFontHZ_16(x, y, fc, bc, str);
			else
				LCD_ShowFontHZ_12(x, y, fc, bc, str);

			str += 2;
			x += size; //下一个汉字偏移
		}
	}
}


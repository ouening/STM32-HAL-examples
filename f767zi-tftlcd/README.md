本例程主要介绍ITI9225 LCD的使用，基于F767ZI-NUCLEO开发板实现，如果读者需要移植到自己的开发环境中，需要修改相应的引脚，需要移植的文件有：

- `./Src/tftlcd.c`:tftlcd实现的源文件
- `./Inc/tftlcd.h`:LCD操作相关头文件，移植的话需要修改里面对应的引脚
- `./Inc/font.h`:字体相关头文件，常用ASCII表
- `./Inc/hanzi.h`:汉字相关头文件



LCD image.jpg：本例程所使用的LCD背面图片，包含了引脚连接信息

F767ZI-NUCLEO.pdf：开发板原理图，LCD连接CN10

NUCLEO-F767ZI_2.0_2.2 LCD.PDF：LCD模块原理图（参考2.2寸设计）


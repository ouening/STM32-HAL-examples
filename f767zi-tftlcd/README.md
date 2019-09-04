本例程主要介绍ITI9225 LCD的使用，基于F767ZI-NUCLEO开发板实现，如果肚子需要移植到自己的开发环境中，需要修改相应的引脚，需要移植的文件有：

- `./src/tftlcd.c`:tftlcd实现的源文件
- `./Inc/tftlcd.h`:LCD操作相关头文件，移植的话需要修改里面对应的引脚
- `./Inc/font.h`:字体相关头文件，常用ASCII表
- `./Inc/hanzi.h`:汉字相关头文件




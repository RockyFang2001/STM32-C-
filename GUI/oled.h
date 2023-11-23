#ifndef __OLED_H
#define __OLED_H		

#ifdef __cplusplus
extern "C" {
#endif
#include "stdlib.h"	    	
#include "stdint.h"							  
//-----------------OLED 定义----------------  					   
#define Max_Column	128      
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(unsigned dat); 
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t Char_Size,uint8_t mode);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void Picture(void);
void OLED_ClearLine(uint8_t line);
void OLED_P6x8Str(uint8_t x, uint8_t y, uint8_t ch[]);
void OLED_P6x8StrW(uint8_t x, uint8_t y, uint8_t ch[]);
#ifdef __cplusplus
}
#endif
#endif

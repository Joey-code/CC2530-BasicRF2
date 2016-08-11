
/*                  Creat By ES                       */
/*           http://shop101811732.taobao.com          */

#ifndef JLX12864_H
#define JLX12864_H

#include "ioCC2530.h"
#include "english_6x8_pixel.h"


#define  uchar unsigned char
#define  uint  unsigned int
#define  ulong  unsigned long

#define lcd_sclk      P0_4 //
#define lcd_sid       P0_5 //
#define lcd_rs      P0_6 //
#define lcd_reset      P0_7 //
#define lcd_cs1      P1_0 //

void transfer_command_lcd(int data1);
void transfer_data_lcd(int data1);
void delay(int n_ms);
void LCD_delay(uchar ns);
void waitkey();
void initial_lcd();
void lcd_address(uint page,uint column);
void clear_screen();
void display_128x64(uchar *dp);
void display_graphic_16x16(uint page,uint column,uchar *dp);
void display_graphic_8x16(uint page,uchar column,uchar *dp);
void display_graphic_5x7(uint page,uchar column,uchar *dp);
void LCD_write_char( unsigned char c);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
void LCD_disp_char(unsigned char x, unsigned char y, unsigned char c);
void LCD_disp_int(unsigned char x, unsigned char y, unsigned int t);


#endif







/*                  Creat By ES                       */
/*           http://es-taobao.taobao.com          */

#include "JLX12864.h"


/*写指令到LCD 模块*/


void transfer_command_lcd (int  SPIData)
{
  unsigned char SPICount;  // Counter used to clock out the data
  lcd_rs=0; LCD_delay(5);
  for (SPICount = 0; SPICount < 8; SPICount++)
  {
    lcd_sclk=0; LCD_delay(10);
    if (SPIData & 0x80) lcd_sid=1;
    else lcd_sid=0;
    LCD_delay(10);
    lcd_sclk=1;
    LCD_delay(10);
    SPIData <<= 1;
  }          	
} 

/*写数据到LCD 模块*/
void transfer_data_lcd(int  SPIData)
{
  unsigned char SPICount;  // Counter used to clock out the data
  lcd_rs=1;
  for (SPICount = 0; SPICount < 8; SPICount++)
  {
    lcd_sclk=0; LCD_delay(5);
    if(SPIData & 0x80) lcd_sid=1;
    else lcd_sid=0;
    LCD_delay(10);
    lcd_sclk=1;
    LCD_delay(10);
    SPIData<<=1;
  }
}
/*延时*/
void delay(int n_ms)
{
  int j,k;
  for(j=0;j<n_ms;j++)
  for(k=0;k<810;k++);
}

void LCD_delay(uchar ns)
{
  uchar j;
  for(j=0;j<ns;j++);
}
/*等待一个按键，我的主板是用P1.1 与GND 之间接一个按键*/
void waitkey()
{
  repeat:
  if (P1&0x02) goto repeat;
  else delay(6);
  if (P1&0x02) goto repeat;
  else
  delay(40);;
}
/*LCD 模块初始化*/
void initial_lcd()
{
  lcd_cs1=0;
//  Rom_CS = 1;
  lcd_reset=0; /*低电平复位*/
  delay(20);
  lcd_reset=1; /*复位完毕*/
  delay(20);
  transfer_command_lcd(0xe2); /*软复位*/
  delay(5);
  transfer_command_lcd(0x2c); /*升压步聚1*/
  delay(5);
  transfer_command_lcd(0x2e); /*升压步聚2*/
  delay(5);
  transfer_command_lcd(0x2f); /*升压步聚3*/
  delay(5);
  transfer_command_lcd(0x23); /*粗调对比度，可设置范围0x20～0x27*/
  transfer_command_lcd(0x81); /*微调对比度*/
  transfer_command_lcd(0x1f); /*0x28,微调对比度的值，可设置范围0x00～0x3f*/
  transfer_command_lcd(0xa2); /*1/9 偏压比（bias）*/
  transfer_command_lcd(0xc8); /*行扫描顺序：从上到下*/
  transfer_command_lcd(0xa0); /*列扫描顺序：从左到右*/
  transfer_command_lcd(0x40); /*起始行：第一行开始*/
  transfer_command_lcd(0xaf); /*开显示*/
  lcd_cs1=1;
}
void lcd_address(uint page,uint column)
{
  column=column-0x01;
  transfer_command_lcd(0xb0+page-1); /*设置页地址*/
  transfer_command_lcd(0x10+(column>>4&0x0f)); /*设置列地址的高4 位*/
  transfer_command_lcd(column&0x0f); /*设置列地址的低4 位*/
}
/*全屏清屏*/
void clear_screen()
{
  unsigned char i,j;
  lcd_cs1=0;
//  Rom_CS = 1;
  for(i=0;i<9;i++)
  {
    transfer_command_lcd(0xb0+i);
    transfer_command_lcd(0x10);
    transfer_command_lcd(0x00);
    for(j=0;j<132;j++)
    {
      transfer_data_lcd(0x00);
    }
  }
  lcd_cs1=1;
}
/*显示128x64 点阵图像*/
void display_128x64(uchar *dp)
{
  uint i,j;
  lcd_cs1=0;
  for(j=0;j<8;j++)
  {
    lcd_address(j+1,1);
    for (i=0;i<128;i++)
    {
      transfer_data_lcd(*dp); /*写数据到LCD,每写完一个8 位的数据后列地址自动加1*/
      dp++;
     }
  }
  lcd_cs1=1;
}
/*显示16x16 点阵图像、汉字、生僻字或16x16 点阵的其他图标*/
void display_graphic_16x16(uint page,uint column,uchar *dp)
{
  uint i,j;
  lcd_cs1=0;
//  Rom_CS = 1;
  for(j=0;j<2;j++)
  {
    lcd_address(page+j,column);
    for (i=0;i<16;i++)
    {
      transfer_data_lcd(*dp); /*写数据到LCD,每写完一个8 位的数据后列地址自动加1*/
      dp++;
    }
  }
  lcd_cs1=1;
}
/*显示8x16 点阵图像、ASCII, 或8x16 点阵的自造字符、其他图标*/
void display_graphic_8x16(uint page,uchar column,uchar *dp)
{
  uint i,j;
  lcd_cs1=0;
//  Rom_CS = 1; // Rick
  for(j=0;j<2;j++)
  {
    lcd_address(page+j,column);
    for (i=0;i<8;i++)
    {
      transfer_data_lcd(*dp); /*写数据到LCD,每写完一个8 位的数据后列地址自动加1*/
      dp++;
    }
  }
  lcd_cs1=1;
}
/*显示 5*7 点阵图像、ASCII, 或5x7 点阵的自造字符、其他图标*/
void display_graphic_5x7(uint page,uchar column,uchar *dp)
{
  uint col_cnt;
  uchar page_address;
  uchar column_address_L,column_address_H;
   page_address = 0xb0+page-1;
  lcd_cs1=0;
//  Rom_CS = 1; // Rick
  column_address_L =(column&0x0f)-1;
  column_address_H =((column>>4)&0x0f)+0x10;
  transfer_command_lcd(page_address); /*Set Page Address*/
  transfer_command_lcd(column_address_H); /*Set MSB of column Address*/
  transfer_command_lcd(column_address_L); /*Set LSB of column Address*/
  for (col_cnt=0;col_cnt<6;col_cnt++)
  {
    transfer_data_lcd(*dp);
    dp++;
  }
  lcd_cs1=1;
}

void LCD_write_char( unsigned char c)
  {
    unsigned char line;
	lcd_cs1=0;
    c -= 32;
    for (line=0; line<6; line++)
    transfer_data_lcd(font6x8[c][line]);
	lcd_cs1=1;
  }

void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
  {
    lcd_cs1=0;
	lcd_address(X,Y);
    while (*s) 
      {
	 LCD_write_char(*s);
	 s++;
      }
	lcd_cs1=1;
  }

void LCD_disp_char(unsigned char x, unsigned char y, unsigned char c)
{
 	unsigned char temp[3];
	lcd_cs1=0;
 	temp[0]='0'+c/100;
 	temp[1]='0'+(c-(c/100)*100)/10;
 	temp[2]='0'+c%10;
    lcd_address(x,y);
    LCD_write_char(temp[0]); 
    LCD_write_char(temp[1]);
    LCD_write_char(temp[2]);
	lcd_cs1=1;
}

void LCD_disp_int(unsigned char x, unsigned char y, unsigned int t)
{
 	unsigned char a[5];
        lcd_cs1=0;

	a[0]='0'+(t/10000)%10;         //取得整数值到数组         	
	a[1]='0'+(t/1000)%10;                                     	
	a[2]='0'+(t/100)%10;                                      	
	a[3]='0'+(t/10)%10;                                       	
	a[4]='0'+(t/1)%10; 
        lcd_address(x,y);
	LCD_write_char(a[0]); 
        LCD_write_char(a[1]);
        LCD_write_char(a[2]);
        LCD_write_char(a[3]);
        LCD_write_char(a[4]);
        lcd_cs1=1;
}







/*                  Creat By ES                       */
/*           http://es-taobao.taobao.com          */

#include "JLX12864.h"


/*дָ�LCD ģ��*/


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

/*д���ݵ�LCD ģ��*/
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
/*��ʱ*/
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
/*�ȴ�һ���������ҵ���������P1.1 ��GND ֮���һ������*/
void waitkey()
{
  repeat:
  if (P1&0x02) goto repeat;
  else delay(6);
  if (P1&0x02) goto repeat;
  else
  delay(40);;
}
/*LCD ģ���ʼ��*/
void initial_lcd()
{
  lcd_cs1=0;
//  Rom_CS = 1;
  lcd_reset=0; /*�͵�ƽ��λ*/
  delay(20);
  lcd_reset=1; /*��λ���*/
  delay(20);
  transfer_command_lcd(0xe2); /*��λ*/
  delay(5);
  transfer_command_lcd(0x2c); /*��ѹ����1*/
  delay(5);
  transfer_command_lcd(0x2e); /*��ѹ����2*/
  delay(5);
  transfer_command_lcd(0x2f); /*��ѹ����3*/
  delay(5);
  transfer_command_lcd(0x23); /*�ֵ��Աȶȣ������÷�Χ0x20��0x27*/
  transfer_command_lcd(0x81); /*΢���Աȶ�*/
  transfer_command_lcd(0x1f); /*0x28,΢���Աȶȵ�ֵ�������÷�Χ0x00��0x3f*/
  transfer_command_lcd(0xa2); /*1/9 ƫѹ�ȣ�bias��*/
  transfer_command_lcd(0xc8); /*��ɨ��˳�򣺴��ϵ���*/
  transfer_command_lcd(0xa0); /*��ɨ��˳�򣺴�����*/
  transfer_command_lcd(0x40); /*��ʼ�У���һ�п�ʼ*/
  transfer_command_lcd(0xaf); /*����ʾ*/
  lcd_cs1=1;
}
void lcd_address(uint page,uint column)
{
  column=column-0x01;
  transfer_command_lcd(0xb0+page-1); /*����ҳ��ַ*/
  transfer_command_lcd(0x10+(column>>4&0x0f)); /*�����е�ַ�ĸ�4 λ*/
  transfer_command_lcd(column&0x0f); /*�����е�ַ�ĵ�4 λ*/
}
/*ȫ������*/
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
/*��ʾ128x64 ����ͼ��*/
void display_128x64(uchar *dp)
{
  uint i,j;
  lcd_cs1=0;
  for(j=0;j<8;j++)
  {
    lcd_address(j+1,1);
    for (i=0;i<128;i++)
    {
      transfer_data_lcd(*dp); /*д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1*/
      dp++;
     }
  }
  lcd_cs1=1;
}
/*��ʾ16x16 ����ͼ�񡢺��֡���Ƨ�ֻ�16x16 ���������ͼ��*/
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
      transfer_data_lcd(*dp); /*д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1*/
      dp++;
    }
  }
  lcd_cs1=1;
}
/*��ʾ8x16 ����ͼ��ASCII, ��8x16 ����������ַ�������ͼ��*/
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
      transfer_data_lcd(*dp); /*д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1*/
      dp++;
    }
  }
  lcd_cs1=1;
}
/*��ʾ 5*7 ����ͼ��ASCII, ��5x7 ����������ַ�������ͼ��*/
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

	a[0]='0'+(t/10000)%10;         //ȡ������ֵ������         	
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






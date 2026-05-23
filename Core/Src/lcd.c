/*
 * lcd.c
 *
 *  Created on: Apr 21, 2026
 *      Author: cch1104
 */
#include "main.h"

#define LCD_RS GPIO_PIN_6
#define LCD_EN GPIO_PIN_7
#define LCD_D4 GPIO_PIN_8
#define LCD_D5 GPIO_PIN_9
#define LCD_D6 GPIO_PIN_10
#define LCD_D7 GPIO_PIN_11

void lcd_Strobe(){
	HAL_GPIO_WritePin(GPIOA, LCD_EN, GPIO_PIN_SET); //E='1'
	HAL_Delay(0.1);
	HAL_GPIO_WritePin(GPIOA, LCD_EN, GPIO_PIN_RESET);//E='0'
	HAL_Delay(0.1);
}


void lcd_Write_cmd(unsigned char c){
	unsigned int d;
	d=c;
	d=(d<<4) & 0x0F00;
	GPIOA -> ODR=d;
	HAL_GPIO_WritePin(GPIOA, LCD_RS, GPIO_PIN_RESET);
	lcd_Strobe();

	d=c;
	d=(d<<8) & 0x0F00;
	GPIOA ->ODR =d;
	HAL_GPIO_WritePin(GPIOA, LCD_RS, GPIO_PIN_RESET);
	lcd_Strobe();
	HAL_Delay(0.1);
}

void lcd_Write_data(unsigned char c){
	unsigned int d;
	d=c;
	d=(d<<4) & 0x0F00;
	GPIOA -> ODR =d;
	HAL_GPIO_WritePin(GPIOA, LCD_RS, GPIO_PIN_SET);
	lcd_Strobe();

	d=c;
	d=(d<<8) & 0x0F00;
	GPIOA ->ODR =d;
	HAL_GPIO_WritePin(GPIOA, LCD_RS, GPIO_PIN_SET);
	lcd_Strobe();
}

void lcd_Clear(void){
	lcd_Write_cmd(0x01);
	HAL_Delay(5);
}

void lcd_Puts(const char*s){
	while(*s)
		lcd_Write_data(*s++);
}

void lcd_Putch(char c){
	unsigned int d;

	d=c;
	d= (d<<4) & 0x0F00;
	GPIOA -> ODR = d;
	HAL_GPIO_WritePin(GPIOA, LCD_RS, GPIO_PIN_SET);
	lcd_Strobe();

	d=c;
	d=(d<<8) & 0x0F00;
	GPIOA -> ODR =d;
	HAL_GPIO_WritePin(GPIOA, LCD_RS, GPIO_PIN_SET);
	lcd_Strobe();

}

void lcd_Goto(int col, int row){
	char address;

	if(row==0)address=0;
	if(row==1)address=0x40;
	address += col -1;

	lcd_Write_cmd(0x80 | address);
}

void lcd_Init(void){
	GPIOA -> ODR =0;
	HAL_Delay(50);
	GPIOA -> ODR= 0x0300;
	lcd_Strobe();
	HAL_Delay(30);
	lcd_Strobe();
	HAL_Delay(20);
	lcd_Strobe();
	HAL_Delay(20);
	GPIOA -> ODR = 0x0200;
	lcd_Strobe();
	HAL_Delay(5);
	lcd_Write_cmd(0x28);
	HAL_Delay(5);
	lcd_Write_cmd(0x0F);
	HAL_Delay(5);
	lcd_Write_cmd(0x01);
	HAL_Delay(5);
	lcd_Write_cmd(0x06);
	HAL_Delay(5);
}

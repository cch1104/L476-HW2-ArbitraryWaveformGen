/*
 * lcd.h
 *
 *  Created on: May 23, 2026
 *      Author: cch11
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_



#endif /* INC_LCD_H_ */

void lcd_Strobe(void);
void lcd_Write_cmd(unsigned char);
void lcd_Write_data(unsigned char);
void lcd_Clear(void);
void lcd_Puts(const char *s);
void lcd_Putch(char c);
void lcd_Goto(int,int);
void lcd_Init(void);

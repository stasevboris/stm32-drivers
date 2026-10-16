#ifndef LCD1602_H
#define LCD1602_H
#include "stm32f4xx.h"
void LCD_port_init(void);
void LCD_init(void);
void LCD_pos(uint8_t line, uint8_t col);
void LCD_send_string(char* str);
void LCD_write(uint8_t data, uint8_t rs);
#endif
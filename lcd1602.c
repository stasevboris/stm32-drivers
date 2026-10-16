#include "lcd1602.h"
#include "delay.h"

void LCD_port_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER &= ~0x0000FFF0; // PB2-PB7
    GPIOB->MODER |=  0x00005550;
}
void LCD_pulse(void) {
    GPIOB->BSRR = (1 << 3); delay_ms(2);
    GPIOB->BSRR = (1 << 19); delay_ms(2);
}
void LCD_write(uint8_t data, uint8_t rs) {
    if(rs) GPIOB->BSRR = (1 << 2); else GPIOB->BSRR = (1 << 18);
    GPIOB->ODR = (GPIOB->ODR & 0xFF0F) | (data & 0xF0);
    LCD_pulse();
    GPIOB->ODR = (GPIOB->ODR & 0xFF0F) | ((data << 4) & 0xF0);
    LCD_pulse();
}
void LCD_init(void) {
    delay_ms(100);
    LCD_write(0x33, 0); delay_ms(10);
    LCD_write(0x32, 0);
    LCD_write(0x28, 0); LCD_write(0x0C, 0); LCD_write(0x01, 0);
    delay_ms(10);
}
void LCD_pos(uint8_t line, uint8_t col) {
    LCD_write((line == 1 ? 0x80 : 0xC0) + col - 1, 0);
}
void LCD_send_string(char* str) {
    while(*str) LCD_write(*str++, 1);
}
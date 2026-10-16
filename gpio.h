// Настройка линий портов

#ifndef GPIO_H_
#define GPIO_H_


//=============================================================================
// Библиотеки 
#include "stm32f4xx.h"

// Определения альтернативных функций (alt_func)
#define SYS 		0x0UL
#define TIM_1 		0x1UL
#define TIM_2 		0x1UL
#define TIM_3 		0x2UL
#define TIM_4 		0x2UL
#define TIM_5 		0x2UL
#define TIM_8 		0x3UL
#define TIM_9 		0x3UL
#define TIM_10 		0x3UL
#define TIM_11 		0x3UL
#define I2C_1 		0x4UL
#define I2C_2 		0x4UL
#define I2C_3 		0x4UL
//...
#define USART_1		0x7UL
#define USART_2		0x7UL
#define USART_3		0x7UL
//...

#define PUSH_PULL	(_Bool)0
#define OPEN_DRAIN	(_Bool)1
#define NO_PULL		0x0UL
#define PULL_UP		0x1UL
#define PULL_DOWN	0x2UL
#define LOW			0x0UL
#define MEDIUM		0x1UL
#define HIGH		0x2UL
#define VERY_HIGH	0x3UL

//=============================================================================
// Прототипы функций
void gpio_enable(GPIO_TypeDef* GPIOx);				// разрешение тактирования порта 
void gpio_alt_func(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t alt_func, _Bool out_type);
													// настройка линии порта на альтернативную функцию
void gpio_input(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t input_type);
void gpio_output(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t speed, _Bool out_type);
													// настройка линии порта на вывод


#endif /* GPIO_H_ */
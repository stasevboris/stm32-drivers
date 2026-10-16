// Настройки таймеров

#ifndef TIMER_H_
#define TIMER_H_

#include "stm32f4xx.h"
#include "delay.h"
#include "gpio.h"

// макрокоманды ===============================================================
// разрешаем прерывания
/*
	TIMx_DIER - регистр разрешения прерываний
		Bit 0 UIE: Update interrupt enable
*/
#define Timer_Int_Enable(TIMx)	TIMx->DIER |= TIM_DIER_UIE;
// запрещаем прерывания
#define Timer_Int_Disable(TIMx)	TIMx->DIER &= ~TIM_DIER_UIE;

// разрешаем работу таймера 
/*
	TIMx_CR1
		Bit 0 CEN: Counter enable
			0: Counter disabled
			1: Counter enabled
*/
#define Timer_Enable(TIMx)		TIMx->CR1 |= TIM_CR1_CEN;
// запрещаем работу таймера 
#define Timer_Disable(TIMx)		TIMx->CR1 &= ~TIM_CR1_CEN;

// cброс счётного регистра
#define Timer_Clear(TIMx)		TIMx->CNT = 0UL;

// прототипы функций ==========================================================
void timer_1_init(void);							// инициализация таймера 1
void timer_2_init(void);							// инициализация таймера 2
void timer_3_init(void);							// инициализация таймера 3
void timer_5_init(void);							// инициализация таймера 5
void PWM_OCx_Init(TIM_TypeDef* TIMx, uint8_t channel, uint16_t prescaler, uint32_t period, uint32_t compare);


#endif /* TIMER_H_ */
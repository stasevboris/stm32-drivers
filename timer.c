/* 	Настройки таймеров
	
	tim_3, tim_4 - 16-bit
	tim_2, tim_5 - 32-bit

	tim_1, 8...11 - APB2 (168 МГц)
	tim_2...7, 12...14 - APB1 (84 МГц)
*/

#include "timer.h"

//=============================================================================
// инициализация таймера 2
void timer_2_init(void)
{
	NVIC_SetPriority(TIM2_IRQn, 1);							// установка самого низкого приоритета (0...15)
	NVIC_EnableIRQ(TIM2_IRQn);								// TIM2 global Interrupt
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;						// разрешаем тактирование таймера 2
/*
	TIMx_PSC - регистр предделителя тактовой частоты
		Bits 15:0 PSC[15:0]: предделитель
			fCK_PSC / (PSC[15:0] + 1).
	При максимальной частоте APB1 = 16 МГц, и предделителе 160 имеем 
	частоту тактирования таймера 100 кГц (период 10 мс).
*/
	TIM2->PSC = 160UL - 1UL;
/*
	TIMx_ARR
		Bits 15:0 ARR[15:0]: предел счёта
	При частоте тактирования 100 кГц и модуле счёта 100000 (0x186A0) имеем период счёта = 1 с
	Таймер не работает, если ARR = 0. 
*/
	TIM2->ARR = 100000UL;

	TIM2->SMCR = RESET;										// сброс регистра SMCR
	TIM2->SMCR &= ~TIM_SMCR_SMS;							// конфигурирование внутреннего тактирования
/*
	TIMx_DIER - регистр разрешения прерываний
		Bit 0 UIE: Update interrupt enable
*/
	TIM2->DIER |= TIM_DIER_UIE;
/*
	TIMx_CR1
		Bit 0 CEN: Counter enable
			0: Counter disabled
			1: Counter enabled
*/
	TIM2->CR1 |= TIM_CR1_CEN;								// разрешаем работу таймера	
	return;
}

//=============================================================================
// инициализация таймера 3 (16 бит)
// (производит отсчёт времени)
void timer_3_init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;						// разрешаем тактирование таймера 3
//	NVIC_EnableIRQ(TIM3_IRQn);								// TIM3 global Interrupt
//	NVIC_SetPriority(TIM3_IRQn, 15);						// установка самого низкого приоритета (0...15)

/*
	TIMx_ARR - модуль счёта
		Bits 15:0 ARR[15:0]: модуль счёта
	При частоте тактирования 1 МГц и модуле счёта 65535 имеем период счёта 1 мкс...65536 мкс
	Таймер не работает, если ARR = 0. 
*/
	TIM3->ARR = 65535;

/*
	TIMx_PSC - регистр предделителя тактовой частоты
		Bits 15:0 PSC[15:0]: предделитель
			fCK_PSC / (PSC[15:0] + 1).
	При максимальной частоте APB1 = 84 МГц, и предделителе 83 (0x0053) имеем 
	частоту тактирования таймера 1 МГц (период 1 мкс).
*/
	TIM3->PSC = 83;

//	Timer_Int_Enable(TIM3);									// разрешаем прерывания
//	Timer_Enable(TIM3);										// разрешаем работу таймера	3
	
	return;
}

//=============================================================================
// инициализация таймера 5 (32 бита) 
// (производит отсчёт времени)
void timer_5_init(void)
{
//	NVIC_SetPriority(TIM5_IRQn, 1);							// установка приоритета (0...15)
//	NVIC_EnableIRQ(TIM5_IRQn);								// TIM2 global Interrupt
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;						// разрешаем тактирование таймера
/*
	TIMx_PSC - регистр предделителя тактовой частоты
		Bits 15:0 PSC[15:0]: предделитель
			fCK_PSC / (PSC[15:0] + 1).
	При максимальной частоте APB2 = 16 МГц, и предделителе 16 имеем 
	частоту тактирования таймера 1 МГц (период 1 мкс).
*/
	TIM5->PSC = 16UL - 1UL;
/*
	TIMx_ARR - модуль счёта 
		Bits 15:0 ARR[15:0]: модуль счёта 
	При частоте тактирования 1 МГц и модуле счёта 0xFFFF имеем период счёта 1 мкс...65536 мкс
	Таймер не работает, если ARR = 0. 
*/
	TIM5->ARR = 0xFFFFFFFF;
/*
	TIMx_CR1 - TIMx control register 1
		ARPE: 	Auto-reload preload enable
		OPM: 	One-pulse mode
		UDIS: 	Update disable
		URS: 	Update request source
*/	
//	TIM5->CR1 |= TIM_CR1_OPM;								// режим одного импульса (исключает дребезг 1 мл. разряда) 
//	TIM5->CR1 |= TIM_CR1_ARPE;								// обновление ARR на вершине счёта	
//	TIM5->CR1 |= TIM_CR1_UDIS;
//	TIM5->CR1 |= TIM_CR1_URS;
/*
	TIMx_EGR - TIMx event generation register
		UG: Update generation
			This bit can be set by software, it is automatically cleared by hardware
*/
//	TIM5->EGR |= TIM_EGR_UG;								// ручное обновление регистров
	
//	TIM5->DIER |= TIM_DIER_UIE;								// разрешаем прерывания по переполнению
//	Timer_Clear(TIM5);										// сброс счётного регистра
	Timer_Enable(TIM5);										// разрешаем работу таймера	
	return;
}


//=============================================================================
// Инициализация выхода ШИМ таймера X (APB1 - 16/72 МГц)
// 
// channel - номер канала таймера
// prescaler - значение предделителя частоты
// period - модуль счёта таймера (период)
// compare - константа сравнения компаратора таймера
//
void PWM_OCx_Init(TIM_TypeDef* TIMx, uint8_t channel, uint16_t prescaler, uint32_t period, uint32_t compare)
{
    /*
        RCC_AHB1ENR - регистр включения устройств, подключеных к шине AHB1
            TIMxEN = 1 - разрешаем тактирование таймера X
	*/  
	if(TIMx == TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	
	else if(TIMx == TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	
	else if(TIMx == TIM4) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;	
	else if(TIMx == TIM5) RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;	
//	else if(TIMx == TIM6) RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;	
//	else if(TIMx == TIM7) RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;	
	
	TIMx->CR1 &= ~TIM_CR1_CEN;						// остановка таймера
	/*
		TIMx_PSC - регистр предделителя тактовой частоты
			Bits 15:0 PSC[15:0]: предделитель
				fCK_PSC / (PSC[15:0] + 1).
	*/
	TIMx->PSC = (uint16_t)(prescaler - 1);
	/*
		TIMx_ARR - модуль счёта 
			Bits 31:16 ARR[31:16] - для timer 2 и 5
			Bits 15:0  ARR[15:0]: модуль счёта 
	*/
	TIMx->ARR = period;								// модуль счёта 	
	TIMx->CR1 |= TIM_CR1_ARPE;						// обновление модуля счёта на вершине счёта	

	if(channel == 1) 
	{
		TIMx->CCER &= ~TIM_CCER_CC1E;				// запрет работы канала 1 на выход
		TIMx->CCR1 = compare;						// TIMx_CCRy - регистр сравнения канала Y
		TIMx->CCMR1 &= ~TIM_CCMR1_OC1M;				// TIMx_CCMRn - режим выхода компаратор таймера PWM Mode 1
		TIMx->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;	
		TIMx->CCER |= TIM_CCER_CC1E;				// TIM_CCER_CCyE - разрешение работы канала 1 на выход
	}
	else if(channel == 2) 
	{	
		TIMx->CCER &= ~TIM_CCER_CC2E;				
		TIMx->CCR2 = compare;
		TIMx->CCMR1 &= ~TIM_CCMR1_OC2M;				
		TIMx->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;	
		TIMx->CCER |= TIM_CCER_CC2E;
	}
	else if(channel == 3) 
	{
		TIMx->CCER &= ~TIM_CCER_CC3E;				
		TIMx->CCR3 = compare;
		TIMx->CCMR2 &= ~TIM_CCMR2_OC3M;				
		TIMx->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;			
		TIMx->CCER |= TIM_CCER_CC3E;
	}
	else if(channel == 4) 
	{	
		TIMx->CCER &= ~TIM_CCER_CC4E;				
		TIMx->CCR4 = compare;
		TIMx->CCMR2 &= ~TIM_CCMR2_OC4M;				
		TIMx->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;				
		TIMx->CCER |= TIM_CCER_CC4E;
	}
	/*
		TIMx_CR1 - регистр 1 управления таймером
			CMS: выбор режима выравнивания
				00: выравнивание по фронту
			DIR: направление счёта
				0: счёт вверх
				1: счёт вниз
			CEN: включение таймера 
				0: выключен
				1: включен
	*/
	TIMx->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);  	// считаем вверх, выравнивание по фронту (Fast PWM)
	/*
		TIMx_EGR - TIMx event generation register
			UG: Update generation
	*/
	TIMx->EGR |= TIM_EGR_UG;						// запись значений предзагрузки и сравнения	 
	TIMx->CR1 |= TIM_CR1_CEN;						// запуск таймера
	return;
}




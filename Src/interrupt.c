/*
 * interrupt.c
 *
 *  Created on: Jul 17, 2026
 *      Author: Freshmai
 */



/* \/ Start of libraries \/ */

#include "interrupt.h"
#include <stdint.h>
#include "stm32f4xx.h"
#include "timer.h"
#include "uart.h"
/* /\ End of libraries /\ */


/* \/ Start of Macros \/ */

#define GPIOCEN							(1U<<2)
#define SYSCFGEN						(1U<<14)
#define UNMASK_MR13						(1U<<13)
#define ENRISINGEDGETRIGGER_TR13		(1U<<13)
#define DISFALLINGEDGETRIGGER_TR13		(1U<<13)
#define TIM2_DIER_TUIE					(1U<<0)
#define USART2_RXNEIE					(1U<<5)
/* /\ End of Macros */

/* \/ Start of function definitions \/ */

/*======================================*
 * 										*
 *  Name: pc13_user_button_exti_init  	*
 *   @param: N/A						*
 *   @brief: start user button B1		*
 *   interrupt							*
 *   @return: N/A						*
 * 										*
 *======================================*/
void pc13_user_button_exti_init(void)
{

	// Enable clock access to Port C
	RCC->AHB1ENR |= GPIOCEN;

	// Enable clock access to SYSCFG
	RCC->APB2ENR |= SYSCFGEN;

	// Set mode bits for Pin 13 on Port C to input mode
	GPIOC->MODER &=~ (1U<<26);
	GPIOC->MODER &=~ (1U<<27);

	// Set EXTI13 as PC13 as the source input
	SYSCFG->EXTICR[3] &=~ (1<<4);
	SYSCFG->EXTICR[3] |=  (1<<5);
	SYSCFG->EXTICR[3] &=~ (1<<6);
	SYSCFG->EXTICR[3] &=~ (1<<7);

	// Set PC13 to connect to the pull down resistor (keeping it at 0V)
	GPIOC->PUPDR &=~ (1U<<26);
	GPIOC->PUPDR |= (1U<<27);

	// Unmask EXTI line 13 (enable interrupt)
	EXTI->IMR |= UNMASK_MR13;

	// Enable Rising edge trigger
	EXTI->RTSR |= ENRISINGEDGETRIGGER_TR13;

	// Disable Falling edge trigger
	EXTI->FTSR &=~ DISFALLINGEDGETRIGGER_TR13;

	// Enable Interrupt in NVIC by taking into consideration how PC13 falls into
	// with EXTI 10-15
	NVIC_EnableIRQ(EXTI15_10_IRQn);


}


/*==========================================*
 * 											*
 *  Name: tim2_periodic_interrupt_init_ms   *
 *   @param: durationMs takes the			*
 *   number of milliseconds the				*
 *   user wants to time						*
 *   @brief: start tim2 interrupt			*
 *   @return: N/A							*
 * 											*
 *==========================================*/
void tim2_periodic_interrupt_init_ms(uint32_t durationMs)
{
	// Initialize tim2 at 1khz
	timer_init(&TIMER_2);

	// Enable TIM2 interrupt
	TIM2->DIER |= TIM2_DIER_TUIE;

	// Enable timer2 interrupt in the NVIC
	NVIC_EnableIRQ(TIM2_IRQn);

	// start tim2 for the time passed by the user in milliseconds
	startTimer(durationMs, &TIMER_2);



}


/*======================================*
 * 										*
 *  Name: usart2_interrupt_int	    	*
 *   @param: N/A						*
 *   @brief: enable usart2 rx interrupt	*
 *   @return: N/A						*
 * 										*
 *======================================*/
void usart2_interrupt_init(void)
{
	// initialize usart2
	usart2_init();

	// Enable interrupt for receiving
	USART2->CR1 |= USART2_RXNEIE;

	// Enable USART2 interrupt in the NVIC
	NVIC_EnableIRQ(USART2_IRQn);


}
/* /\ End of function definitions /\ */



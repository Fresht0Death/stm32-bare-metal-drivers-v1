/*
 * timer.c
 *
 *  Created on: Jul 1, 2026
 *      Author: Freshmai
 */

/* \/  Start of libraries \/ */

#include "stm32f4xx.h"
#include "gpio.h"
#include "timer.h"
#include <stddef.h>
/* /\  End of libraries /\ */


/* \/ Start of Macro definitons \/ */

#define ENPROCESSORCLK				(1U<<2)
#define ENCOUNTERSYSCLK				(1U<<0)
#define COUNTFLAG					(1U<<16)
#define COUNTERENABLE				(1U<<0)
#define TIMERUPDATEINTERRUPTFLAG	(1U<<0)
#define MAX16BITTIMERVALUE			(65536U)

#define RCCAPB1ENTIM2				(1<<0)
#define RCCAPB1ENTIM3				(1<<1)
#define RCCAPB1ENTIM4				(1<<2)
#define RCCAPB1ENTIM5				(1<<3)
#define RCCAPB1ENTIM6				(1<<4)
#define RCCAPB1ENTIM7				(1<<5)
#define RCCAPB1ENTIM12				(1<<6)
#define RCCAPB1ENTIM13				(1<<7)
#define RCCAPB1ENTIM14				(1<<8)


#define RCCAPB2ENTIM1				(1U<<0)
#define RCCAPB2ENTIM8				(1U<<1)
#define RCCAPB2ENTIM9				(1U<<16)
#define RCCAPB2ENTIM10				(1U<<17)
#define RCCAPB2ENTIM11				(1U<<18)
/* /\ End of Macro definitions /\ */


/* \/ Timer Object definitions \/  */

// Default clock 16,000,000 Hz = 16 MHz
// PSC is 15,999 so the timer count at 1kHz

/* TIM1 Object */
TimerHandle TIMER_1 = {

	.timerInstance = TIM1,
	.timerPrescaler = (15999)

};

/* TIM2 Object */
TimerHandle TIMER_2 = {

	.timerInstance = TIM2,
	.timerPrescaler = (15999)

};
/* TIM3 Object */
TimerHandle TIMER_3 = {

	.timerInstance = TIM3,
	.timerPrescaler = (15999)

};
/* TIM4 Object */
TimerHandle TIMER_4 = {

	.timerInstance = TIM4,
	.timerPrescaler = (15999)

};
/* TIM5 Object */
TimerHandle TIMER_5 = {

	.timerInstance = TIM5,
	.timerPrescaler = (15999)

};

/* TIM6 Object */
TimerHandle TIMER_6 = {

	.timerInstance = TIM6,
	.timerPrescaler = (15999)

};

/* TIM7 Object */
TimerHandle TIMER_7 = {

	.timerInstance = TIM7,
	.timerPrescaler = (15999)

};

/* TIM8 Object */
TimerHandle TIMER_8 = {

	.timerInstance = TIM8,
	.timerPrescaler = (15999)

};

/* TIM9 Object */
TimerHandle TIMER_9 = {

	.timerInstance = TIM9,
	.timerPrescaler = (15999)

};

/* TIM10 Object */
TimerHandle TIMER_10 = {

	.timerInstance = TIM10,
	.timerPrescaler = (15999)

};

/* TIM11 Object */
TimerHandle TIMER_11 = {

	.timerInstance = TIM11,
	.timerPrescaler = (15999)

};

/* TIM12 Object */
TimerHandle TIMER_12 = {

	.timerInstance = TIM12,
	.timerPrescaler = (15999)

};

/* TIM13 Object */
TimerHandle TIMER_13 = {

	.timerInstance = TIM13,
	.timerPrescaler = (15999)

};

/* TIM14 Object */
TimerHandle TIMER_14 = {

	.timerInstance = TIM14,
	.timerPrescaler = (15999)

};
/* /\ End of Timer Object Definitions /\ */




/* \/ Start of function definitions \/ */

/*======================================*
 * 										*
 * Name: systick_init					*
 *  @args: void							*
 *  @param: void						*
 *  @brief: initialize systick			*
 * 										*
 *======================================*/
void systick_init(void)
{
	SysTick->CTRL |= ENPROCESSORCLK;
	SysTick->CTRL |= ENCOUNTERSYSCLK;

}


/*======================================*
 * 										*
 * Name: systick_delay_ms				*
 *  @args: uint32_t						*
 *  @param: uint8_t for when timer is   *
 *  		done						*
 *  @brief: initialize systick and 		*
 *  		start the timer based on	*
 *  		the length the user 		*
 *  		passes through in ms		*
 * 										*
 *======================================*/
uint8_t systick_delay_ms(uint32_t delay_ms)
{

	SysTick->LOAD = 16000 - 1U; // reload with number of clocks per millisecond
	SysTick->VAL = 0; // clear current value register


	systick_init();

	for(uint32_t i= 0; i< delay_ms; i++)
	{
		while((SysTick->CTRL & COUNTFLAG) == 0){} // wait until timer reaches 0
	}


	SysTick->CTRL = 0;

	return 1;

}


/*==========================================*
 * 											*
 * Name: timer_init							* APB1 timers: 2, 3, 4, 5, 12, 13, 14
 *  @args: TimerHandle*						* APB2 timers: 1, 8, 9, 10, 11
 *  @param: void							*
 *  @brief: initialize timer by giving 		*
 *  		the timer access to the 		*
 *  		clock, set PSC, force update	*
 *  		gen., reset timer, and      	*
 * 			clear update flag 		 		*
 * 											*
 *==========================================*/
void timer_init(TimerHandle *timer)
{

	if((timer == NULL) || (timer->timerInstance == NULL))
		{
			return;
		}

	if(timer->timerInstance == TIM1){
		RCC->APB2ENR |= RCCAPB2ENTIM1;
	}else if(timer->timerInstance == TIM2){
		RCC->APB1ENR |= RCCAPB1ENTIM2;
	}else if(timer->timerInstance == TIM3){
		RCC->APB1ENR |= RCCAPB1ENTIM3;
	}else if(timer->timerInstance == TIM4){
		RCC->APB1ENR |= RCCAPB1ENTIM4;
	}else if(timer->timerInstance == TIM5){
		RCC->APB1ENR |= RCCAPB1ENTIM5;
	}else if(timer->timerInstance == TIM6){
		RCC->APB1ENR |= RCCAPB1ENTIM6;
	}else if(timer->timerInstance == TIM7){
		RCC->APB1ENR |= RCCAPB1ENTIM7;
	}else if(timer->timerInstance == TIM8){
		RCC->APB2ENR |= RCCAPB2ENTIM8;
	}else if(timer->timerInstance == TIM9){
		RCC->APB2ENR |= RCCAPB2ENTIM9;
	}else if(timer->timerInstance == TIM10){
		RCC->APB2ENR |= RCCAPB2ENTIM10;
	}else if(timer->timerInstance == TIM11){
		RCC->APB2ENR |= RCCAPB2ENTIM11;
	}else if(timer->timerInstance == TIM12){
		RCC->APB1ENR |= RCCAPB1ENTIM12;
	}else if(timer->timerInstance == TIM13){
		RCC->APB1ENR |= RCCAPB1ENTIM13;
	}else if(timer->timerInstance == TIM14){
		RCC->APB1ENR |= RCCAPB1ENTIM14;
	}else{
		return;
	}

	// Setting prescaler value
	timer->timerInstance->PSC = timer->timerPrescaler;

	// Set update generation to force the new prescaler value into the register
	timer->timerInstance->EGR |= TIM_EGR_UG;

	// Reset timer count
	timer->timerInstance->CNT = 0x0000;

	// Clear update flag
	timer->timerInstance->SR &=~ TIMERUPDATEINTERRUPTFLAG;

}


/*==========================================*
 * 							 	     		*
 * Name: startTimer  		         		*
 * 	@args: uint32_t , TimerHandle*    		*
 * 	@param: void			 	     		*
 * 	@brief: start the specified 			*
 * 		timer for counting based	 		*
 * 		on the amount of time given 		*
 * 		by the user and the specific timer  *
 * 		and return if timer start based on  *
 * 		if args are valid				    *
 * 		 					 				*
 *==========================================*/
uint8_t startTimer(uint32_t durationMs, TimerHandle *timer)
{

	if((durationMs == 0) 															||
	   (timer == NULL) 																||
	   (timer->timerInstance  == NULL) 												||
	   ((timer->timerInstance == TIM1)  && (durationMs  > MAX16BITTIMERVALUE)) 		||
	   ((timer->timerInstance == TIM8)  && (durationMs  > MAX16BITTIMERVALUE)) 		||
	   ((timer->timerInstance == TIM3)  && (durationMs  > MAX16BITTIMERVALUE)) 		||
	   ((timer->timerInstance == TIM4)  && (durationMs  > MAX16BITTIMERVALUE))  	||
	   ((timer->timerInstance == TIM9)  && (durationMs  > MAX16BITTIMERVALUE))  	||
	   ((timer->timerInstance == TIM10) && (durationMs  > MAX16BITTIMERVALUE)) 		||
	   ((timer->timerInstance == TIM11) && (durationMs  > MAX16BITTIMERVALUE)) 		||
	   ((timer->timerInstance == TIM12) && (durationMs  > MAX16BITTIMERVALUE)) 		||
	   ((timer->timerInstance == TIM13) && (durationMs  > MAX16BITTIMERVALUE)) 		||
	   ((timer->timerInstance == TIM14) && (durationMs  > MAX16BITTIMERVALUE)) 		||
	   ((timer->timerInstance == TIM6)  && (durationMs  > MAX16BITTIMERVALUE))  	||
	   ((timer->timerInstance == TIM7)  && (durationMs  > MAX16BITTIMERVALUE)))
	{
		return 0;
	}
	else
	{

	// Disable counter before config
	timer->timerInstance->CR1 &=~ COUNTERENABLE;

	// Set value where update event happens
	timer->timerInstance->ARR = durationMs - 1;

	// Reset timer count
	timer->timerInstance->CNT = 0x0000;

	// Clear update flag
	timer->timerInstance->SR &=~ TIMERUPDATEINTERRUPTFLAG;

	// Enable counter
	timer->timerInstance->CR1 |= COUNTERENABLE;

	return 1;

	}


}


/*==========================================*
 * 							 	     		*
 * Name: check_timer_flag            		*
 * 	@args: TimerHandle*  			  		*
 * 	@param: uint8_t		 	     			*
 * 	@brief: start the specified 			*
 * 		timer for counting based	 		*
 * 		on the amount of time given 		*
 * 		by the user and the specific timer  *
 * 		 					 				*
 *==========================================*/
uint8_t check_timer_flag(TimerHandle *timer)
{

	if((timer == NULL) || (timer->timerInstance == NULL))
	{
		return 0;
	}

	if((timer->timerInstance->SR & TIMERUPDATEINTERRUPTFLAG) == 1)
	{
		// Reset update event flag
		timer->timerInstance->SR &=~ TIMERUPDATEINTERRUPTFLAG;

		// Disable counter
		timer->timerInstance->CR1 &=~ COUNTERENABLE;
		return 1;
	}else
	{
		return 0;
	}
}
/* /\ End of function definitions /\ */

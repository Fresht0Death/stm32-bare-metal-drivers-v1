/*
 * timer.h
 *
 *  Created on: Jul 1, 2026
 *      Author: Freshmai
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include "stm32f4xx.h"

/* \/ Timer Struct \/ */

typedef struct{

	TIM_TypeDef* timerInstance;
	uint16_t timerPrescaler;


} TimerHandle;
/* /\ End of Timer Struct /\ */


/* \/ Start of extending the scope of our timer handles \/ */

extern TimerHandle TIMER_1;
extern TimerHandle TIMER_2;
extern TimerHandle TIMER_3;
extern TimerHandle TIMER_4;
extern TimerHandle TIMER_5;
extern TimerHandle TIMER_6;
extern TimerHandle TIMER_7;
extern TimerHandle TIMER_8;
extern TimerHandle TIMER_9;
extern TimerHandle TIMER_10;
extern TimerHandle TIMER_11;
extern TimerHandle TIMER_12;
extern TimerHandle TIMER_13;
extern TimerHandle TIMER_14;
/* /\ End of extending the scope of our timer handles /\ */


/* \/ Start of function prototypes \/ */

/*======================================*
 * 										*
 * Name: systick_init					*
 *  @args: void							*
 *  @param: void						*
 *  @brief: initialize systick			*
 * 										*
 *======================================*/
void systick_init(void);


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
uint8_t systick_delay_ms(uint32_t delay_ms);


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
void timer_init(TimerHandle *timer);


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
uint8_t startTimer(uint32_t durationMs, TimerHandle *timer);


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
uint8_t check_timer_flag(TimerHandle *timer);
/* /\ End of function prototypes /\ */



#endif /* TIMER_H_ */

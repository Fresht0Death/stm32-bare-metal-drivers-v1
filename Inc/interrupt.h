/*
 * interrupt.h
 *
 *  Created on: Jul 17, 2026
 *      Author: Freshmai
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <stdint.h>


/*======================================*
 * 										*
 *  Name: pc13_user_button_exti_init  	*
 *   @param: N/A						*
 *   @brief: start user button B1		*
 *   interrupt							*
 *   @return: N/A						*
 * 										*
 *======================================*/
void pc13_user_button_exti_init(void);


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
void tim2_periodic_interrupt_init_ms(uint32_t durationMs);


/*======================================*
 * 										*
 *  Name: usart2_interrupt_int	    	*
 *   @param: N/A						*
 *   @brief: enable usart2 rx interrupt	*
 *   @return: N/A						*
 * 										*
 *======================================*/
void usart2_interrupt_init(void);



#endif /* INTERRUPT_H_ */

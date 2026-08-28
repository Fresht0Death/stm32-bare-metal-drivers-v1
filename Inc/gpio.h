/*
 * gpio.h
 *
 *  Created on: Jun 11, 2026
 *      Author: Freshmai
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>


/*======================================*
 * 										*
 * Name: toggle_pa5_led					*
 *  @args: void							*
 *  @param: void						*
 *  @brief: toggles the on-board led		*
 * 										*
 *======================================*/
void toggle_pa5_led(void);


/*======================================*
 * 										*
 * Name: gpioa_set_mode					*
 *  @args: uint8_t, uint8_t				*
 *  @param: void						*
 *  @brief: set the mode of any pin		*
 *  		in GPIOA					*
 * 										*
 *======================================*/
void gpioa_set_mode(uint8_t mode, uint8_t pin);


/*======================================*
 * 										*
 * Name: gpioa_enable					*
 *  @args: void							*
 *  @param: void						*
 *  @brief: enable clock access to 		*
 *  		GPIOA						*
 * 										*
 *======================================*/
void gpioa_enable(void);


/*======================================*
 * 										*
 * Name: delay							*
 *  @args: void							*
 *  @param: void						*
 *  @brief: starts a blocking delay		*
 * 										*
 *======================================*/
void delay(void);


#define GPIOAEN		    (1U<<0)
#define PA5				(1U<<5)




#endif /* GPIO_H_ */

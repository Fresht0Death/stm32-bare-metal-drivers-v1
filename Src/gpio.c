/*
 * gpio.c
 *
 *  Created on: Jun 11, 2026
 *      Author: Freshmai
 */

/* \/  Start of libraries \/ */

#include "stm32f4xx.h"
#include <stdio.h>
#include "gpio.h"
/* /\  End of libraries /\ */

/* \/  Start of function definitions \/ */

/*======================================*
 * 										*
 * Name: gpioa_enable					*
 *  @args: void							*
 *  @param: void						*
 *  @brief: enable clock access to 		*
 *  		GPIOA						*
 * 										*
 *======================================*/
void gpioa_enable(void)
{

	RCC->AHB1ENR |= GPIOAEN;

}


/*======================================*
 * 										*
 * Name: gpioa_set_mode					*
 *  @args: uint8_t, uint8_t				*
 *  @param: void						*
 *  @brief: set the mode of any pin		*
 *  		in GPIOA					*
 * 										*
 *======================================*/
void gpioa_set_mode(uint8_t mode, uint8_t pin)
{

	/*
	 * 	Different Modes:
	 *
	 * 0 = 	00: Input (reset state)
	 * 1 = 	01: General purpose output mode
	 * 2 =	10: Alternate function mode
	 * 3 =	11: Analog mode
	 *
	 * */


	/* Only for modes allowed therefore our range is from 0-3*/
	if(((mode < 0) || (mode > 3)))
	{
        fprintf(stderr, "Error: Acceptable mode range [0,3] (%d).\r\n", mode);
        return;


	}else if(((pin < 0) || (pin > 15)))
	{
		fprintf(stderr, "Error: Acceptable pin range [0,15] (%d).\r\n", pin);
		return;
	}
	else{

		int8_t bitPin1 = pin 	 * 2;
		int8_t bitPin2 = bitPin1 + 1;



		switch(mode)
		{
		case 0:
			GPIOA->MODER &=~ (1U<<bitPin1);
			GPIOA->MODER &=~ (1U<<bitPin2);
			break;
		case 1:
			GPIOA->MODER |= (1U<<bitPin1);
			GPIOA->MODER &=~ (1U<<bitPin2);
			break;
		case 2:
			GPIOA->MODER &=~ (1U<<bitPin1);
			GPIOA->MODER |= (1U<<bitPin2);
			break;
		case 3:
			GPIOA->MODER |= (1U<<bitPin1);
			GPIOA->MODER |= (1U<<bitPin2);
			break;
		default:
			break;


		}



	}



}



/*======================================*
 * 										*
 * Name: toggle_pa5_led					*
 *  @args: void							*
 *  @param: void						*
 *  @brief: toggles the on-board led		*
 * 										*
 *======================================*/
void toggle_pa5_led(void)
{
	gpioa_enable();

	gpioa_set_mode(1, 5);

	GPIOA->ODR ^= PA5;

}


/*======================================*
 * 										*
 * Name: delay							*
 *  @args: void							*
 *  @param: void						*
 *  @brief: starts a blocking delay		*
 * 										*
 *======================================*/
void delay(void)
{

	for(int i = 0; i < 1000000; i++);


}
/* /\  End of function definitions /\ */


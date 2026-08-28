/*
 * adc.c
 *
 *  Created on: Jul 18, 2026
 *      Author: Freshmai
 */


/* \/ Start of Libraries \/ */

#include "stm32f4xx.h"
#include <stdint.h>
#include "timer.h"
#include "stm_err.h"
/* /\ End of Libraries /\ */

/* \/ Start of Macros \/ */

#define ADC1EN					(1U<<8)
#define GPIOAEN					(1U<<0)
#define ADC1CONT				(1U<<1)
#define ADC1_SR_EOC				(1U<<1)
#define ADC2EN					(1U<<9)
#define GPIOAEN					(1U<<0)
#define ADC2ADON				(1U<<0)
#define ADC2EOCIE				(1U<<5)
#define ADC2CONT				(1U<<1)

#define ADON_CR2				(1U<<0)
#define SWSTART_CR2				(1U<<30)

#define TIM_UIF					(1U<<0)

volatile uint32_t sumSamples = 0;
volatile uint32_t numSamples = 0;
/* /\ End of Macros /\ */

/* \/ Start of function definitions \/ */

/*======================================*
 * 										*
 * Name: single_polling_adc1_pa0_init	*
 *  @args: void							*
 *  @param: my_err_t to check status	*
 *  @brief: initialize adc1 via pa0 for *
 *  single polling						*
 * 										*
 *======================================*/
my_err_t single_polling_adc1_pa0_init(void)
{
	// Enable clock access to ADC1
	RCC->APB2ENR |=	ADC1EN;

	// Enable clock to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// Set PA0 to analog mode
	GPIOA->MODER |= (1U<<0);
	GPIOA->MODER |= (1U<<1);

	// Set number of conversion per sequence to 1
	ADC1->SQR1 &=~ (1U<<20);
	ADC1->SQR1 &=~ (1U<<21);
	ADC1->SQR1 &=~ (1U<<22);
	ADC1->SQR1 &=~ (1U<<23);

	// Set channel 0 to the first conversion in the sequence
	ADC1->SQR3 &=~ (1U<<0);
	ADC1->SQR3 &=~ (1U<<1);
	ADC1->SQR3 &=~ (1U<<2);
	ADC1->SQR3 &=~ (1U<<3);
	ADC1->SQR3 &=~ (1U<<4);

	// Disable continuous conversion mode
	ADC1->CR2 &=~ ADC1CONT;

	// Set sampling time to 84 cycles : 100 for channel 0
	ADC1->SMPR2 &=~ (1U<<0);
	ADC1->SMPR2 &=~ (1U<<1);
	ADC1->SMPR2 |= 	(1U<<2);

	// Turn ADC1 on
	ADC1->CR2 |= ADON_CR2;

	// short stabilization delay
	for(int i =0; i <10000; i++);

	return MY_OK;

}


/*==========================================*
 * 											*
 * Name: read_adc1_pa0_value				*
 *  @args: void								*
 *  @param: uint32_t that returns adc value	*
 *  @brief: start conversion and wait until *
 *  analog to digital conversion complete	*
 *  										*
 *==========================================*/
uint32_t read_adc1_pa0_value(void)
{
	// Start conversion of regular channels
	ADC1->CR2 |= SWSTART_CR2;

	// loop while conversion hasn't ended
	while(!(ADC1->SR & ADC1_SR_EOC));

	// return current digital value
	return ADC1->DR;


}


/*==========================================*
 * 											*
 * Name: adc_to_voltage3V3_integer_part		*
 *  @args: uint32_t adc_value				*
 *  @param: uint32_t whole number part of   *
 *  					of voltage	        *
 *  @brief: convert adc to voltage when 	*
 *  		connected to 3.3V and returns 	*
 *  		the whole number part of the	*
 *  		voltage							*
 *  										*
 *==========================================*/
uint32_t adc_to_voltage3V3_integer_part(uint32_t adc_value)
{

	return ((adc_value * 3300U)/(4096U));

}


/*==========================================*
 * 											*
 * Name: adc_to_voltage3V3_decimal_part		*
 *  @args: uint32_t adc_value				*
 *  @param: uint32_t fraction part of       *
 *  					of voltage	        *
 *  @brief: convert adc to voltage when 	*
 *  		connected to 3.3V and returns 	*
 *  		the fraction part of the	    *
 *  		voltage							*
 *  										*
 *==========================================*/
uint32_t adc_to_voltage3V3_decimal_part(uint32_t adc_value)
{

	uint32_t remainder = ((adc_value * 3300U)%(4096U));

	uint32_t decimal_part = (remainder * 1000U) / 4096U;

	return decimal_part;

}


/*==========================================*
 * 											*
 * Name: adc_to_voltage5V_integer_part		*
 *  @args: uint32_t adc_value				*
 *  @param: uint32_t whole number part of   *
 *  					of voltage	        *
 *  @brief: convert adc to voltage when 	*
 *  		connected to 5V and returns 	*
 *  		the whole number part of the	*
 *  		voltage							*
 *  										*
 *==========================================*/
uint32_t adc_to_voltage5V_integer_part(uint32_t adc_value)
{

	return ((adc_value * 5U)/(4096U));

}


/*==========================================*
 * 											*
 * Name: adc_to_voltage5V_decimal_part		*
 *  @args: uint32_t adc_value				*
 *  @param: uint32_t fraction part of       *
 *  					of voltage	        *
 *  @brief: convert adc to voltage when 	*
 *  		connected to 5V and returns 	*
 *  		the fraction part of the	    *
 *  		voltage							*
 *  										*
 *==========================================*/
uint32_t adc_to_voltage5V_decimal_part(uint32_t adc_value)
{

	uint32_t remainder = ((adc_value * 5U)%(4096U));

	uint32_t decimal_part = (remainder * 1000U) / 4096U;

	return decimal_part;

}


/*==========================================*
 * 											*
 * Name: adc1_pa0_avg_voltage				*
 *  @args: uint32_t sample					*
 *  @param: uint32_t avg of adc1 samples    *
 *  @brief: calculates the avg voltage of   *
 *  		the adc1 values via pa0			*
 *  										*
 *==========================================*/
uint32_t adc1_pa0_avg_voltage(uint32_t sample)
{
	sumSamples = sample + sumSamples;
	numSamples++;

	return (sumSamples / numSamples);
}


/*==========================================*
 * 											*
 * Name: num_of_samples						*
 *  @args: void								*
 *  @param: uint32_t number of samples      *
 *  @brief: returns current number of 		*
 *  		samples that have been 			*
 *  		calculated by adc1				*
 *  										*
 *==========================================*/
uint32_t num_of_samples(void)
{
	return numSamples;
}


/*==========================================*
 * 											*
 * Name: adc2_pa4_interrupt_oneshot_init	*
 *  @args: void								*
 *  @param: void							*
 *  @brief: initialize adc2 via pa4 	 	*
 *  		to do one adc cycle				*
 * 											*
 *==========================================*/
void adc2_pa4_interrupt_oneshot_init(void)
{

	// Enable clock access to ADC2
	RCC->APB2ENR |= ADC2EN;

	// Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// Set PA4 to analog mode
	GPIOA->MODER |= (1U<<8);
	GPIOA->MODER |= (1U<<9);

	// ADC2 part of channel 4 to first conversion in the sequence
	// PA4 : ADC12_IN4 (datasheet)
	ADC2->SQR3 &=~ (1U<<0);
	ADC2->SQR3 &=~ (1U<<1);
	ADC2->SQR3 |=  (1U<<2);
	ADC2->SQR3 &=~ (1U<<3);
	ADC2->SQR3 &=~ (1U<<4);

	// Disable continuous conversion
	ADC2->CR2 &=~ (1U<<1);

	// Set sampling time for Channel 4 to 84 cycles
	ADC2->SMPR2 &=~ (1<<12);
	ADC2->SMPR2 &=~ (1<<13);
	ADC2->SMPR2 |= (1<<14);

	// Turn on ADC2
	ADC2->CR2 |= ADC2ADON;

	// short stabilization delay
	for(int i =0; i <10000; i++);

	// Enable ADC2 interrupt
	ADC2->CR1 |= ADC2EOCIE;

	// Enable ADC interrupt in NVIC
	NVIC_EnableIRQ(ADC_IRQn);

	// Start conversion of regular channels
	ADC2->CR2 |= SWSTART_CR2;

}


/*==========================================*
 * 											*
 * Name: adc2_pa4_interrupt_continuous_init	*
 *  @args: void								*
 *  @param: void							*
 *  @brief: initialize adc2 via pa4 for 	*
 *  		to be continuously converting	*
 *  		values through an interrupt		*
 * 											*
 *==========================================*/
void adc2_pa4_interrupt_continuous_init(void)
{
	// Enable clock access to ADC2
		RCC->APB2ENR |= ADC2EN;

		// Enable clock access to GPIOA
		RCC->AHB1ENR |= GPIOAEN;

		// Set PA4 to analog mode
		GPIOA->MODER |= (1U<<8);
		GPIOA->MODER |= (1U<<9);

		// ADC2 part of channel 4 to first conversion in the sequence
		// PA4 : ADC12_IN4 (datasheet)
		ADC2->SQR3 &=~ (1U<<0);
		ADC2->SQR3 &=~ (1U<<1);
		ADC2->SQR3 |=  (1U<<2);
		ADC2->SQR3 &=~ (1U<<3);
		ADC2->SQR3 &=~ (1U<<4);

		// Enable continuous conversion
		ADC2->CR2 |= (1U<<1);

		// Set sampling time for Channel 4 to 84 cycles
		ADC2->SMPR2 &=~ (1<<12);
		ADC2->SMPR2 &=~ (1<<13);
		ADC2->SMPR2 |= (1<<14);

		// Turn on ADC2
		ADC2->CR2 |= ADC2ADON;

		// short stabilization delay
		for(int i =0; i <10000; i++);

		// Enable ADC2 interrupt
		ADC2->CR1 |= ADC2EOCIE;

		// Enable ADC interrupt in NVIC
		NVIC_EnableIRQ(ADC_IRQn);

		// Start conversion of regular channels
		ADC2->CR2 |= SWSTART_CR2;


}


/*==========================================*
 * 											*
 * Name: adc1_pa0_single_polling_timed_init	*
 *  @args: uint32_t, TimerHandle*								*
 *  @param: void							*
 *  @brief: initialize adc1 via pa0 for 	*
 *  		to return its based on the 		*
 *  		timed sequence chosen by the	*
 *  		user							*
 * 											*
 *==========================================*/
void adc1_pa0_single_polling_timed_init(uint32_t delayMs, TimerHandle *timer)
{
	single_polling_adc1_pa0_init();

	timer_init(timer);

	startTimer(delayMs, timer);

}
/* /\ End of function definitions /\ */


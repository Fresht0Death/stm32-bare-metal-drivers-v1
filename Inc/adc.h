/*
 * adc.h
 *
 *  Created on: Jul 18, 2026
 *      Author: Freshmai
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm_err.h"
#include <stdint.h>
#include "stm32f4xx.h"
#include "timer.h"


/*======================================*
 * 										*
 * Name: single_polling_adc1_pa0_init	*
 *  @args: void							*
 *  @param: my_err_t to check status	*
 *  @brief: initialize adc1 via pa0 for *
 *  single polling						*
 * 										*
 *======================================*/
my_err_t single_polling_adc1_pa0_init(void);


/*==========================================*
 * 											*
 * Name: read_adc1_pa0_value				*
 *  @args: void								*
 *  @param: uint32_t that returns adc value	*
 *  @brief: start conversion and wait until *
 *  analog to digital conversion complete	*
 *  										*
 *==========================================*/
uint32_t read_adc1_pa0_value(void);


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
uint32_t adc_to_voltage3V3_integer_part(uint32_t adc_value);


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
uint32_t adc_to_voltage3V3_decimal_part(uint32_t adc_value);


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
uint32_t adc_to_voltage5V_integer_part(uint32_t adc_value);


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
uint32_t adc_to_voltage5V_decimal_part(uint32_t adc_value);


/*==========================================*
 * 											*
 * Name: adc1_pa0_avg_voltage				*
 *  @args: uint32_t sample					*
 *  @param: uint32_t avg of adc1 samples    *
 *  @brief: calculates the avg voltage of   *
 *  		the adc1 values via pa0			*
 *  										*
 *==========================================*/
uint32_t adc1_pa0_avg_voltage(uint32_t sample);


/*==========================================*
 * 											*
 * Name: adc2_pa4_interrupt_oneshot_init	*
 *  @args: void								*
 *  @param: void							*
 *  @brief: initialize adc2 via pa4 	 	*
 *  		to do one adc cycle				*
 * 											*
 *==========================================*/
void adc2_pa4_interrupt_oneshot_init(void);


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
void adc2_pa4_interrupt_continuous_init(void);


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
void adc1_pa0_single_polling_timed_init(uint32_t delayMs, TimerHandle *timer);


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
uint32_t num_of_samples(void);

#endif /* ADC_H_ */

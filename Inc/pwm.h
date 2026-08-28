/*
 * pwm.h
 *
 *  Created on: Jul 21, 2026
 *      Author: Freshmai
 */

#ifndef PWM_H_
#define PWM_H_

#include "stm_err.h"
#include "stm32f4xx.h"
#include <stdint.h>

typedef struct
{
	TIM_TypeDef *pwminstance;
	uint32_t pwmPrescaler;
	uint32_t pwmFrequency;
	uint32_t pwmDutyCycle;
	uint32_t pwmArr;


} PWMHandle;

extern PWMHandle TIM1_PWM;
extern PWMHandle TIM8_PWM;


/*==========================================*
 * 											*
 * Name: pwm_set_tim8_duty_cycle			*
 *  @args: uint16_t							*
 *  @param: void							*
 *  @brief: sets TIM8/PWM duty cycle        *
 * 											*
 *==========================================*/
void pwm_set_tim8_duty_cycle(uint16_t duty);


/*==========================================*
 * 											*
 * Name: tim_pwm_init						*
 *  @args: PWMHandle*						*
 *  @param: my_err_t to check status		*
 *  @brief: initializes a timer for pwm     *
 * 											*
 *==========================================*/
my_err_t tim_pwm_init(PWMHandle *pwmTimer);


/*==========================================*
 * 											*
 * Name: pwm_set_frequency					*
 *  @args: PWMHandle*, uint32_t, uint32_t	*
 *  @param: my_err_t to check status		*
 *  @brief: set the frequency of the pwm    *
 * 											*
 *==========================================*/
my_err_t pwm_set_frequency(PWMHandle *pwmTimer, uint32_t Prescaler, uint32_t Arr);


my_err_t pwm_set_duty_ticks(PWMHandle *pwmTimer, uint32_t duty_ticks);

#endif /* PWM_H_ */

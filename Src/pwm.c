/*
 * pwm.c
 *
 *  Created on: Jul 21, 2026
 *      Author: Freshmai
 */

/* \/ Start of libraries \/ */

#include "stm32f4xx.h"
#include <stdint.h>
#include "pwm.h"
#include "stm_err.h"
#include <stddef.h>
/* /\ End of libraries /\ */

/* \/ Start of Macros \/ */

#define TIM8EN					(1U<<1)
#define GPIOAEN					(1U<<0)
#define OC1PE					(1U<<3)
#define MOE						(1U<<15)
#define TIMCEN					(1U<<0)
#define TIM1EN					(1U<<0)
/* /\ End of Macros /\ */



/* \/ Start of PWM object definitions \/ */

PWMHandle TIM8_PWM = {

		.pwminstance = TIM8,
		.pwmPrescaler = 16- 1


};

PWMHandle TIM1_PWM = {

		.pwminstance = TIM1,
		.pwmPrescaler = 16- 1

};
/* /\ End of PWM object definitions /\ */


/* \/ Start of function definitions \/ */

/*==========================================*
 * 											*
 * Name: pwm_set_frequency					*
 *  @args: PWMHandle*, uint32_t, uint32_t	*
 *  @param: my_err_t to check status		*
 *  @brief: set the frequency of the pwm    *
 * 											*
 *==========================================*/
my_err_t pwm_set_frequency(PWMHandle *pwmTimer, uint32_t prescaler_div, uint32_t period_ticks)
{
	if(pwmTimer == NULL || pwmTimer->pwminstance == NULL)
	{
		return MY_ERR_BAD_ARG;
	}
	if(prescaler_div == 0 || period_ticks == 0)
	{
		return MY_ERR_BAD_ARG;
	}



	pwmTimer->pwminstance->PSC = prescaler_div - 1;
	pwmTimer->pwminstance->ARR = period_ticks - 1;

	pwmTimer->pwmPrescaler = prescaler_div;
	pwmTimer->pwmArr = period_ticks;
	pwmTimer->pwmFrequency = 16000000U / (prescaler_div * period_ticks);

	return MY_OK;


}


/*==========================================*
 * 											*
 * Name: tim_pwm_init						*
 *  @args: PWMHandle*						*
 *  @param: my_err_t to check status		*
 *  @brief: initializes a timer for pwm     *
 * 											*
 *==========================================*/
my_err_t tim_pwm_init(PWMHandle *pwmTimer)
{

	if(pwmTimer == NULL || pwmTimer->pwminstance == NULL)
	{
		return MY_ERR_BAD_ARG;
	}


	if(pwmTimer->pwminstance == TIM1)
	{
		// Enable clock access to TIM1
		RCC->APB2ENR |= TIM1EN;

		// Enable clock access to GPIOA
		RCC->AHB1ENR |= GPIOAEN;

		// Set PA7 to alternate mode
		GPIOA->MODER |= (1U<<15);
		GPIOA->MODER &=~ (1U<<14);

		// Set alternate function register for PA7 to AF1(0001)
		GPIOA->AFR[0] |= (1U<<28);
		GPIOA->AFR[0] &=~ (1U<<29);
		GPIOA->AFR[0] &=~ (1U<<30);
		GPIOA->AFR[0] &=~ (1U<<31);



	}else if(pwmTimer->pwminstance == TIM8)
	{
		//Enable clock access to TIM8
		RCC->APB2ENR |= TIM8EN;

		// Enable clock access to GPIOA
		RCC->AHB1ENR |= GPIOAEN;

		// Set PA5 as alternate mode
		GPIOA->MODER |= (1U<<11);
		GPIOA->MODER &=~ (1U<<10);

		// Set alternate function register for PA5 to AF3
		GPIOA->AFR[0] |= (1U<<20);
		GPIOA->AFR[0] |= (1U<<21);
		GPIOA->AFR[0] &=~ (1U<<22);
		GPIOA->AFR[0] &=~ (1U<<23);


	}else
	{
		return MY_ERR_BAD_ARG;
	}

	// setting default frequency of 1 KHz based on 16 MHz default clock assumption
	pwm_set_frequency(pwmTimer, 16, 1000);

	// Configure Channel Channel 1 PWM Mode 1
	// Clear the output compare mode bits and set to 110 (PWM 1)
	pwmTimer->pwminstance->CCMR1 &=~ (1U<<4);
	pwmTimer->pwminstance->CCMR1 &=~ (1U<<5);
	pwmTimer->pwminstance->CCMR1 &=~ (1U<<6);

	pwmTimer->pwminstance->CCMR1 &=~ (1U<<4);
	pwmTimer->pwminstance->CCMR1 |= (1U<<5);
	pwmTimer->pwminstance->CCMR1 |= (1U<<6);

	// Enable preload (so changes to CCR1 register until next update)
	pwmTimer->pwminstance->CCMR1 |= OC1PE;

	// Enable main output (this is only used in advanced timers)
	pwmTimer->pwminstance->BDTR |= MOE;

	// Set initial duty cycle to 50%
	pwmTimer->pwminstance->CCR1 = 500;

	// Enable complimentary Channel 1 output
	pwmTimer->pwminstance->CCER |= (1U<<2);

	// Enable the timer
	pwmTimer->pwminstance->CR1 |= TIMCEN;

	return MY_OK;

}


/*==========================================*
 * 											*
 * Name: pwm_set_tim8_duty_cycle			*
 *  @args: uint16_t							*
 *  @param: void							*
 *  @brief: sets TIM8/PWM duty cycle        *
 * 											*
 *==========================================*/
void pwm_set_tim8_duty_cycle(uint16_t duty)
{
	if(duty > TIM8->ARR)
	{
		duty = TIM8->ARR;
	}

	TIM8->CCR1 = duty;


}

my_err_t pwm_set_duty_ticks(PWMHandle *pwmTimer, uint32_t duty_ticks)
{
    if(pwmTimer == NULL || pwmTimer->pwminstance == NULL)
    {
        return MY_ERR_BAD_ARG;
    }

    if(duty_ticks > pwmTimer->pwminstance->ARR)
    {
        duty_ticks = pwmTimer->pwminstance->ARR;
    }

    pwmTimer->pwminstance->CCR1 = duty_ticks;
    pwmTimer->pwmDutyCycle = duty_ticks;

    return MY_OK;
}
/* /\ End of function definitions /\ */


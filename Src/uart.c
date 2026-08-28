/*
 * uart.c
 *
 *  Created on: Jun 23, 2026
 *      Author: Freshmai
 */

/* \/ Start of libraries \/' */

#include "stm32f4xx.h"
#include <stdio.h>
#include "stm_err.h"
/* /\ End of libraries /\  */


/* \/ Start of  Macros definitions \/ */

// Enable clock access to USART2
#define RCCAPB1USART2EN				(1U<<17)

// Enable clock access to GPIOA
#define GPIOAEN						(1U<<0)

// Enable transmitter for USART
#define USARTTXEN					(1U<<3)

// Enable USART
#define USARTUE						(1U<<13)

// Baudrate
#define BAUDRATE					(115200)

// default clock speeds
#define APB1_16MHZ					(16000000)
#define APB2_16MHZ					(16000000)

// USART transmit data register empty
#define USARTSRTXE					(1U<<7)

// Enable receiving for USART2
#define USART2RXEN					(1U<<2)

// Flag to see if data has been received
#define USART2RXNE					(1U<<5)

// Enable clock access to APB2
#define RCCAPB2USART1EN				(1U<<4)
/* /\ End of Macros definition /\ */


/* \/ Function Definitions \/ */

/*===========================================*
 *      						             * USART[1,2,3,6]
 * 	Name: set_usart_baudrate        		 * UART [4,5]
 * 		@args: int ,int ,uint8_t             *
 *  	@param: void	                     *
 *  	@brief: set the baudrate for usart   *
 *  	based on the periph clock speed,     *
 *  	user desired baudrate, and which     *
 *  	usart is being used		             *
 *  										 *
 *  	Default baudrate: 115200             *
 *  	Default periph_clock: 16 MHz     	 *
 * 									         *
 *===========================================*/
void set_usart_baudrate(int baudrate, int periph_clk, uint8_t usartNumber)
{

	switch(usartNumber)
	{
	case 1:
		// 1. Clear the USART Enable (UE) bit before changing configurations
		USART1->CR1 &=~ USARTUE;

		// 2. Calculate and assign the BRR value using integer rounding
		USART1->BRR = (periph_clk + (baudrate / 2U)) / baudrate;

		/* Enable USART */
		USART1->CR1 |= USARTUE;
		break;
	case 2:
		// 1. Clear the USART Enable (UE) bit before changing configurations
		USART2->CR1 &=~ USARTUE;

		// 2. Calculate and assign the BRR value using integer rounding
		USART2->BRR = (periph_clk + (baudrate / 2U)) / baudrate;

		/* Enable USART */
		USART2->CR1 |= USARTUE;
		break;
	case 3:
		// 1. Clear the USART Enable (UE) bit before changing configurations
		USART3->CR1 &=~ USARTUE;

		// 2. Calculate and assign the BRR value using integer rounding
		USART3->BRR = (periph_clk + (baudrate / 2U)) / baudrate;

		/* Enable USART */
		USART3->CR1 |= USARTUE;
		break;
	case 4:
		// Not yet setup for uart4
		break;
	case 5:
		// not yet setup for uart5
		break;
	case 6:
		// 1. Clear the USART Enable (UE) bit before changing configurations
		USART6->CR1 &=~ USARTUE;

		// 2. Calculate and assign the BRR value using integer rounding
		USART6->BRR = (periph_clk + (baudrate / 2U)) / baudrate;

		/* Enable USART */
		USART6->CR1 |= USARTUE;
		break;
	default:
		break;

	}

}


/*======================================*
 *      						        *
 * 	Name: usart2_write         			*
 * 		@args: int                      *
 *  	@param: int                     *
 *  	@brief: transmit char passed    *
 *  	by user to terminal 			*
 *      						        *
 *======================================*/
int usart2_write(int chr)
{
	/* Wait while TX buffer is empty */
	while(!(USART2->SR & USARTSRTXE)){}

	/* Pass char to the data register */
	USART2->DR = (chr & 0xFF);

	return chr;
}


/*======================================*
 *      						        *
 * 	Name: usart2_read      		        *
 * 		@args: void                     *
 *  	@param: int                     *
 *  	@brief: read input from         *
 *      the terminal					*
 *      						        *
 *======================================*/
int usart2_read(void)
{
	/* Wait until receive data register is empty */
	while(!(USART2->SR & USART2RXNE)){}

	// return what is in the register
	return USART2->DR;

}


/*======================================*
 *      						        *
 * 	Name: usart2_init          			*
 * 		@args: void                     * PA2: USART2_TX, AFR[8,11] = AF7
 *  	@param: void               	    * PA3: USART2_RX, AFR[12,15] = AF7
 *  	@brief: grants access to the    *
 *  	clock for usart2 and GPIOA,     *
 *  	sets up gpio pins(PA2 and PA3)  *
 *  	for alt func mode to be used    *
 *  	for rx and tx for uart, and     *
 *  	sets baudrate to 115200			*
 *      								*
 *======================================*/
my_err_t usart2_init(void)
{

	// usart2_init rx code
	/* Enable clock access to USART2 */
		RCC->APB1ENR |= RCCAPB1USART2EN;

		/* Enable clock access to GPIOA */
		RCC->AHB1ENR |= GPIOAEN;

		// clear afr[0] pin3 bits to 0s
		GPIOA->AFR[0] &=~ (0x0000F000);

		/* Set alternate function to PIN3 for AF7 */
		GPIOA->AFR[0] |=  (1U<<12);
		GPIOA->AFR[0] |=  (1U<<13);
		GPIOA->AFR[0] |=  (1U<<14);
		GPIOA->AFR[0] &=~ (1U<<15);

		// clear pa3 MODER pins to 0s
		GPIOA->MODER &=~ (0x000000C0);

		/* Set alternate mode for GPIO PA3 */
		GPIOA->MODER &=~ (1U<<6);
		GPIOA->MODER |=  (1U<<7);

		/* Enable RX(receiving) */
		USART2->CR1 |= USART2RXEN;


		//clear afr[0] pin2 bits to 0s
		GPIOA->AFR[0] &=~ (0x00000F00);

		/* Set alternate function to PIN2 for AF7 */
		GPIOA->AFR[0] |=  (1U<<8);
		GPIOA->AFR[0] |=  (1U<<9);
		GPIOA->AFR[0] |=  (1U<<10);
		GPIOA->AFR[0] &=~ (1U<<11);

		// Clear MODER bits for PA2
		GPIOA->MODER &=~ (0x00000030);

		/* Set alternate mode for GPIO PA2 */
		GPIOA->MODER &=~ (1U<<4);
		GPIOA->MODER |=  (1U<<5);

		/* Enable TX(transmission) */
		USART2->CR1 |= USARTTXEN;

		// Setting usart2 baudrate
		set_usart_baudrate(BAUDRATE, APB1_16MHZ, 2);

		return MY_OK;

}


/*=======================================*
 *      						         *
 * 	Name: __io_putchar          		 *
 * 		@args: int                       *
 *  	@param: int                 	 *
 *  	@brief: we overwrite the C       *
 *  	function for printing to the     *
 *  	terminal with our usart2_write() *
 *  	function and then we return what *
 *  	we printed						 *
 *      						         *
 *=======================================*/
int __io_putchar(int ch)
{
	// transmit char over uart to be shown in the terminal
	usart2_write(ch);

	// return passed character
	return ch;

}

/*=======================================*
 *      						         *
 * 	Name: __io_getchar          		 *
 * 		@args: void                      *
 *  	@param: int                 	 *
 *  	@brief: we overwrite the C       *
 *  	function for getting input from  *
 *  	the terminal with our            *
 *  	function and then we return what *
 *  	we received						 *
 *      						         *
 *=======================================*/
int __io_getchar(void)
{
	// read and store character from the user
    int ch = usart2_read();

    // test if the user typed enter key and handle it
    if(ch == '\r')
    {
        usart2_write('\r');
        usart2_write('\n');
        ch = '\n';
    }
    else
    {
    	// if regular key is typed just print it out
        usart2_write(ch);
    }

    // return character read from the user
    return ch;
}

/*=======================================*
 *      						         *
 * 	Name: read_line             		 *
 * 		@args: char*, int                *
 *  	@param: void                	 *
 *  	@brief: this our own scanf  	 *
 *      type function used to read input *
 *      from the user and echo it        *
 *      								 *
 *=======================================*/
void read_line(char *buffer, int max_len)
{
	// used for incrementing through the string
    int i = 0;

    // stores each character of the string
    int ch;

    // loop through while we haven't reached the end of the string
    while(i < max_len - 1)
    {
    	// only store the first 8 bits of what was read from the terminal
        ch = usart2_read() & 0xFF;

        // handle if newline or carriage return is typed
        if(ch == '\r' || ch == '\n')
        {
            break;
        }

        // write character into our buffer
        buffer[i] = ch;

        // increment
        i++;

        usart2_write(ch);  // echo typed character
    }

    // replace the last character with null terminating char
    buffer[i] = '\0';

    // print carriage return and newline at the end of the sequence
    usart2_write('\r');
    usart2_write('\n');
}


/*=======================================*
 *      						         *
 * 	Name: usart1_tx_init        		 *
 * 		@args: void                      *
 *  	@param: void                	 *
 *  	@brief: initialize usart1 for   *
 *  	transmission					 *
 *      								 *
 *=======================================*/
void usart1_tx_init(void)
{
	// Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// Enable clock access to usart1
	RCC->APB2ENR |= RCCAPB2USART1EN;

	// clear PA9 MODER bits to 0s
	GPIOA->MODER &=~ (0x000C0000);

	// Set alternate function mode for PA9
	GPIOA->MODER |= (1U<<19);
	GPIOA->MODER &=~(1U<<18);


	// clear afr[9] pin9 bits to 0s
	GPIOA->AFR[1] &=~(0x000000F0);

	// Set afrh9 to af7
	GPIOA->AFR[1] |= (1U<<4);
	GPIOA->AFR[1] |= (1U<<5);
	GPIOA->AFR[1] |= (1U<<6);
	GPIOA->AFR[1] &=~ (1U<<7);

	// Enable USART1 tx (transfer)
	USART1->CR1 |= USARTTXEN;

	// Set baudrate for USART1 using default clock speed at
	// 115200 baud rate
	set_usart_baudrate(BAUDRATE ,APB2_16MHZ , 1);



}


/*======================================*
 *      						        *
 * 	Name: usart1_write         		    *
 * 		@args: int                      *
 *  	@param: int                		*
 *  	@brief: transmit char passed    *
 *  	by user to terminal 			*
 *      						        *
 *======================================*/
int usart1_write(int chr)
{
	// wait until transmit data register is not empty
	while(!(USART1->SR & USARTSRTXE)){}

	// send our char over uart tx to be printed to the terminal
	USART1->DR = (chr & 0xFF);

	return chr;

}
/*/\ End of function definitions /\ */





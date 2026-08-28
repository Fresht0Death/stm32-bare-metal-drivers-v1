/*
 * uart.h
 *
 *  Created on: Jun 23, 2026
 *      Author: Freshmai
 */

#ifndef UART_H_
#define UART_H_

#include "stm_err.h"
#include <stdint.h>

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
void set_usart_baudrate(int baudrate, int periph_clk, uint8_t usartNumber);


/*======================================*
 *      						        *
 * 	Name: usart2_init          			*
 * 		@args: void                     * PA2: USART2_TX, AFR[8,11] = AF7
 *  	@param: void               	    * PA3: USART2_RX, AFR[12,15] = AF7
 *  	@brief: grants access to the   *
 *  	clock for usart2 and GPIOA,     *
 *  	sets up gpio pins(PA2 and PA3)  *
 *  	for alt func mode to be used    *
 *  	for rx and tx for uart, and     *
 *  	sets baudrate to 115200			*
 *      								*
 *======================================*/
my_err_t usart2_init(void);


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
void read_line(char *buffer, int max_len);


/*=======================================*
 *      						         *
 * 	Name: usart1_tx_init        		 *
 * 		@args: void                      *
 *  	@param: void                	 *
 *  	@brief: initialize usart1 for   *
 *  	transmission					 *
 *      								 *
 *=======================================*/
void usart1_tx_init(void);


/*======================================*
 *      						        *
 * 	Name: usart1_write         		    *
 * 		@args: int                      *
 *  	@param: int                		*
 *  	@brief: transmit char passed    *
 *  	by user to terminal 			*
 *      						        *
 *======================================*/
int usart1_write(int chr);






#endif /* UART_H_ */

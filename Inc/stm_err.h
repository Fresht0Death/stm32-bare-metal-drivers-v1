/*
 * stm_err.h
 *
 *  Created on: Jul 24, 2026
 *      Author: Freshmai
 */

#ifndef STM_ERR_H_
#define STM_ERR_H_

#pragma once
#include <stdint.h>

typedef int32_t my_err_t;

#define MY_OK          0
#define MY_FAIL       -1
#define MY_ERR_NO_MEM  0x101
#define MY_ERR_BAD_ARG 0x102
#define MY_ERR_TIMEOUT 0x103

// Convert error code to text string
const char* my_err_to_name(my_err_t code);


#endif /* STM_ERR_H_ */

/*
 * stm_err.c
 *
 *  Created on: Jul 24, 2026
 *      Author: Freshmai
 */


#include "stm_err.h"

const char* my_err_to_name(my_err_t code) {
    switch (code) {
        case MY_OK:          return "MY_OK";
        case MY_FAIL:        return "MY_FAIL";
        case MY_ERR_NO_MEM:  return "MY_ERR_NO_MEM";
        case MY_ERR_BAD_ARG: return "MY_ERR_BAD_ARG";
        case MY_ERR_TIMEOUT: return "MY_ERR_TIMEOUT";
        default:             return "UNKNOWN_ERROR";
    }
}

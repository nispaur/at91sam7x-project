/*
 * debug.c
 *
 *  Created on: 5 Apr 2016
 *      Author: stardami
 */

#include <sys/usart/usart.h>
#include "debug.h"
#include <sys/board.h>

void print_debug(const char *str) {
    // We suppose the usart0 initialised (flag ?)
    write_str_USART0("\n[DEBUG] ");
    write_str_USART0(str);
}

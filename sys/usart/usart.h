#pragma once

#include <stdint.h>

#define AT91C_US_ASYNC_MODE                                                    \
    (AT91C_US_USMODE_NORMAL + AT91C_US_NBSTOP_1_BIT + AT91C_US_PAR_NONE +      \
     AT91C_US_CHRL_8_BITS + AT91C_US_CLKS_CLOCK)

void InitUSART0(void);

void write_char_USART0(char);
uint8_t read_char_USART0(void);
void write_str_USART0(const char *buff);
uint8_t read_char_USART0_nonstop(void);
int is_ready_USART0(void);
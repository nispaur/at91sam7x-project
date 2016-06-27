/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "math.h"
#include "trace.h"

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Returns the minimum value between two integers.
/// \param a  First integer to compare.
/// \param b  Second integer to compare.
//------------------------------------------------------------------------------
uint32_t min(uint32_t a, uint32_t b)
{ 
    if (a < b) {

        return a;
    }
    else {

        return b;
    }
}

//------------------------------------------------------------------------------
/// Returns the absolute value of an integer.
/// \param value  Integer value.
//------------------------------------------------------------------------------
// Do not call this function "abs", problem with gcc !
uint32_t absv(signed int value)
{
    if (value < 0) {

        return -value;
    }
    else {

        return value;
    }
}

//------------------------------------------------------------------------------
/// Computes and returns x power of y.
/// \param x  Value.
/// \param y  Power.
//------------------------------------------------------------------------------
uint32_t power(uint32_t x, uint32_t y)
{
    uint32_t result = 1;
    
    while (y > 0) {

        result *= x;
        y--;
    } 
    return result;
}

// Fonctions de la stdlib pour diviser 



/*
 * 32-bit unsigned integer divide.
 */
unsigned int __aeabi_uidiv(unsigned int num, unsigned int den)
{
    return __udivmodsi4(num, den, 0);
}

uint32_t __udivmodsi4(uint32_t num, uint32_t den, uint32_t * rem_p)
{
    uint32_t quot = 0, qbit = 1;

    if (den == 0) {
        
        return 0;   /* If trap returns... */
    }

    /* Left-justify denominator and count shift */
    while ((int32_t) den >= 0) {
        den <<= 1;
        qbit <<= 1;
    }

    while (qbit) {
        if (den <= num) {
            num -= den;
            quot += qbit;
        }
        den >>= 1;
        qbit >>= 1;
    }

    if (rem_p)
        *rem_p = num;

    return quot;
}
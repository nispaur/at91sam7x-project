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

#include "trace.h"

//------------------------------------------------------------------------------
//         Internal variables
//------------------------------------------------------------------------------

/// Trace level can be set at applet initialization
#if !defined(NOTRACE) && (DYN_TRACES == 1)
    uint32_t traceLevel = TRACE_LEVEL;
#endif
  
#ifndef NOFPUT
#include <stdio.h>
#include <stdarg.h>

//------------------------------------------------------------------------------
/// \exclude
/// Implementation of fputc using the DBGU as the standard output. Required
/// for printf().
/// \param c  Character to write.
/// \param pStream  Output stream.
/// \param The character written if successful, or -1 if the output stream is
/// not stdout or stderr.
//------------------------------------------------------------------------------
signed int fputc(signed int c, FILE *pStream)
{
    if ((pStream == stdout) || (pStream == stderr)) {

        TRACE_PutChar(c);
        return c;
    }
    else {

        return EOF;
    }
}

//------------------------------------------------------------------------------
/// \exclude
/// Implementation of fputs using the DBGU as the standard output. Required
/// for printf(). Does NOT currently use the PDC.
/// \param pStr  String to write.
/// \param pStream  Output stream.
/// \return Number of characters written if successful, or -1 if the output
/// stream is not stdout or stderr.
//------------------------------------------------------------------------------
signed int fputs(const char *pStr, FILE *pStream)
{
    signed int num = 0;

    while (*pStr != 0) {

        if (fputc(*pStr, pStream) == -1) {

            return -1;
        }
        num++;
        pStr++;
    }

    return num;
}

#undef putchar

//------------------------------------------------------------------------------
/// \exclude
/// Outputs a character on the DBGU.
/// \param c  Character to output.
/// \return The character that was output.
//------------------------------------------------------------------------------
signed int putchar(signed int c)
{
    return fputc(c, stdout);
}

#endif //#ifndef NOFPUT

//------------------------------------------------------------------------------
//         Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Print char if printable. If not print a point
/// \param c char to
//------------------------------------------------------------------------------
static void PrintChar(uint8_t c)
{
    if( (/*c >= 0x00 &&*/ c <= 0x1F) ||
        (c >= 0xB0 && c <= 0xDF) ) {

       printf(".");
    }
    else {

       printf("%c", c);
    }
}

//------------------------------------------------------------------------------
//         Global Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Displays the content of the given frame on the Trace interface.
/// \param pBuffer  Pointer to the frame to dump.
/// \param size  Buffer size in bytes.
//------------------------------------------------------------------------------
void TRACE_DumpFrame(uint8_t *pFrame, uint32_t size)
{
    uint32_t i;

    for (i=0; i < size; i++) {
        printf("%02X ", pFrame[i]);
    }

    printf("\n\r");
}

//------------------------------------------------------------------------------
/// Displays the content of the given buffer on the Trace interface.
/// \param pBuffer  Pointer to the buffer to dump.
/// \param size     Buffer size in bytes.
/// \param address  Start address to display
//------------------------------------------------------------------------------
void TRACE_DumpMemory(
    uint8_t *pBuffer,
    uint32_t size,
    uint32_t address
    )
{
    uint32_t i, j;
    unsigned int lastLineStart;
    uint8_t* pTmp;

    for (i=0; i < (size / 16); i++) {

        printf("0x%08X: ", (unsigned int)(address + (i*16)));
        pTmp = (uint8_t*)&pBuffer[i*16];
        for (j=0; j < 4; j++) {
            printf("%02X%02X%02X%02X ", pTmp[0],pTmp[1],pTmp[2],pTmp[3]);
            pTmp += 4;
        }

        pTmp = (uint8_t*)&pBuffer[i*16];
        for (j=0; j < 16; j++) {
            PrintChar(*pTmp++);
        }

        printf("\n\r");
    }

    if( (size%16) != 0) {
        lastLineStart = size - (size%16);
        printf("0x%08X: ", (unsigned int)(address + lastLineStart));

        for (j= lastLineStart; j < lastLineStart+16; j++) {

            if( (j!=lastLineStart) && (j%4 == 0) ) {
                printf(" ");
            }
            if(j<size) {
                printf("%02X", pBuffer[j]);
            }
            else {
                printf("  ");
            }
        }

        printf(" ");
        for (j= lastLineStart; j <size; j++) {
            PrintChar(pBuffer[j]);
        }

        printf("\n\r");
    }
}
    
//------------------------------------------------------------------------------
/// Reads an integer
//------------------------------------------------------------------------------
uint8_t TRACE_GetInteger(uint32_t *pValue)
{
    uint8_t key;
    uint8_t nbNb = 0;
    uint32_t value = 0;
    while(1) {
        key = TRACE_GetChar();
        TRACE_PutChar(key);
        if(key >= '0' &&  key <= '9' ) {
            value = (value * 10) + (key - '0');
            nbNb++;
        }
        else if(key == 0x0D || key == ' ') {
            if(nbNb == 0) {
                printf("\n\rWrite a number and press ENTER or SPACE!\n\r");       
                return 0; 
            } else {
                printf("\n\r"); 
                *pValue = value;
                return 1;
            }
        } else {
            printf("\n\r'%c' not a number!\n\r", key);
            return 0;  
        }
    }
}

//------------------------------------------------------------------------------
/// Reads an integer and check the value
//------------------------------------------------------------------------------
uint8_t TRACE_GetIntegerMinMax(
    uint32_t *pValue, 
    uint32_t min, 
    uint32_t max
    )
{
    uint32_t value = 0;

    if( TRACE_GetInteger(&value) == 0) {
        return 0;
    }
    
    if(value < min || value > max) {
        printf("\n\rThe number have to be between %u and %u\n\r", (unsigned int)min, (unsigned int)max);
        return 0; 
    }

    printf("\n\r"); 
    *pValue = value;
    return 1;
}

//------------------------------------------------------------------------------
/// Reads an hexadecimal number
//------------------------------------------------------------------------------
uint8_t TRACE_GetHexa32(uint32_t *pValue)
{
    uint8_t key;
    uint32_t i = 0;
    uint32_t value = 0;
    for(i = 0; i < 8; i++) {
        key = TRACE_GetChar();
        TRACE_PutChar(key);
        if(key >= '0' &&  key <= '9' ) {
            value = (value * 16) + (key - '0');
        }
        else if(key >= 'A' &&  key <= 'F' ) {
            value = (value * 16) + (key - 'A' + 10) ;
        }
        else if(key >= 'a' &&  key <= 'f' ) {
            value = (value * 16) + (key - 'a' + 10) ;
        }        
        else {
            printf("\n\rIt is not a hexa character!\n\r");       
            return 0; 
        }
    }

    printf("\n\r");    
    *pValue = value;     
    return 1;
}


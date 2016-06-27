#ifndef _TC_H_
#define _TC_H_ 

#include "tc.h"
#include <sys/board.h>

//------------------------------------------------------------------------------
//         Global Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Configures a Timer Counter to operate in the given mode. Timer is stopped
/// after configuration and must be restarted with TC_Start(). All the
/// interrupts of the timer are also disabled.
/// \param pTc  Pointer to an AT91S_TC instance.
/// \param mode  Operating mode (TC_CMR value).
//------------------------------------------------------------------------------
void TC_Configure(int timer, uint32_t mode);

//------------------------------------------------------------------------------
/// Enables the timer clock and performs a software reset to start the counting.
/// \param pTc  Pointer to an AT91S_TC instance.
//------------------------------------------------------------------------------
void TC_Start(int timer);

//------------------------------------------------------------------------------
/// Disables the timer clock, stopping the counting.
/// \param pTc  Pointer to an AT91S_TC instance.
//------------------------------------------------------------------------------
void TC_Stop(int timer);

//------------------------------------------------------------------------------
/// Finds the best MCK divisor given the timer frequency and MCK. The result
/// is guaranteed to satisfy the following equation:
/// \pre
///   (MCK / (DIV * 65536)) <= freq <= (MCK / DIV)
/// \endpre
/// with DIV being the highest possible value.
/// \param freq  Desired timer frequency.
/// \param mck  Master clock frequency.
/// \param div  Divisor value.
/// \param tcclks  TCCLKS field value for divisor.
/// \return 1 if a proper divisor has been found; otherwise 0.
//------------------------------------------------------------------------------
uint8_t TC_FindMckDivisor(uint32_t freq, uint32_t mck, uint32_t *div,
                          uint32_t *tcclks) ;

#endif // _TC_H_
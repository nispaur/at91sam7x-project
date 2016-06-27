
#include "tc.h"
#include <sys/board.h>

const AT91PS_TC tc[] = {AT91C_BASE_TC0, AT91C_BASE_TC1, AT91C_BASE_TC2};

//------------------------------------------------------------------------------
//         Global Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Configures a Timer Counter to operate in the given mode. Timer is stopped
/// after configuration and must be restarted with TC_Start(). All the
/// interrupts of the timer are also disabled.
/// \param tc[timer]  Pointer to an AT91S_TC instance.
/// \param mode  Operating mode (TC_CMR value).
//------------------------------------------------------------------------------
void TC_Configure(int timer, uint32_t mode) {
    // Disable TC clock
    tc[timer]->TC_CCR = AT91C_TC_CLKDIS;

    // Disable interrupts
    tc[timer]->TC_IDR = 0xFFFFFFFF;

    // Clear status register
    tc[timer]->TC_SR;

    // Set mod
    tc[timer]->TC_CMR = mode;
}

//------------------------------------------------------------------------------
/// Enables the timer clock and performs a software reset to start the counting.
/// \param tc[timer]  Pointer to an AT91S_TC instance.
//------------------------------------------------------------------------------
void TC_Start(int timer) { 
    // Enable clock and do a software reset at the same time.
    tc[timer]->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG; 
}

//------------------------------------------------------------------------------
/// Disables the timer clock, stopping the counting.
/// \param tc[timer]  Pointer to an AT91S_TC instance.
//------------------------------------------------------------------------------
void TC_Stop(int timer) { 
    // The same in reverse.
    tc[timer]->TC_CCR = AT91C_TC_CLKDIS; 
}

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
                          uint32_t *tcclks) {
    uint32_t index = 0;
    uint32_t divisors[5] = {2, 8, 32, 128, 1024};

    // Satisfy lower bound
    while (freq < ((mck / divisors[index]) / 65536)) {
        // If no divisor can be found, return 0
        if (++index == 5)
            return 0;
    }

    // Try to maximize DIV while satisfying upper bound
    while (index++ < 4) {
        if (freq > (mck / divisors[index]))
            break;
    }

    // Store results
    if (div)
        *div = divisors[index];

    if (tcclks)
        *tcclks = index;

    return 1;
}

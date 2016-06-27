//usart.c
#include "usart.h"
#include "olimex/AT91SAM7X256.h"
#include "olimex/bits.h"

const AT91PS_PIO u_pPioA = AT91C_BASE_PIOA;
const AT91PS_PIO u_pPioB = AT91C_BASE_PIOB;
const AT91PS_PMC u_pPMC = AT91C_BASE_PMC;
//const AT91PS_USART AT91C_BASE_US0 = AT91C_BASE_US0;
const AT91PS_USART u_pUSART1 = AT91C_BASE_US1;
const AT91PS_PDC u_pPDC0 = AT91C_BASE_PDC_US0;
const AT91PS_PDC u_pPDC1 = AT91C_BASE_PDC_US1;
const AT91PS_MC u_pMC = AT91C_BASE_MC;
const AT91PS_AIC u_pAic = AT91C_BASE_AIC;

void InitUSART0(void) {
    AT91C_BASE_PIOA->PIO_PDR = BIT0 | BIT1; //Disables the PIO from controlling the corresponding pin (enables peripheral control of the pin).
    AT91C_BASE_PIOA->PIO_ASR = BIT0 | BIT1; //Assigns the I/O line to the peripheral B function.
    AT91C_BASE_PIOA->PIO_BSR = 0;

    //enable the clock of USART
    u_pPMC->PMC_PCER = 1 << AT91C_ID_US0;

    //Disable interrupts
    //u_pUSART->US_IDR = (uint32_t) -1;

    //Reset receiver and transmitter
    AT91C_BASE_US0->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RXDIS | AT91C_US_TXDIS;

    //Define the baud rate divisor register
    //const uint32_t main_clock = 47923200;
    //const uint32_t main_clock = 326578;
    //const uint32_t main_clock = 2*14756000;
    //const uint32_t baud_rate  = 9600;

    //set baud rate divisor register
    //u_pUSART->US_BRGR = 192;  //((2*147456000)/9600x16)
    //AT91C_BASE_US0->US_BRGR = 313;   //((48000000)/9600x16)
    //u_pUSART->US_BRGR = 96;   //((14745600)/9600x16)
    AT91C_BASE_US0->US_BRGR = 25;  //((14745600)/115200x16)

    //write the Timeguard Register
    AT91C_BASE_US0->US_TTGR = 0;

    //Set the USART mode
    //u_pUSART->US_MR = AT91C_US_ASYNC_MODE;
    //u_pUSART->US_MR = 0x4c0;
    //u_pUSART->US_MR = 0x8c0;
    AT91C_BASE_US0->US_MR = 0x08c0;

    //Enable the RX and TX PDC transfer requests
    u_pPDC0->PDC_PTCR = AT91C_PDC_TXTEN | AT91C_PDC_RXTEN;

    //Enable usart - enable RX receiver and TX transmitter
    AT91C_BASE_US0->US_CR = 0x50;
    print_debug("Init usart terminé.\n");
}

void write_char_USART0(char ch) {
    while (!(AT91C_BASE_US0->US_CSR & AT91C_US_TXRDY) == 1);
    AT91C_BASE_US0->US_THR = ((ch & 0x1FF));
}

uint8_t read_char_USART0(void) {
    while (!(AT91C_BASE_US0->US_CSR & AT91C_US_RXRDY) == 1);
    return ((AT91C_BASE_US0->US_RHR) & 0x1FF);
}

uint8_t read_char_USART0_nonstop(void) {
    return ((AT91C_BASE_US0->US_CSR & AT91C_US_RXRDY) == 1) ? ((AT91C_BASE_US0->US_RHR) & 0x1FF) : 0;
}

void write_str_USART0(const char* buff){
    uint32_t i = 0x0;
    while (buff[i] != '\0')
        write_char_USART0(buff[i++]);
}

int is_ready_USART0() {
    return ((AT91C_BASE_US0->US_CSR & AT91C_US_RXRDY) == 1);
}
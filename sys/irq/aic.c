

/* Code de gestion de l'AIC */
#include "aic.h"
#include <sys/board.h>

AT91PS_AIC aic = AT91C_BASE_AIC;

/* Procedure d'initialisation de l'AIC */
void aic_init(void) {
	// Activer le Debug Control Register
	// Sans cela, gdb risque de lire les registres de l'AIC et accidentellement aquitter une irq.
	// En mode Debug, il est nécessaire d'écrire dans AIC_IVR pour aquitter l'itr.
    AT91C_BASE_AIC->AIC_DCR = AT91C_AIC_DCR_PROT;
}

void aic_irqsetup(int src, int conf, void (*irqhandler)(void)) {
	aic = AT91C_BASE_AIC;
	aic->AIC_IDCR = 1<<src; 					// Desactivation de l'interruption sur la source
	aic->AIC_SVR[src] = (AT91_REG)irqhandler; 	// Mise en place du handler 
	aic->AIC_SMR[src] = conf;
	aic->AIC_ICCR = 1<<src; 					// Raz des irq anterieures
	aic->AIC_EOICR = 0xf00;						// Aquittement des itr anterieures
}

// Active les interrutpions sur la voie src, suppose la conf déja faite.
void aic_irqenable(int src) {
	aic->AIC_IECR = 1<<src;
}

// Desactive les interrutpions sur la voie src, suppose la conf déja faite.
void aic_irqdisable(int src) {
	aic->AIC_IDCR = 1<<src;
}
#ifndef _AIC_H_
#define _AIC_H_

/* Code de gestion de l'AIC */
#include <sys/board.h>

/* Procedure d'initialisation de l'AIC */
void aic_init(void);

void aic_irqsetup(int src, int conf, void (*irqhandler)(void));

// Active les interrutpions sur la voie src, suppose la conf déja faite.
void aic_irqenable(int src);

// Desactive les interrutpions sur la voie src, suppose la conf déja faite.
void aic_irqdisable(int src);

#endif
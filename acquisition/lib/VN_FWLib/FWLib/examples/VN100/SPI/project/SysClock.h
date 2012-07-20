
/* Define to prevent recursive inclusion */
#ifndef SYSCLOCK_H
#define SYSCLOCK_H

/* Includes */
#include "stm32f10x_lib.h"



/* Function Prototypes */
void SYSCLOCK_Init(void (*ptr2Func)(void));
void SYSCLOCK_Enable(void);
void SYSCLOCK_Disable(void);
void SYSCLOCK_SetTimeSpan(u32 timeSpan);
void SysTickHandler(void);



#endif


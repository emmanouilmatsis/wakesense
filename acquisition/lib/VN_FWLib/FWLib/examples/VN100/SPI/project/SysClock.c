
#include "SysClock.h"

/* Function pointer for timer */
static void (*SYSCLOCK_extFunc)(void);

/*******************************************************************************
* Function Name  : SYSCLOCK_Init
* Description    : Initialize the SYSTick Peripheral.
* Input          : timeSpan = Time span of the timer in micro seconds.
*                : ptr2Func = Pointer to function to call on timer tick.
* Output         : None
* Return         : None
*******************************************************************************/
void SYSCLOCK_Init(void (*ptr2Func)(void))
{	
	/* Set the priority and sub priority of the SysTick interrupt handler */
	NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 1, 2);

  /* Enable SysTick interrupt */
  SysTick_ITConfig(ENABLE);
	
	/* Set external function pointer */
	SYSCLOCK_extFunc = ptr2Func;
}

/*******************************************************************************
* Function Name  : SYSCLOCK_Enable
* Description    : Enable the SYSTick Peripheral.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SYSCLOCK_Enable()
{
	/* Enable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);		
}

/*******************************************************************************
* Function Name  : SYSCLOCK_Disable
* Description    : Disable the SYSTick Peripheral.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SYSCLOCK_Disable()
{
	/* Disable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Disable);		
}

/*******************************************************************************
* Function Name  : SYSCLOCK_SetTimeSpan
* Description    : Set the timespan for the SYSTick Peripheral timer.
* Input          : timeSpan = Time span of the timer in micro seconds.
* Output         : None
* Return         : None
*******************************************************************************/
void SYSCLOCK_SetTimeSpan(u32 timeSpan)
{
	/* Disable the SysTick Counter */
  /* SysTick_CounterCmd(SysTick_Counter_Disable); */
	
	/* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
  SysTick_SetReload(9*timeSpan);
	
	/* Enable the SysTick Counter */
  /* SysTick_CounterCmd(SysTick_Counter_Enable); */
}

/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTickHandler(void)
{
	(*SYSCLOCK_extFunc)();	
}


/**
 * \file    nvic_it.c
 *
 *          NVIC interrupts handler.
 * 
 * \author  Kamil Kiełbasa 
 */

// Include files --------------------------------------
#include <stdbool.h>
#include <string.h>
#include "peripherals.h"

// Module defines -------------------------------------
// Module types and type definitiones -----------------
// Module interface variables and constants -----------
// Static variables and constants ---------------------
// Static function declarations -----------------------
// Static function definitions ------------------------
// Module interface function definitions --------------

void NMI_Handler( void )
{
    __disable_irq();
    while ( true ) {};
}

void HardFault_Handler( void )
{
    __disable_irq();
    while ( true ) {};
}

void MemManage_Handler( void )
{
    __disable_irq();
    while ( true ) {};
}

void BusFault_Handler( void )
{
    __disable_irq();
    while ( true ) {};
}

void UsageFault_Handler( void )
{
    __disable_irq();
    while ( true ) {};
}

void DebugMon_Handler( void )
{
    __disable_irq();
    while ( true ) {};
}

void SysTick_Handler( void )
{
    HAL_IncTick();
}

void EXTI15_10_IRQHandler( void )
{
    if ( 0x00U != __HAL_GPIO_EXTI_GET_IT( BUTTON_BLUE_Pin ) )
    {
        __HAL_GPIO_EXTI_CLEAR_IT( BUTTON_BLUE_Pin );

        const char* button_str = "Pushed button!\n\r";
        const size_t len = strlen( button_str );
        HAL_UART_Transmit( &uart1_handle, ( uint8_t* )button_str, len, HAL_MAX_DELAY );
    }
}

void USART1_IRQHandler( void )
{
    HAL_UART_IRQHandler( &uart1_handle );
}

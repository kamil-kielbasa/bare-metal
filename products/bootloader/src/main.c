/**
 * \file    main.c
 *
 *          Core implementation of bootloader.
 * 
 * \author  Kamil Kiełbasa 
 */

// Include files --------------------------------------
#include "shared_storage.h"
#include "peripherals.h"
#include "emb_assert.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

// Module defines -------------------------------------
// Module types and type definitiones -----------------
// Module interface variables and constants -----------
// Static variables and constants ---------------------
static uint8_t received;

// Static function declarations -----------------------

/**
 * @brief Jump to different image in NVM.
 * 
 * @param address should be image first address with stack pointer and reset handler.
 */
static __attribute__ ( ( noreturn ) ) void jump( uint32_t address );

/**
 * @brief Save in flash new boot counter and boot reason.
 */
static void save_boot_meta_data( void );

// Static function definitions ------------------------
static __attribute__ ( ( noreturn ) ) void jump( uint32_t address )
{
    // Setup vector table to the beginning of the image.
    register const uint32_t vtor = address;
    SCB->VTOR = vtor;

    asm (
        "ldr r0, [%0]\n"        //< R0 = *vtor; first element in VTABLE is initial stack pointer.
        "mov sp, r0\n"          //< SP = R0;
        "ldr r0, [%0, #4]\n"    //< R0 = *(vtor + 4); Second element in VTABLE is address to reset handler.
        "bx  r0\n"              //< PC = R0; jump to reset handler.
    :
    : "r" ( vtor )
    : "memory" );

    __builtin_unreachable();
}

static void save_boot_meta_data( void )
{
    bool ret = false;

    ret = shared_storage_init();
    emb_assert( ret );

    shared_flash_partition_t partition = { 0 };

    ret = shared_storage_load( &partition );
    emb_assert( ret );

    boot_reason_t reason = boot_talk_get_reason();
    emb_assert( reason < BOOT_REASON_UNDEFINED );

    partition.boot_counter += 1;
    partition.boot_reason_counter[ reason ] += 1;
    
    ret = shared_storage_store( &partition );
    emb_assert( ret );
}

// Module interface function definitions --------------

void HAL_UART_RxCpltCallback( UART_HandleTypeDef *huart ) 
{
    if ( USART1 == huart->Instance )
    {
        HAL_UART_Receive_IT( &uart1_handle, &received, 1 );
        return;
    }
}

void HAL_UART_TxCpltCallback( UART_HandleTypeDef *huart)
{
    if ( USART1 == huart->Instance )
    {
        return;
    }
}

int main( void )
{
    // by default enable all interrupts and HAL initialization.
    __enable_irq();
    HAL_Init();

    // peripherals initialization.
    peripherals_system_clocks_init();
    peripherals_blue_button_init();
    peripherals_led_init();
    peripherals_uart1_init();
    peripherals_iwdg_init();

    // start waiting for any incoming data from UART IRQ.
    HAL_UART_Receive_IT( &uart1_handle, &received, 1 );

    // save boot counter and reason of reset.
    save_boot_meta_data();

    // print how you are over the UART.
    const char bl_hello[] = "Bootloader started!\r\n";
    const size_t bl_hello_len = strlen( &bl_hello[ 0 ] );
    HAL_UART_Transmit_IT( &uart1_handle, ( uint8_t* )&bl_hello[ 0 ], bl_hello_len );

    while ( true )
    {
        HAL_Delay( 1000 );
        HAL_GPIO_TogglePin( LD2_GPIO_Port, LD2_Pin );

        const char ping_str[] = "ping!\r\n";
        const size_t ping_len = strlen( &ping_str[ 0 ] );
        HAL_UART_Transmit_IT( &uart1_handle, ( uint8_t* )&ping_str[ 0 ], ping_len );

        HAL_IWDG_Refresh( &iwdg_handle );
    }

    // this address is an example
    jump( 0x08000000 );
}

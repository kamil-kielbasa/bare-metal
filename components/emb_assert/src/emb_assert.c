/**
 * \file    crc.c
 *
 *          Assert dedicated for embedded system.
 * 
 * \author  Kamil Kiełbasa 
 */

// Include files --------------------------------------
#include "emb_assert.h"
#include "stm32l4xx_hal.h"

#include <stdint.h>
#include <assert.h>

// Module defines -------------------------------------
#define MAGIC_HASH  ( 0xf30ca884U )

// Module types and type definitiones -----------------
typedef struct
{
    uint32_t value;
    uint32_t value_inv;
} shared_ram_t;

static_assert( sizeof( shared_ram_t ) == 8, "Wrong size of struct in case of embedded assert" );

// Module interface variables and constants -----------
// Static variables and constants ---------------------
shared_ram_t __attribute__ ( ( section ( ".shared_ram" ) ) ) shared_ram;

// Static function declarations -----------------------
// Static function definitions ------------------------
// Module interface function definitions --------------

void emb_assert( bool condition )
{
    if ( condition )
    {
        return;
    }

    shared_ram.value = MAGIC_HASH;
    shared_ram.value_inv = ~MAGIC_HASH;

    __disable_irq();

    HAL_NVIC_SystemReset();
}

bool emb_assert_is_reason( void )
{
    if ( ( shared_ram.value ^ shared_ram.value_inv ) == 0xffffffffU )
    {
        shared_ram.value = 0;
        shared_ram.value_inv = 0;
        return true;
    }

    return false;
}

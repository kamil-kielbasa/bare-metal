/**
 * \file    boot_talk.c
 *
 *          Boot talk component.
 * 
 * \author  Kamil Kiełbasa 
 */

// Include files --------------------------------------
#include "boot_talk.h"
#include "emb_assert.h"
#include "stm32l4xx_hal_rcc.h"

// Module defines -------------------------------------
// Module types and type definitiones -----------------
// Module interface variables and constants -----------
// Static variables and constants ---------------------
// Static function declarations -----------------------
// Static function definitions ------------------------
// Module interface function definitions --------------

boot_reason_t boot_talk_get_reason( void )
{
    if ( emb_assert_is_reason() )
    {
        __HAL_RCC_CLEAR_RESET_FLAGS();
        return BOOT_REASON_ASSERT;
    }

    if ( __HAL_RCC_GET_FLAG( RCC_FLAG_IWDGRST ) ||
         __HAL_RCC_GET_FLAG( RCC_FLAG_WWDGRST ) )
    {
        __HAL_RCC_CLEAR_RESET_FLAGS();
        return BOOT_REASON_WATCHDOG;
    }

    if ( __HAL_RCC_GET_FLAG( RCC_FLAG_BORRST ) ||
         __HAL_RCC_GET_FLAG( RCC_FLAG_SFTRST ) ||
         __HAL_RCC_GET_FLAG( RCC_FLAG_PINRST ) )
    {
        __HAL_RCC_CLEAR_RESET_FLAGS();
        return BOOT_REASON_NORMAL;
    }

    return BOOT_REASON_UNDEFINED;
}

/**
 * \file    shared_storage.c
 *
 *          Shared storage in flash component.
 * 
 * \author  Kamil Kiełbasa 
 */

// Include files --------------------------------------
#include "shared_storage.h"
#include "crc.h"
#include "stm32l4xx_hal_flash.h"
#include "stm32l4xx_hal_flash_ex.h"

#include <string.h>

// Module defines -------------------------------------
// Module types and type definitiones -----------------
// Module interface variables and constants -----------
extern const uint8_t storage_shared_storage_start[];
extern const uint8_t storage_shared_storage_end[];

// Static variables and constants ---------------------
// Static function declarations -----------------------
// Static function definitions ------------------------
static bool partition_load( shared_flash_partition_t* partition )
{
    return partition == memcpy( partition, storage_shared_storage_start, sizeof( *partition ) );
}

static bool partition_store( shared_flash_partition_t* partition )
{
    FLASH_EraseInitTypeDef erase_init =
    {
        .TypeErase = FLASH_TYPEERASE_PAGES,
        .Banks = FLASH_BANK_2,
        .Page = 0,
        .NbPages = 1,
    };

    HAL_FLASH_Unlock();

    uint32_t page_error = 0x0;

    if ( HAL_OK != HAL_FLASHEx_Erase( &erase_init, &page_error ) &&
         0xffffffff == page_error )
    {
        return false;
    }

    uint64_t* ptr = ( uint64_t* )partition;

    for ( size_t i = 0; i < sizeof( shared_flash_partition_t ) / sizeof( uint64_t ); ++i )
    {
        if ( HAL_OK != HAL_FLASH_Program( FLASH_TYPEPROGRAM_DOUBLEWORD, 
                                          ( uint32_t )&storage_shared_storage_start[ i * sizeof( uint64_t ) ], 
                                          ptr[ i ] ) )
        {
            return false;
        }
    }

    HAL_FLASH_Lock();

    return true;
}

// Module interface function definitions --------------

bool shared_storage_init( void )
{
    const ptrdiff_t shared_storage_length = &storage_shared_storage_end[ 0 ] - &storage_shared_storage_start[ 0 ];

    if ( FLASH_PAGE_SIZE != shared_storage_length )
    {
        return false;
    }

    shared_flash_partition_t partition = { 0 };

    if ( false == partition_load( &partition ) )
    {
        return false;
    }

    if ( 0U != crc16_ccitt_mcrf4xx_block( ( const uint8_t* )&partition, sizeof( partition ) ) )
    {
        memset( &partition, 0, sizeof( partition ) );
        partition.crc = crc16_ccitt_mcrf4xx_block( ( const uint8_t* )&partition, offsetof( shared_flash_partition_t, crc ) );

        return partition_store( &partition );
    }

    return true;
}

bool shared_storage_load( shared_flash_partition_t* partition )
{
    shared_flash_partition_t tmp = { 0 };

    if ( false == partition_load( &tmp ) )
    {
        return false;
    }

    if ( 0U == crc16_ccitt_mcrf4xx_block( ( const uint8_t* )&tmp, sizeof( tmp ) ) )
    {
        return ( partition == memcpy( partition, &tmp, sizeof( *partition ) ) );
    }
    
    return false;
}

bool shared_storage_store( shared_flash_partition_t* partition )
{
    partition->crc = crc16_ccitt_mcrf4xx_block( ( const uint8_t* )partition, offsetof( shared_flash_partition_t, crc ) );

    return partition_store( partition );
}

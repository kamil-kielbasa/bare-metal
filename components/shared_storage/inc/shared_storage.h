/**
 * \file    shared_storage.h
 *
 *          Shared storage in flash component.
 * 
 * \author  Kamil Kiełbasa 
 */

// Header guard ---------------------------------------
#ifndef SHARED_STORAGE_H
#define SHARED_STORAGE_H

// Include files --------------------------------------
#include "boot_talk.h"

#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

// Defines --------------------------------------------
// Types and type definitions -------------------------
typedef struct
{
    uint32_t boot_counter;
    uint32_t boot_reason_counter[ BOOT_REASON_MAX ];
    uint16_t padding;
    uint16_t crc;
} __attribute( ( aligned ( sizeof ( uint64_t ) ) ) ) shared_flash_partition_t;

static_assert( sizeof( shared_flash_partition_t ) == ( sizeof( uint64_t ) * 3 ), "Wrong size of struct in case of flash write" );

// Module interface variables and constants -----------
// Extern variables and constant declarations ---------
// Module interface function declarations -------------

/**
 * @brief Initialization of shared storage.
 * 
 * @return true if successful, otherwise false.
 */
bool shared_storage_init( void );

/**
 * @brief Read partition from shared storage flash. 
 * 
 * @param partition pointer to partition that will be loaded.
 * 
 * @return true if successful, otherwise false.
 */
bool shared_storage_load( shared_flash_partition_t* partition );

/**
 * @brief Write updated partition to shared storage flash.
 * 
 * @param partition pointer to partition that will be stored.
 * 
 * @return true if successful, otherwise false.
 */
bool shared_storage_store( shared_flash_partition_t* partition );

#endif /* SHARED_STORAGE_H */

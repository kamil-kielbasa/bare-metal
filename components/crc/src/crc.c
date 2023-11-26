/**
 * \file    crc.c
 *
 *          CRC generic component.
 * 
 * \author  Kamil Kiełbasa 
 */

// Include files --------------------------------------
#include "crc.h"

// Module defines -------------------------------------
// Module types and type definitiones -----------------
// Module interface variables and constants -----------
// Static variables and constants ---------------------
// Static function declarations -----------------------
static uint16_t crc16_ccitt_mcrf4xx( uint16_t crc, uint8_t byte );

// Static function definitions ------------------------
static uint16_t crc16_ccitt_mcrf4xx( uint16_t crc, uint8_t byte )
{
    crc ^= byte;
    crc = ( crc >> 8 ) | ( crc << 8 );
    crc ^= ( crc & 0xff00 ) << 4;
    crc ^= ( crc >> 8 ) >> 4;
    crc ^= ( crc & 0xff00 ) >> 5;

    return crc;
}

// Module interface function definitions --------------

uint16_t crc16_ccitt_mcrf4xx_block( const uint8_t* ptr, uint32_t len )
{
    uint16_t crc = 0xffff;

    for ( uint32_t i = 0; i < len; ++i )
    {
        crc = crc16_ccitt_mcrf4xx( crc, ptr[ i ] );
    }

    return crc;
}

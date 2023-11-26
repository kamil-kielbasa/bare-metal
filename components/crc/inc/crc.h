/**
 * \file    crc.h
 *
 *          Generic CRC component.
 * 
 * \author  Kamil Kiełbasa 
 */

// Header guard ---------------------------------------
#ifndef CRC_H
#define CRC_H

// Include files --------------------------------------
#include <stdint.h>

// Defines --------------------------------------------
// Types and type definitions -------------------------
// Module interface variables and constants -----------
// Extern variables and constant declarations ---------
// Module interface function declarations -------------

/**
 * @brief CRC-16/MCRF4XX implementation.
 * 
 * @param ptr input data to calculate CRC.
 * @param len length of @ptr.
 * 
 * @return 16 bits CRC on @ptr and @len.
 */
uint16_t crc16_ccitt_mcrf4xx_block( const uint8_t* ptr, uint32_t len );

#endif /* CRC_H */

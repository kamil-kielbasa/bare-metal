/**
 * \file    emb_assert.h
 *
 *          Assert dedicated for embedded system.
 * 
 * \author  Kamil Kiełbasa 
 */

// Header guard ---------------------------------------
#ifndef EMB_ASSERT_H
#define EMB_ASSERT_H

// Include files --------------------------------------
#include <stdbool.h>

// Defines --------------------------------------------
// Types and type definitions -------------------------
// Module interface variables and constants -----------
// Extern variables and constant declarations ---------
// Module interface function declarations -------------

/**
 * @brief in case of false condition following things will be done:
 *        - save in shared ram that assert happened.
 *        - disable interrupts.
 *        - perform software reset.
 * 
 * @param condition condition to check.
 */
void emb_assert( bool condition );

/**
 * @brief check if reset was done by assert. Once check, persistent ram will be zeros.
 * 
 * @return true if assert was reason of reset, otherwise false.
 */
bool emb_assert_is_reason( void );

#endif /* EMB_ASSERT_H */

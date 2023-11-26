/**
 * \file    boot_talk.h
 *
 *          Boot talk component.
 * 
 * \author  Kamil Kiełbasa 
 */

// Header guard ---------------------------------------
#ifndef BOOT_TALK_H
#define BOOT_TALK_H

// Include files --------------------------------------
// Defines --------------------------------------------
// Types and type definitions -------------------------
typedef enum
{
    BOOT_REASON_NORMAL,         //< normal boot, BOR or SW.
    BOOT_REASON_WATCHDOG,       //< booted because of watchdog.
    BOOT_REASON_ASSERT,         //< assert detected.
    BOOT_REASON_FLASH_FAILURE,  //< flash shared state failure detected.
    BOOT_REASON_UNDEFINED,      //< know firmware case but not defined here.
    BOOT_REASON_MAX = BOOT_REASON_UNDEFINED,
} boot_reason_t;

// Module interface variables and constants -----------
// Extern variables and constant declarations ---------
// Module interface function declarations -------------

/**
 * @brief Getter for device boot reason.
 * 
 * @return boot reason based on RCC->CSR.
 */
boot_reason_t boot_talk_get_reason( void );

#endif /* BOOT_TALK_H */

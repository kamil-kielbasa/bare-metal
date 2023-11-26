/**
 * \file    peripherals.h
 *
 *          Common peripherals setup.
 * 
 * \author  Kamil Kiełbasa 
 */

// Header guard ---------------------------------------
#ifndef PERIPHERALS_H
#define PERIPHERALS_H

// Include files --------------------------------------
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_pwr_ex.h"
#include "stm32l4xx_hal_rcc.h"
#include "stm32l4xx_hal_rcc_ex.h"
#include "stm32l4xx_hal_usart.h"
#include "stm32l4xx_hal_iwdg.h"
#include "stm32l4xx_hal_flash.h"
#include "stm32l4xx_hal_flash_ex.h"

// Defines --------------------------------------------
#define BUTTON_BLUE_Pin GPIO_PIN_13
#define BUTTON_BLUE_GPIO_Port GPIOC
#define BUTTON_BLUE_EXTI_IRQn EXTI15_10_IRQn

#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

// Types and type definitions -------------------------
// Module interface variables and constants -----------
// Extern variables and constant declarations ---------
extern UART_HandleTypeDef uart1_handle;
extern IWDG_HandleTypeDef iwdg_handle;

// Module interface function declarations -------------

/*
 * Clocks configuration:
 * - system clock (SYSCLK) will have (max) 80 MHz frequency.
 * - input for SYSCLK will be phase-locked loop (PLLCLK).
 * - input for PLLCLK will be multi-speed internal clock (MSI).
 * - to achieve max frequency for system clock we need to setup 4 level of flash latency.
 * - all buses (AHB, APB1, APB2) will be divided by 1 because we want keep maximum frequency.
 * - we setup low-speed internal clock (LSI) for independent watchdog.
 */
void peripherals_system_clocks_init( void );

/*
 * Blue button configuration:
 * - enable AHB2 peripheral clock for port C.
 * - setup GPIO pin, mode and pull.
 * - enabe interrupt with proper preemption priority.
 */
void peripherals_blue_button_init( void );

/*
 * LED configuration:
 * - enable AHB2 peripheral clock for port A.
 * - setup GPIO pin, mode, pull and speed.
 * - by default, set pin to low state.
 */
void peripherals_led_init( void );

/*
 * UART1 configuration:
 * - set peripherial clock config.
 * - enable AHB2 peripheral clock for port A and USART1.
 * - setup GPIO pin, mode, pull, speed and alternate.
 * - setup UART parameters.
 */
void peripherals_uart1_init( void );

/*
 * IWDG configuration:
 * - LSI clock has 32kHz frequency.
 * - we divide 32kHz for 128 and achieve 250 ticks per second.
 * - watchdog window is 10 seconds. 
 * 
 * Calculations:
 * ( 32000 (LSI) / 128 ) * X (seconds) = 2500 (window)
 */
void peripherals_iwdg_init( void );

#endif /* MAIN_H */

/**
 * \file    peripherals.c
 *
 *          Common peripherals setup.
 * 
 * \author  Kamil Kiełbasa 
 */

// Include files --------------------------------------
#include "peripherals.h"
#include <stdbool.h>

// Module defines -------------------------------------
// Module types and type definitiones -----------------
UART_HandleTypeDef uart1_handle;
IWDG_HandleTypeDef iwdg_handle;

// Module interface variables and constants -----------
// Static variables and constants ---------------------
// Static function declarations -----------------------
static void error_handler( void );

// Static function definitions ------------------------
static void error_handler( void )
{
    __disable_irq();
    while ( true ) {};
}

// Module interface function definitions --------------


void peripherals_system_clocks_init( void )
{
    /* Regulator voltage output range 1 mode, typical output voltage at 1.2 V, system frequency up to 80 MHz. */
    if ( HAL_OK != HAL_PWREx_ControlVoltageScaling( PWR_REGULATOR_VOLTAGE_SCALE1 ) )
    {
        error_handler();
    }

    /* Initializes the RCC oscillators according to the below specified parameters. */
    RCC_OscInitTypeDef rcc_osc_config =
    {
        .OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_MSI,
        .LSIState = RCC_LSI_ON,
        .MSIState = RCC_MSI_ON,
        .MSICalibrationValue = 0,
        .MSIClockRange = RCC_MSIRANGE_6,
        .PLL.PLLState = RCC_PLL_ON,
        .PLL.PLLSource = RCC_PLLSOURCE_MSI,
        .PLL.PLLM = 1,
        .PLL.PLLN = 40,
        .PLL.PLLP = RCC_PLLP_DIV7,
        .PLL.PLLQ = RCC_PLLQ_DIV2,
        .PLL.PLLR = RCC_PLLR_DIV2,
    };

    if ( HAL_OK != HAL_RCC_OscConfig( &rcc_osc_config ) )
    {
        error_handler();
    }

    /* Initializes the CPU, AHB and APB buses clocks. */
    uint32_t flash_latency = FLASH_LATENCY_4;
    RCC_ClkInitTypeDef rcc_clk_config =
    {
        .ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
        .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
        .AHBCLKDivider = RCC_SYSCLK_DIV1,
        .APB1CLKDivider = RCC_HCLK_DIV1,
        .APB2CLKDivider = RCC_HCLK_DIV1,
    };

    if ( HAL_OK != HAL_RCC_ClockConfig( &rcc_clk_config, flash_latency ) )
    {
        error_handler();
    }
}

void peripherals_blue_button_init( void )
{
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef blue_button_config =
    {
        .Pin = BUTTON_BLUE_Pin,
        .Mode = GPIO_MODE_IT_RISING,
        .Pull = GPIO_NOPULL,
    };

    HAL_GPIO_Init( BUTTON_BLUE_GPIO_Port, &blue_button_config );

    const uint32_t preempt_prio = 16;
    const uint32_t sub_prio = 0;
    HAL_NVIC_SetPriority( EXTI15_10_IRQn, preempt_prio, sub_prio );
    HAL_NVIC_EnableIRQ( EXTI15_10_IRQn );
}

void peripherals_led_init( void )
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef led_config =
    {
        .Pin = LD2_Pin,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    
    HAL_GPIO_Init( LD2_GPIO_Port, &led_config );
    HAL_GPIO_WritePin( LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET );
}

void peripherals_uart1_init( void )
{
    /* Set configuration for peripherial clocks. */
    RCC_PeriphCLKInitTypeDef periph_clk_config =
    { 
        .PeriphClockSelection = RCC_PERIPHCLK_USART1,
        .Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK,
    };

    if ( HAL_OK != HAL_RCCEx_PeriphCLKConfig( &periph_clk_config ) )
    {
        error_handler();
    }

    /* Enable clocks for GPIO & UART. */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    /* GPIO configuration: PA9 is TX, PA10 is RX. */
    GPIO_InitTypeDef uart_config = 
    {
        .Pin = GPIO_PIN_9 | GPIO_PIN_10,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF7_USART1,
    };

    HAL_GPIO_Init( GPIOA, &uart_config );

    /* UART1 configuration. */
    uart1_handle = ( UART_HandleTypeDef )
    {
        .Instance = USART1,
        .Init.BaudRate = 115200,
        .Init.WordLength = UART_WORDLENGTH_8B,
        .Init.StopBits = UART_STOPBITS_1,
        .Init.Parity = UART_PARITY_NONE,
        .Init.Mode = UART_MODE_TX_RX,
        .Init.HwFlowCtl = UART_HWCONTROL_NONE,
        .Init.OverSampling = UART_OVERSAMPLING_16,
        .Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE,
        .AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT,
    };

    if ( HAL_OK != HAL_UART_Init( &uart1_handle ) )
    {
        error_handler();
    }

    const uint32_t preempt_priority = 1;
    const uint32_t sub_priority = 0;
    HAL_NVIC_SetPriority( USART1_IRQn, preempt_priority, sub_priority );
    HAL_NVIC_EnableIRQ( USART1_IRQn );
}

void peripherals_iwdg_init( void )
{
    iwdg_handle = ( IWDG_HandleTypeDef )
    {
        .Instance = IWDG,
        .Init.Prescaler = IWDG_PRESCALER_128,
        .Init.Window = 2500,
        .Init.Reload = 2500,
    };

    if ( HAL_OK != HAL_IWDG_Init( &iwdg_handle ) )
    {
        error_handler();
    }

    HAL_IWDG_Refresh( &iwdg_handle );
}

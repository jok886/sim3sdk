//-------------------------------------------------------------------------------
// Copyright (c) 2012 by Silicon Laboratories
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v10/License_Agreement_v10.htm
//
//
// Original content and implementation provided by Silicon Laboratories
//-------------------------------------------------------------------------------

//==============================================================================
// WARNING:
//
// This file is auto-generated by AppBuilder and should not be modified.
// Any hand modifications will be lost if the project is regenerated.
//==============================================================================
#include "circular_buffer.h"
#include "gTIMER.h"

// Include peripheral access modules used in this file
#include <SI32_TIMER_A_Type.h>
#include <si32_device.h>

//==============================================================================
// 1st Level Interrupt Handlers
//==============================================================================
/* Timer0L for EP1, Timer0H for EP2, Timer1L for EP3, Timer2L for EP4*/
void TIMER0L_IRQHandler()
{
    if (SI32_TIMER_A_is_low_overflow_interrupt_pending(SI32_TIMER_0))
    {
        stop_timer(0);
    }
}

void TIMER0H_IRQHandler()
{
    if (SI32_TIMER_A_is_high_overflow_interrupt_pending(SI32_TIMER_0))
    {
        stop_timer(1);
    }
}

void TIMER1L_IRQHandler()
{
    if (SI32_TIMER_A_is_low_overflow_interrupt_pending(SI32_TIMER_1))
    {
        stop_timer(2);
    }
}

void TIMER1H_IRQHandler()
{
    if (SI32_TIMER_A_is_high_overflow_interrupt_pending(SI32_TIMER_1))
    {
        stop_timer(3);
    }
}

/*
 * idx indicate Timer0L, Timer0H, Timer1L,Timer1H
 * Timer affect circular in buffer.
 */
void stop_timer(uint8_t idx)
{
    uint8_t ep = (idx + 1) | 0x80;
    circular_buffer_pools_t * cb_in = circular_buffer_pointer(ep);
    uint16_t timer_reload = circular_buffer_timer_reload(cb_in);
    switch(idx)
    {
        case 0:
            SI32_TIMER_A_stop_low_timer (SI32_TIMER_0);
            SI32_TIMER_A_clear_low_overflow_interrupt (SI32_TIMER_0);
            SI32_TIMER_A_set_low_count (SI32_TIMER_0, timer_reload);
            break;
        case 1:
            SI32_TIMER_A_stop_high_timer (SI32_TIMER_0);
            SI32_TIMER_A_clear_high_overflow_interrupt (SI32_TIMER_0);
            SI32_TIMER_A_set_high_count (SI32_TIMER_0, timer_reload);
            break;
        case 2:
            SI32_TIMER_A_stop_low_timer (SI32_TIMER_1);
            SI32_TIMER_A_clear_low_overflow_interrupt (SI32_TIMER_1);
            SI32_TIMER_A_set_low_count (SI32_TIMER_1, timer_reload);
            break;
        case 3:
            SI32_TIMER_A_stop_high_timer (SI32_TIMER_1);
            SI32_TIMER_A_clear_high_overflow_interrupt (SI32_TIMER_1);
            SI32_TIMER_A_set_high_count (SI32_TIMER_1, timer_reload);
            break;
    }
    circular_buffer_put_read_ready(cb_in,1);
}

/* idx indicate Timer0L, Timer0H, Timer1L,Timer1H*/
void start_timer(uint8_t idx)
{
    uint8_t ep = (idx + 1) | 0x80;
    circular_buffer_pools_t * cb_in = circular_buffer_pointer(ep);
    uint16_t timer_reload = circular_buffer_timer_reload(cb_in);

    switch(idx)
    {
        case 0:
            //  Set Next Timer Reload to finish transfer after there is room in FIFO
            SI32_TIMER_A_set_low_count (SI32_TIMER_0, timer_reload);
            SI32_TIMER_A_set_low_reload (SI32_TIMER_0, timer_reload);
            SI32_TIMER_A_start_low_timer (SI32_TIMER_0);
            break;
        case 1:
            //  Set Next Timer Reload to finish transfer after there is room in FIFO
            SI32_TIMER_A_set_high_count (SI32_TIMER_0, timer_reload);
            SI32_TIMER_A_set_high_reload (SI32_TIMER_0, timer_reload);
            SI32_TIMER_A_start_high_timer (SI32_TIMER_0);
            break;
        case 2:
            //  Set Next Timer Reload to finish transfer after there is room in FIFO
            SI32_TIMER_A_set_low_count (SI32_TIMER_1, timer_reload);
            SI32_TIMER_A_set_low_reload (SI32_TIMER_1, timer_reload);
            SI32_TIMER_A_start_low_timer (SI32_TIMER_1);
            break;
        case 3:
            //  Set Next Timer Reload to finish transfer after there is room in FIFO
            SI32_TIMER_A_set_high_count (SI32_TIMER_1, timer_reload);
            SI32_TIMER_A_set_high_reload (SI32_TIMER_1,timer_reload);
            SI32_TIMER_A_start_high_timer (SI32_TIMER_1);
            break;
    }

}

//==============================================================================
// Configuration Functions
//==============================================================================
void TIMER_enter_default_mode_from_reset(void)
{
    // // Setup Timer0 with split mode
    SI32_TIMER_A_select_split_timer_mode(SI32_TIMER_0);
    SI32_TIMER_A_set_clock_divider_counter(SI32_TIMER_0, 256 - APBCLK / TIMER0_BASE_FREQ);
    SI32_TIMER_A_set_clock_divider_reload(SI32_TIMER_0, 256 - APBCLK / TIMER0_BASE_FREQ);
    SI32_TIMER_A_select_low_clock_source_timer_clock_divider(SI32_TIMER_0);
    SI32_TIMER_A_select_high_clock_source_timer_clock_divider(SI32_TIMER_0);

    SI32_TIMER_A_enable_low_overflow_interrupt(SI32_TIMER_0);
    SI32_TIMER_A_enable_high_overflow_interrupt(SI32_TIMER_0);

    NVIC_ClearPendingIRQ(TIMER0L_IRQn);
    NVIC_SetPriority(TIMER0L_IRQn, TIMER0L_InterruptPriority);
    NVIC_EnableIRQ(TIMER0L_IRQn);
    NVIC_ClearPendingIRQ(TIMER0H_IRQn);
    NVIC_SetPriority(TIMER0H_IRQn, TIMER0H_InterruptPriority);
    NVIC_EnableIRQ(TIMER0H_IRQn);

    // Setup Timer1 with split mode
    SI32_TIMER_A_select_split_timer_mode(SI32_TIMER_1);
    SI32_TIMER_A_set_clock_divider_counter(SI32_TIMER_1, 256 - APBCLK / TIMER1_BASE_FREQ);
    SI32_TIMER_A_set_clock_divider_reload(SI32_TIMER_1, 256 - APBCLK / TIMER1_BASE_FREQ);
    SI32_TIMER_A_select_low_clock_source_timer_clock_divider(SI32_TIMER_1);
    SI32_TIMER_A_select_high_clock_source_timer_clock_divider(SI32_TIMER_1);

    SI32_TIMER_A_enable_low_overflow_interrupt(SI32_TIMER_1);
    SI32_TIMER_A_enable_high_overflow_interrupt(SI32_TIMER_1);

    NVIC_ClearPendingIRQ(TIMER1L_IRQn);
    NVIC_SetPriority(TIMER1L_IRQn, TIMER1L_InterruptPriority);
    NVIC_EnableIRQ(TIMER1L_IRQn);

    NVIC_ClearPendingIRQ(TIMER1H_IRQn);
    NVIC_SetPriority(TIMER1H_IRQn, TIMER1H_InterruptPriority);
    NVIC_EnableIRQ(TIMER1H_IRQn);
}

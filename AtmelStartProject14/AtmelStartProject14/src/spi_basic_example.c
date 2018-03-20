/**
 * \file
 *
 * \brief SPI basic driver example.
 *
 *
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <atmel_start.h>
#include <spi_basic_example.h>
#include <spi_basic.h>
#include <atomic.h>

static uint8_t buffer[16] = "data";

static void drive_slave_select_low(void);
static void drive_slave_select_high(void);

static void drive_slave_select_low()
{
	// Control GPIO to drive SS_bar low
}

static void drive_slave_select_high()
{
	// Control GPIO to drive SS_bar high
}

uint8_t SPI_0_test_spi_basic(void)
{

	// Register callback function releasing SS when transfer is complete
	SPI_0_register_callback(drive_slave_select_high);

	// SPI Basic driver is in IRQ-mode, enable global interrupts.
	ENABLE_INTERRUPTS();

	// Test driver, assume that the SPI MISO and MOSI pins have been looped back

	drive_slave_select_low();
	SPI_0_exchange_block(buffer, sizeof(buffer));

	while (SPI_0_status_busy())
		; // Wait for the transfer to complete

	// Check that the correct data was received
	if (strncmp((char *)buffer, "data", strlen("data")))
		return 0; // ERROR: Wrong data received

	// If we get here, everything was OK
	return 1;
}
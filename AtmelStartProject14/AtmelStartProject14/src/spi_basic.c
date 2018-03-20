/**
 * \file
 *
 * \brief SPI basic driver.
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
#include <avr/io.h>
#include <spi_basic.h>
#include <atmel_start_pins.h>

#define MOSI_bm 1<<2 //on port C
#define SCK_bm 1<<0 //on port C
#define SS_bm 1<<3 //on port A
#define SPI_PORT PORTC //We can now use SPI_PORT.DIR, .OUT etc.
#define SS_PORT PORTA //SS is on a different port


void SPI_MasterInit()
{
	// Initialize the SPI port as master
	// You will need to set MOSI, SCK, SS (slave select) as outputs
	
	//Connect OLED to EXT3
	
	PORTMUX.CTRLB |= PORTMUX_SPI0_bm; //To use alternative SPI pins
	
	SPI_PORT.DIR |= MOSI_bm | SCK_bm; //Set pins as output
	SS_PORT.DIR |= SS_bm;

	SS_PORT.OUTSET = SS_bm; //Set SS high -> OLED inactive
	
	
	// Now enable SPI, Master and set clock rate
	
	SPI0.CTRLA |= SPI_ENABLE_bm | SPI_MASTER_bm; //Default clock divisor of 4 is fine
	
	

	//Make sure SS does not disable master mode (Possibly not required)
	SPI0.CTRLB |= SPI_SSD_bm;
}


void SPI_MasterTransmit(char cData)
{
	// First select the correct slave by setting its slave select (SS) LOW
	SS_PORT.OUTCLR = SS_bm;

	// Then start the transmission by assigning the data to the SPI data register
	SPI0.DATA = cData;

	// Now wait for the data transmission to complete by periodically checking the SPI status register
	//the SPI_IF and SPI_WRCOL is the only interrupt flag with a function in non-buffered mode.
	while(!(SPI0.INTFLAGS & SPI_IF_bm)){
		
	}
	SPI0.DATA; //Dummy read to clear flag

	// Finally set the slave select bit HIGH before leaving the function
	SS_PORT.OUTSET = SS_bm;
}



typedef struct SPI_0_descriptor_s {
	spi_transfer_status_t  status;
	uint8_t *              data;
	uint8_t                size;
	spi_transfer_type_t    type;
	spi_transfer_done_cb_t cb;
} SPI_0_descriptor_t;

static SPI_0_descriptor_t SPI_0_desc;

void SPI_0_init()
{

	SPI0.CTRLA = 1 << SPI_CLK2X_bp    /* Enable Double Speed: enabled */
	             | 0 << SPI_DORD_bp   /* Data Order Setting: disabled */
	             | 1 << SPI_ENABLE_bp /* Enable Module: enabled */
	             | 0 << SPI_MASTER_bp /* SPI module in slave mode */
	             | SPI_PRESC_DIV4_gc; /* System Clock / 4 */

	// SPI0.CTRLB = 0 << SPI_BUFEN_bp /* Buffer Mode Enable: disabled */
	//		 | 0 << SPI_BUFWR_bp /* Buffer Write Mode: disabled */
	//		 | SPI_MODE_0_gc /* SPI Mode 0 */
	//		 | 0 << SPI_SSD_bp; /* Slave Select Disable: disabled */

	SPI0.INTCTRL = 0 << SPI_DREIE_bp    /* Data Register Empty Interrupt Enable: disabled */
	               | 1 << SPI_IE_bp     /* Interrupt Enable: enabled */
	               | 0 << SPI_RXCIE_bp  /* Receive Complete Interrupt Enable: disabled */
	               | 0 << SPI_SSIE_bp   /* Slave Select Trigger Interrupt Enable: disabled */
	               | 0 << SPI_TXCIE_bp; /* Transfer Complete Interrupt Enable: disabled */

	SPI_0_desc.status = SPI_FREE;
	SPI_0_desc.cb     = NULL;
}

void SPI_0_enable()
{
	SPI0.CTRLA |= SPI_ENABLE_bm;
}

void SPI_0_disable()
{
	SPI0.CTRLA &= ~SPI_ENABLE_bm;
}

/*
  Callback function is typically used to:
  In master mode: Release SS after transfer as finished.
  In slave mode: Implement SPI protocol, setting up next transfer
                 after the previous one (i.e. data transfer phase
                 after control/command phase).
*/
void SPI_0_register_callback(spi_transfer_done_cb_t f)
{
	SPI_0_desc.cb = f;
}

ISR(SPI0_INT_vect)
{
	/* SPI_0_desc.data points to array element
	   to write the received data to. The data to be transmitted
	   is in the next array element.
	*/
	uint8_t rdata = SPI0.DATA;
	uint8_t wdata = 0;

	SPI0.INTFLAGS = SPI_RXCIF_bm;

	if (SPI_0_desc.type != SPI_WRITE) {
		*SPI_0_desc.data = rdata;
	}

	SPI_0_desc.data++;

	if (SPI_0_desc.type != SPI_READ)
		wdata = *SPI_0_desc.data;

	SPI_0_desc.size--;
	// if more bytes to be transferred
	if (SPI_0_desc.size != 0) {
		// more data to send, send a byte
		SPI0.DATA = wdata;
	}

	// if last byte has been transferred, update status
	// and optionally call callback
	else {
		SPI_0_desc.status = SPI_DONE;
		if (SPI_0_desc.cb != NULL) {
			SPI_0_desc.cb();
		}
	}
}

bool SPI_0_status_free()
{
	return (SPI_0_desc.status == SPI_FREE);
}

bool SPI_0_status_idle()
{
	return (SPI_0_desc.status == SPI_IDLE);
}

bool SPI_0_status_busy()
{
	return (SPI_0_desc.status == SPI_BUSY);
}

bool SPI_0_status_done()
{
	return (SPI_0_desc.status == SPI_DONE);
}

uint8_t SPI_0_exchange_byte(uint8_t data)
{
	// Blocking wait for SPI free makes the function work
	// seamlessly also with IRQ drivers.
	while (SPI_0_desc.status == SPI_BUSY)
		;
	SPI0.DATA = data;
	while (!(SPI0.INTFLAGS & SPI_RXCIF_bm))
		;
	return SPI0.DATA;
}

void SPI_0_exchange_block(void *block, uint8_t size)
{
	SPI_0_desc.data   = (uint8_t *)block;
	SPI_0_desc.size   = size;
	SPI_0_desc.type   = SPI_EXCHANGE;
	SPI_0_desc.status = SPI_BUSY;

	SPI0.DATA = *SPI_0_desc.data;
}

void SPI_0_write_block(void *block, uint8_t size)
{
	SPI_0_desc.data   = (uint8_t *)block;
	SPI_0_desc.size   = size;
	SPI_0_desc.type   = SPI_WRITE;
	SPI_0_desc.status = SPI_BUSY;

	SPI0.DATA = *SPI_0_desc.data;
}

void SPI_0_read_block(void *block, uint8_t size)
{
	SPI_0_desc.data   = (uint8_t *)block;
	SPI_0_desc.size   = size;
	SPI_0_desc.type   = SPI_READ;
	SPI_0_desc.status = SPI_BUSY;

	SPI0.DATA = 0;
}
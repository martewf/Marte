//**
 //* \file spi_basic.h
 //*
 //* \brief SPI basic driver.
 //*
 //*
 //* Copyright (C) 2016 Atmel Corporation. All rights reserved.
 //*
 //* \asf_license_start
 //*
 //* \page License
 //*
 //* Redistribution and use in source and binary forms, with or without
 //* modification, are permitted provided that the following conditions are met:
 //*
 //* 1. Redistributions of source code must retain the above copyright notice,
 //*    this list of conditions and the following disclaimer.
 //*
 //* 2. Redistributions in binary form must reproduce the above copyright notice,
 //*    this list of conditions and the following disclaimer in the documentation
 //*    and/or other materials provided with the distribution.
 //*
 //* 3. The name of Atmel may not be used to endorse or promote products derived
 //*    from this software without specific prior written permission.
 //*
 //* 4. This software may only be redistributed and used in connection with an
 //*    Atmel microcontroller product.
 //*
 //* THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 //* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 //* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 //* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 //* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 //* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 //* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 //* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 //* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 //* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 //* POSSIBILITY OF SUCH DAMAGE.
 //*
 //* \asf_license_stop
 //*
 //*
 //*/

/**
 * \defgroup doc_driver_spi_basic SPI Basic
 * \ingroup doc_driver_spi
  *
 * \section doc_driver_spi_basic_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */

#ifndef SPI_BASIC_H_INCLUDED
#define SPI_BASIC_H_INCLUDED

#include <compiler.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Function pointer to callback function called by SPI IRQ.
    NULL=default value: No callback function is to be used.
*/
typedef void (*spi_transfer_done_cb_t)(void);

/** Specify whether the SPI transfer is to be uni- or bidirectional.
    A bidirectional transfer (=SPI_EXCHANGE) causes the received data
    to overwrite the buffer with the data to transmit.
*/
typedef enum spi_transfer_type {
	SPI_EXCHANGE, ///< SPI transfer is bidirectional
	SPI_READ,     ///< SPI transfer reads, writes only 0s
	SPI_WRITE     ///< SPI transfer writes, discards read data
} spi_transfer_type_t;

/** Status of the SPI hardware and SPI bus.*/
typedef enum spi_transfer_status {
	SPI_FREE, ///< SPI hardware is not open, bus is free.
	SPI_IDLE, ///< SPI hardware has been opened, no transfer ongoing.
	SPI_BUSY, ///< SPI hardware has been opened, transfer ongoing.
	SPI_DONE  ///< SPI hardware has been opened, transfer complete.
} spi_transfer_status_t;

/**
 * \brief Initialize SPI interface
 * If module is configured to disabled state, the clock to the SPI is disabled
 * if this is supported by the device's clock system.
 *
 * \return Initialization status.
 * \retval 0 the SPI init was successful
 * \retval 1 the SPI init was not successful
 */
void SPI_0_init(void);

/**
 * \brief Enable SPI_0
 * 1. If supported by the clock system, enables the clock to the SPI
 * 2. Enables the SPI module by setting the enable-bit in the SPI control register
 *
 * \return Nothing
 */
void SPI_0_enable();

/**
 * \brief Disable SPI_0
 * 1. Disables the SPI module by clearing the enable-bit in the SPI control register
 * 2. If supported by the clock system, disables the clock to the SPI
 *
 * \return Nothing
 */
void SPI_0_disable();

/**
 * \brief Exchange one byte over SPI SPI_0. Blocks until done.
 *
 * \param[in] data The byte to transfer
 *
 * \return Received data byte.
 */
uint8_t SPI_0_exchange_byte(uint8_t data);

/**
 * \brief Exchange a buffer over SPI SPI_0. Blocks if using polled driver.
 *
 * \param[inout] block The buffer to transfer. Received data is returned here.
 * \param[in] size The size of buffer to transfer
 *
 * \return Nothing.
 */
void SPI_0_exchange_block(void *block, uint8_t size);

/**
 * \brief Write a buffer over SPI SPI_0. Blocks if using polled driver.
 *
 * \param[in] block The buffer to transfer
 * \param[in] size The size of buffer to transfer
 *
 * \return Nothing.
 */
void SPI_0_write_block(void *block, uint8_t size);

/**
 * \brief Read a buffer over SPI SPI_0. Blocks if using polled driver.
 *
 * Zeros are transmitted out of the SPI.
 *
 * \param[out] block Received data is written here.
 * \param[in] size The size of buffer to transfer
 *
 * \return Nothing.
 */
void SPI_0_read_block(void *block, uint8_t size);

/**
 * \brief Register a callback function to be called at the end of the SPI ISR.
 *
 * \param[in] f Pointer to function to be called
 *
 * \return Nothing.
 */
void SPI_0_register_callback(spi_transfer_done_cb_t f);

/**
 * \brief Check if SPI bus is FREE.
 *
 * \return SPI free status
 * \retval true  SPI bus is FREE
 * \retval false SPI bus is not FREE
 */
bool SPI_0_status_free(void);

/**
 * \brief Check if SPI bus is IDLE.
 *
 * \return SPI IDLE status
 * \retval true  SPI bus is IDLE
 * \retval false SPI bus is not IDLE
 */
bool SPI_0_status_idle(void);

/**
 * \brief Check if SPI bus is BUSY.
 *
 * \return SPI BUSY status
 * \retval true  SPI bus is BUSY
 * \retval false SPI bus is not BUSY
 */
bool SPI_0_status_busy(void);

/**
 * \brief Check if SPI bus is DONE.
 *
 * \return SPI DONE status
 * \retval true  SPI bus is DONE
 * \retval false SPI bus is not DONE
 */
bool SPI_0_status_done(void);

#ifdef __cplusplus
}
#endif

#endif /* SPI_BASIC_H_INCLUDED */

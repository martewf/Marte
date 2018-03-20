

#include <stdio.h>
#include <string.h>
#include <atmel_start.h>


uint8_t buffer[16] = "data";
volatile int8_t status = 0; // Just to keep pass() and fail() from being optimized away

void fail(void);
void drive_slave_select(void);
void release_slave_select(void);
void fail(void);
void pass(void);

void drive_slave_select_low(){
	// Control GPIO to drive SS_bar low
}

void drive_slave_select_high(){
	// Control GPIO to drive SS_bar high
}

void fail(){
	status = -1;
	while(1);
}

void pass(){
	status = 1;
	while(1);
}



int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	// Register callback function releasing SS when transfer is complete
	SPI_0_register_callback(drive_slave_select_high);
	
	// If SPI Basic driver is in IRQ-mode, enable global interrupts.
	sei();

	// Test driver functions, assumes that the SPI MISO and MOSI pins
	// have been looped back, and that the open/close interface was enabled
	// during configuration in START
	
	if(!SPI_0_open(SPI_0_DEFAULT))
	// Not able to open SPI, call fail() or optionally do something
	// else waiting for the SPI to become free
	fail();
	
	drive_slave_select_low();
	SPI_0_exchange_block(buffer, sizeof(buffer));
	while(SPI_0_status_busy()); // Wait for the transfer to complete
	SPI_0_close();
	
	// Check that the correct data was received
	if (strncmp((char*)buffer, "data", strlen("data")))
	fail();
	
	// If we get here, everything was OK
	pass();

	/* Replace with your application code */
	while (1) {
	}
}


#include <atmel_start.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include<stdio.h>


int main(void)
{
	atmel_start_init();
	DISP_init();
	set_time();
	set_sleep_mode (SLEEP_MODE_STANDBY);
	
	while (1) 
	{	
		sleep_enable();
		sleep_mode();
		
	}
}





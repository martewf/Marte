//
// display.c
//

#define DSP_MODE_bm 1<<5
#define DSP_MODE_PORT PORTC
#define DSP_RST_bm 1<<5
#define DSP_RST_PORT PORTB
#define F_CPU 333333


// System headers
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

// Project headers
#include "display.h"
#include "spi_basic.h"
#include "font8x8.h"


// This file contains a simple driver for the display so that one can print characters and strings.
// You shouldn't need to do anything in this file for things to work.
// Of course, you're welcome to check it out if you want to see how things work under the hood.

void DISP_rst();
void DISP_transmit_cmd(uint8_t cmd);
void DISP_transmit_data(uint8_t data);
void DISP_set_mode(bool mode);

int cursor_row = 0;
int cursor_col = 0;

void DISP_print(const char* str)
{
	while(*str != '\0')
	{
		DISP_putc(*str);
		str++;
	}
}

void DISP_putc(char c)
{

	if(c == '\n')
	{
		if(cursor_col == 0)
		{
			DISP_putc(' ');
		}

		while(cursor_col != 0)
		{
			DISP_putc(' ');
		}
		return;
	}

	else if(c == '\t')
	{
		int count = 3 - (cursor_col % 4);

		for(int i = 0; i < count; i++)
		{
			DISP_putc(' ');
		}
		return;
	}
	
	else if(c < 0x20)
	{
		return;
	}
	else if(c < 0x7F)
	{

		
		uint8_t d[8] = {
			pgm_read_byte(&(font8x8_basic[(int)c - 0x20][0])),
			pgm_read_byte(&(font8x8_basic[(int)c - 0x20][1])),
			pgm_read_byte(&(font8x8_basic[(int)c - 0x20][2])),
			pgm_read_byte(&(font8x8_basic[(int)c - 0x20][3])),
			pgm_read_byte(&(font8x8_basic[(int)c - 0x20][4])),
			pgm_read_byte(&(font8x8_basic[(int)c - 0x20][5])),
			pgm_read_byte(&(font8x8_basic[(int)c - 0x20][6])),
			pgm_read_byte(&(font8x8_basic[(int)c - 0x20][7]))
		};
		
		for(uint8_t b = 0; b < 8; b++)
		{
			uint8_t db = 0;
			db |= (((d[0] >> b) & 0x1) << 0);
			db |= (((d[1] >> b) & 0x1) << 1);
			db |= (((d[2] >> b) & 0x1) << 2);
			db |= (((d[3] >> b) & 0x1) << 3);
			db |= (((d[4] >> b) & 0x1) << 4);
			db |= (((d[5] >> b) & 0x1) << 5);
			db |= (((d[6] >> b) & 0x1) << 6);
			db |= (((d[7] >> b) & 0x1) << 7);

			DISP_transmit_data(db);
		}
	}
	else if(c < 0xA0)
	{
		return;
	}
	else if(c > 0xC0)
	{

		
		uint8_t d[8] = {
			pgm_read_byte(&(font8x8_ext_latin[(int)c - 0xc0][0])),
			pgm_read_byte(&(font8x8_ext_latin[(int)c - 0xc0][1])),
			pgm_read_byte(&(font8x8_ext_latin[(int)c - 0xc0][2])),
			pgm_read_byte(&(font8x8_ext_latin[(int)c - 0xc0][3])),
			pgm_read_byte(&(font8x8_ext_latin[(int)c - 0xc0][4])),
			pgm_read_byte(&(font8x8_ext_latin[(int)c - 0xc0][5])),
			pgm_read_byte(&(font8x8_ext_latin[(int)c - 0xc0][6])),
			pgm_read_byte(&(font8x8_ext_latin[(int)c - 0xc0][7]))
		};
		
		for(uint8_t b = 0; b < 8; b++)
		{
			uint8_t db = 0;
			db |= (((d[0] >> b) & 0x1) << 0);
			db |= (((d[1] >> b) & 0x1) << 1);
			db |= (((d[2] >> b) & 0x1) << 2);
			db |= (((d[3] >> b) & 0x1) << 3);
			db |= (((d[4] >> b) & 0x1) << 4);
			db |= (((d[5] >> b) & 0x1) << 5);
			db |= (((d[6] >> b) & 0x1) << 6);
			db |= (((d[7] >> b) & 0x1) << 7);

			DISP_transmit_data(db);
		}
	}

	

	cursor_col++;
	cursor_col %= 16;

	if(cursor_col == 0)
	{
		cursor_row++;
		cursor_row %= 4;
	}
}

void DISP_clear()
{
	if(cursor_row == 0)
	{
		DISP_print("\n");
	}
	while(cursor_row != 0)
	{
		DISP_print("\n");
	}

	for(int i = 0; i < 4 * 16; i++)
	{
		DISP_putc(' ');
	}
}

void DISP_write_bitmap(const uint8_t* bm)
{
	
	for(uint8_t y = 0; y < 4; y++)
	{
		for(uint8_t x = 0; x < 16; x++)
		{
			uint8_t d[8] = {
				pgm_read_byte(&(bm[(8 * y + 0) * 16 + x])),
				pgm_read_byte(&(bm[(8 * y + 1) * 16 + x])),
				pgm_read_byte(&(bm[(8 * y + 2) * 16 + x])),
				pgm_read_byte(&(bm[(8 * y + 3) * 16 + x])),
				pgm_read_byte(&(bm[(8 * y + 4) * 16 + x])),
				pgm_read_byte(&(bm[(8 * y + 5) * 16 + x])),
				pgm_read_byte(&(bm[(8 * y + 6) * 16 + x])),
				pgm_read_byte(&(bm[(8 * y + 7) * 16 + x]))
			};

			for(uint8_t b = 0; b < 8; b++)
			{
				uint8_t db = 0;
				db |= (((d[0] >> b) & 0x1) << 0);
				db |= (((d[1] >> b) & 0x1) << 1);
				db |= (((d[2] >> b) & 0x1) << 2);
				db |= (((d[3] >> b) & 0x1) << 3);
				db |= (((d[4] >> b) & 0x1) << 4);
				db |= (((d[5] >> b) & 0x1) << 5);
				db |= (((d[6] >> b) & 0x1) << 6);
				db |= (((d[7] >> b) & 0x1) << 7);

				DISP_transmit_data(db);
			}
		}
	}
}

void DISP_transmit_cmd(uint8_t cmd)
{
	DISP_set_mode(0);
	SPI_MasterTransmit(cmd);
}

void DISP_transmit_data(uint8_t time)
{
	DISP_set_mode(1);
	SPI_MasterTransmit(time);
}

// mode: 1 = data, 0 = cmd
void DISP_set_mode(bool mode)
{
	if(mode)
	{
		DSP_MODE_PORT.OUTSET = DSP_MODE_bm;
	}
	else
	{
		DSP_MODE_PORT.OUTCLR = DSP_MODE_bm;

	}
}

void DISP_init()
{
	DSP_RST_PORT.DIRSET = DSP_RST_bm;
	DSP_MODE_PORT.DIRSET = DSP_MODE_bm;

	SPI_MasterInit();

	DISP_set_mode(1);

	DISP_rst();

	DISP_transmit_cmd(0xAE); // display off

	// p32
	DISP_transmit_cmd(0xD5); // clock
	DISP_transmit_cmd(0xF0); // upper nibble is rate, lower nibble is divisor

	// p31
	DISP_transmit_cmd(0xA8); // mux ratio p31
	DISP_transmit_cmd(0x20); // rtfm

	// p31
	DISP_transmit_cmd(0xD3); // display offset
	DISP_transmit_cmd(0x00); // rtfm

	// p3 (appendix)
	DISP_transmit_cmd(0x8D); // charge pump
	DISP_transmit_cmd(0x14); // enable

	// p30, p34
	DISP_transmit_cmd(0x20); // memory addr mode
	DISP_transmit_cmd(0x00); // horizontal

	// p31
	DISP_transmit_cmd(0xA1); // segment remap
	DISP_transmit_cmd(0xA5); // display on

	// p31
	DISP_transmit_cmd(0xC8); // com scan direction

	// p31
	DISP_transmit_cmd(0x7F); // display start line

	// p31
	DISP_transmit_cmd(0x22); // set page address
	DISP_transmit_cmd(0x00);
	DISP_transmit_cmd(0x03);

	// p31, p40
	DISP_transmit_cmd(0xDA); // com hardware cfg
	DISP_transmit_cmd(0x02); // com cfg

	// p28
	DISP_transmit_cmd(0x81); // contrast aka current
	DISP_transmit_cmd(0xFF); // 128 is midpoint

	// p32
	DISP_transmit_cmd(0xD9); // precharge
	DISP_transmit_cmd(0x11); // rtfm

	// p32
	DISP_transmit_cmd(0xDB); // vcomh deselect level
	DISP_transmit_cmd(0x20); // rtfm

	DISP_transmit_cmd(0xA6); // non-inverted
	DISP_transmit_cmd(0xA4); // display scan on
	DISP_transmit_cmd(0xAF); // drivers on

	DISP_clear();
}

void DISP_rst()
{
	DSP_RST_PORT.OUTCLR = DSP_RST_bm;
	_delay_ms(10);
	DSP_RST_PORT.OUTSET = DSP_RST_bm;
	_delay_ms(10);
}


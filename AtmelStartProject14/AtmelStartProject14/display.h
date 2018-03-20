//
// display.h
//

#ifndef DISPLAY_H_
#define DISPLAY_H_

// Below are the public functions that you can use to play with the screen

// Call this method to initialize the screen. This must be done before using any other functions
void DISP_init();

// Call this method to write a bitmap to the screen. The bitmap must be a 32x16(512) array of bytes
// where each row of 16 bytes (128 bits) is one row of monochrome pixels on the screen.
// Black = 0, White = 1
void DISP_write_bitmap(const uint8_t* bitmap);

// This clears the display
void DISP_clear();

// Print a single character to the screen
void DISP_putc(char c);

// Print a string literal to the screen
void DISP_print(const char* str);


#endif /* DISPLAY_H_ */
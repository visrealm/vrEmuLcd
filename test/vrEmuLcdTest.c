#include "vrEmuLcd.h"
#include <stdio.h>

#define LCD_WIDTH 16
#define LCD_HEIGHT 2

int main()
{
	VrEmuLcd *lcd = vrEmuLcdNew(LCD_WIDTH, LCD_HEIGHT, EmuLcdRomA00);

	// send it commands:
	vrEmuLcdSendCommand(lcd, LCD_CMD_FUNCTION | LCD_CMD_FUNCTION_LCD_2LINE | 0x10);
	vrEmuLcdSendCommand(lcd, LCD_CMD_CLEAR);
	vrEmuLcdSendCommand(lcd, LCD_CMD_HOME);
	vrEmuLcdSendCommand(lcd, LCD_CMD_DISPLAY | LCD_CMD_DISPLAY_ON);

	// send it data
	vrEmuLcdWriteByte(lcd, 'H');
	vrEmuLcdWriteByte(lcd, 'e');
	vrEmuLcdWriteByte(lcd, 'l');
	vrEmuLcdWriteByte(lcd, 'l');
	vrEmuLcdWriteByte(lcd, 'o');

	// or cheat
	vrEmuLcdWriteString(lcd, " world!");

	// then periodically, render it. 
	vrEmuLcdUpdatePixels(lcd);   // generates a snapshot of the pixels state

	for (int y = 0; y < vrEmuLcdNumPixelsY(lcd); ++y) {
	  for (int x = 0; x < vrEmuLcdNumPixelsX(lcd); ++x) {
		  
		// do whatever you like with the pixel information. render it to a texture, output it to  a console, whatever
	   // values returned are:  -1 = no pixel (character borders), 0 = pixel off, 1 = pixel on
		char pixel = vrEmuLcdPixelState(lcd, x, y);
		printf("%c", (pixel<0)?' ':(pixel==0?'.':'#'));
	  }
	  printf("\n");
	}
	
	vrEmuLcdDestroy(lcd);
}

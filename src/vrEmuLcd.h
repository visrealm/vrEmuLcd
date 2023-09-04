/*
 * Troy's HD44780U Lcd Display Emulator
 *
 * Copyright (c) 2020 Troy Schrapel
 *
 * This code is licensed under the MIT license
 *
 * https://github.com/visrealm/VrEmuLcd
 *
 */

#ifndef _VR_EMU_LCD_H_
#define _VR_EMU_LCD_H_

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define VR_EMU_LCD_DLLEXPORT EMSCRIPTEN_KEEPALIVE
#elif VR_EMU_LCD_COMPILING_DLL
#define VR_EMU_LCD_DLLEXPORT __declspec(dllexport)
#elif defined WIN32 && !defined VR_EMU_LCD_STATIC
#define VR_EMU_LCD_DLLEXPORT __declspec(dllimport)
#else
#define VR_EMU_LCD_STATIC 1
#ifdef __cplusplus
#define VR_EMU_LCD_DLLEXPORT extern "C"
#else
#define VR_EMU_LCD_DLLEXPORT extern
#endif
#endif

#include <stdint.h>

 /* PRIVATE DATA STRUCTURE
  * ---------------------------------------- */
struct vrEmuLcd_s;
typedef struct vrEmuLcd_s VrEmuLcd;

/* PUBLIC CONSTANTS
 * ---------------------------------------- */

const uint8_t LCD_CMD_CLEAR = 0x01;
const uint8_t LCD_CMD_HOME = 0x02;
const uint8_t LCD_CMD_ENTRY_MODE = 0x04;
const uint8_t LCD_CMD_ENTRY_MODE_INCREMENT = 0x02;
const uint8_t LCD_CMD_ENTRY_MODE_DECREMENT = 0x00;
const uint8_t LCD_CMD_ENTRY_MODE_SHIFT = 0x01;
const uint8_t LCD_CMD_DISPLAY = 0x08;
const uint8_t LCD_CMD_DISPLAY_ON = 0x04;
const uint8_t LCD_CMD_DISPLAY_CURSOR = 0x02;
const uint8_t LCD_CMD_DISPLAY_CURSOR_BLINK = 0x01;
const uint8_t LCD_CMD_SHIFT = 0x10;
const uint8_t LCD_CMD_SHIFT_CURSOR = 0x00;
const uint8_t LCD_CMD_SHIFT_DISPLAY = 0x08;
const uint8_t LCD_CMD_SHIFT_LEFT = 0x00;
const uint8_t LCD_CMD_SHIFT_RIGHT = 0x04;
const uint8_t LCD_CMD_FUNCTION = 0x20;
const uint8_t LCD_CMD_FUNCTION_LCD_1LINE = 0x00;
const uint8_t LCD_CMD_FUNCTION_LCD_2LINE = 0x08;
const uint8_t LCD_CMD_FUNCTION_EXT_MODE = 0x04;
const uint8_t LCD_CMD_FUNCTION_STD_MODE = 0x00;
const uint8_t LCD_CMD_EXT_FUNCTION_GFX = 0x02;
const uint8_t LCD_CMD_EXT_FUNCTION_STD = 0x00;
const uint8_t LCD_CMD_SET_CGRAM_ADDR = 0x40;
const uint8_t LCD_CMD_SET_DRAM_ADDR = 0x80;


typedef enum
{
  EmuLcdRomA00,  // Japanese
  EmuLcdRomA02   // European
} vrEmuLcdCharacterRom;

/* PUBLIC INTERFACE
 * ---------------------------------------- */

 /* Function:  vrEmuLcdNew
  * --------------------
  * create a new LCD
  *
  * cols: number of display columns  (8 to 40)
  * rows: number of display rows (1, 2 or 4)
  * rom:  character rom to load
  */
VR_EMU_LCD_DLLEXPORT
VrEmuLcd* vrEmuLcdNew(int width, int height, vrEmuLcdCharacterRom rom);

/* Function:  vrEmuLcdDestroy
 * --------------------
 * destroy an LCD
 *
 * lcd: lcd object to destroy / clean up
 */
VR_EMU_LCD_DLLEXPORT
void vrEmuLcdDestroy(VrEmuLcd* lcd);

/* Function:  vrEmuLcdSendCommand
 * --------------------
 * send a command to the lcd (RS is low, R/W is low)
 *
 * data: the data (DB0 -> DB7) to send
 */
VR_EMU_LCD_DLLEXPORT
void vrEmuLcdSendCommand(VrEmuLcd* lcd, uint8_t data);


/* Function:  vrEmuLcdWriteByte
 * --------------------
 * write a byte to the lcd (RS is high, R/W is low)
 *
 * data: the data (DB0 -> DB7) to send
 */
VR_EMU_LCD_DLLEXPORT
void vrEmuLcdWriteByte(VrEmuLcd* lcd, uint8_t data);

/* Function:  vrEmuLcdWriteString
 * ----------------------------------------
 * write a string to the lcd
 * iterates over the characters and sends them individually
 *
 * str: the string to write.
 */
VR_EMU_LCD_DLLEXPORT
void vrEmuLcdWriteString(VrEmuLcd* lcd, const char* str);


/* Function:  vrEmuLcdGetDataOffset
 * ----------------------------------------
 * return the character offset in ddram for a given
 * row and column on the display.
 *
 * can be used to set the current cursor address
 */
VR_EMU_LCD_DLLEXPORT
int vrEmuLcdGetDataOffset(VrEmuLcd* lcd, int row, int col);

/* Function:  vrEmuLcdReadByte
 * --------------------
 * read a byte from the lcd (RS is high, R/W is high)
 *
 * returns: the data (DB0 -> DB7) at the current address
 */
VR_EMU_LCD_DLLEXPORT
uint8_t vrEmuLcdReadByte(VrEmuLcd* lcd);

/* Function:  vrEmuLcdReadByteNoInc
 * --------------------
 * read a byte from the lcd (RS is high, R/W is high)
 * don't update the address/scroll
 *
 * returns: the data (DB0 -> DB7) at the current address
 */
VR_EMU_LCD_DLLEXPORT
uint8_t vrEmuLcdReadByteNoInc(VrEmuLcd* lcd);

/* Function:  vrEmuLcdReadAddress
 * --------------------
 * read the current address offset (RS is low, R/W is high)
 *
 * returns: the current address offset (either CGRAM or DDRAM)
 */
VR_EMU_LCD_DLLEXPORT
uint8_t vrEmuLcdReadAddress(VrEmuLcd* lcd);


/* Function:  vrEmuLcdCharBits
 * ----------------------------------------
 * return a character's pixel data
 *
 * pixel data consists of 5 uint8_ts where each is
 * a vertical row of bits for the character
 *
 * c: character index
 *    0 - 15   cgram
 *    16 - 255 rom
 */
VR_EMU_LCD_DLLEXPORT
const uint8_t* vrEmuLcdCharBits(VrEmuLcd* lcd, uint8_t c);

/* Function:  vrEmuLcdUpdatePixels
 * ----------------------------------------
 * updates the display's pixel data
 * changes are only reflected in the pixel data when this function is called
 */
VR_EMU_LCD_DLLEXPORT
void vrEmuLcdUpdatePixels(VrEmuLcd* lcd);

/* Function:  vrEmuLcdNumPixels
 * ----------------------------------------
 * get the size of the entire display in pixels (including unused border pixels)
 */
VR_EMU_LCD_DLLEXPORT
void vrEmuLcdNumPixels(VrEmuLcd* lcd, int* width, int* height);

/* Function:  vrEmuLcdNumPixelsX
 * ----------------------------------------
 * returns: number of horizontal pixels in the display
 */
VR_EMU_LCD_DLLEXPORT
int vrEmuLcdNumPixelsX(VrEmuLcd* lcd);

/* Function:  vrEmuLcdNumPixelsY
 * ----------------------------------------
 * returns: number of vertical pixels in the display
 */
VR_EMU_LCD_DLLEXPORT
int vrEmuLcdNumPixelsY(VrEmuLcd* lcd);

/* Function:  charvrEmuLcdPixelState
 * ----------------------------------------
 * returns: pixel state at the given location
 *
 * -1 = no pixel (character borders)
 *  0 = pixel off
 *  1 = pixel on
 *
 */
VR_EMU_LCD_DLLEXPORT
char vrEmuLcdPixelState(VrEmuLcd* lcd, int x, int y);

#endif // _VR_EMU_LCD_H_

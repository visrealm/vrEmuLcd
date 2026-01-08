# vrEmuLcd - HD44780 Character LCD Emulator

| Platform | Native | WebAssembly |
| --- | --- | --- |
| Ubuntu | <a href="https://github.com/visrealm/vrEmuLcd/actions/workflows/cmake-ubuntu.yml"><img src="https://github.com/visrealm/vrEmuLcd/actions/workflows/cmake-ubuntu.yml/badge.svg"/></a> | <a href="https://github.com/visrealm/vrEmuLcd/actions/workflows/webassembly-ubuntu.yml"><img src="https://github.com/visrealm/vrEmuLcd/actions/workflows/webassembly-ubuntu.yml/badge.svg"/></a> |
| Windows | <a href="https://github.com/visrealm/vrEmuLcd/actions/workflows/cmake-windows.yml"><img src="https://github.com/visrealm/vrEmuLcd/actions/workflows/cmake-windows.yml/badge.svg"/></a> | <a href="https://github.com/visrealm/vrEmuLcd/actions/workflows/webassembly-windows.yml"><img src="https://github.com/visrealm/vrEmuLcd/actions/workflows/webassembly-windows.yml/badge.svg"/></a> |

Core engine written in C with a flexible Web front-end.

This Character LCD Emulator can be used to emulate most standard LCD displays.

It accepts and responds to most commands listed in the [HD44780 datasheet](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)

It also now support most commands for a 128x64 graphics LCD [12864B datasheet](https://www.exploreembedded.com/wiki/images/7/77/QC12864B.pdf)

## Screenshots:

#### Hello, World!
<img src="res/vrEmuLcd.gif" alt="Hello, World!" width="400px">

#### Different Colors
<img src="res/helloworld2.png" alt="Different Colors!" width="400px">

#### Different Sizes
<img src="res/lcd2004_demo.png" alt="Different Sizes!" width="400px">

#### 128 x 64 Graphics LCD
<img src="https://visrealm.github.io/hbc-56/img/glcd_basic.gif" alt="HBC-56 Emulator LCD Window" width="400px">

#### Custom characters (CGRAM support)
<img src="res/the8bitguy.png" alt="Custom characters (CGRAM support)!" width="400px">

## Live examples:

* [Hello, World!](https://visrealm.github.io/vrEmuLcd/examples/helloworld)
* [The 8-Bit Guy: Character LCD](https://visrealm.github.io/vrEmuLcd/examples/8bitguy)
* [Hello Graphics LCD](https://visrealm.github.io/vrEmuLcd/examples/hellogfx)
* [Troy's Breadboard Computer!](https://cpu.visualrealmsoftware.com/emu/?h=7e387e0c7e01370730b800b801bdebbdc20f48b1bd7f4e07bd81b805b8061701ba040b4703ccc0b80307b3c050323937c0da392b40b8044f02b1bd7f4f05b1495704ce71bd76b9058a0abd7f4f02bd7f4e4956fa3e61bdd94f0317fddaba022f6c4f06b149bd7f4ebd76b9067ecc4701fc4700fc2f22c107eff83e7e0fbd6ebdabc0b017a4ce0a491730da014eb0ce0a174ace0740d4f0460f07d80f01e039a3cd2f9d010a49d00a88fc6e17f0070fd8da0f0ef83cb7780a17bdb5b5b5cd3fc1c16e7e80078a0f1050fec0e13ec87ec00f0a50fec0e13ed26e4700c00f3af83ee84701c0b8010730b8006e0740f00f0a074ab05182fec0c172fa3ef3566e0000000064000000000000000f08080808080808080808080808080f1f00000000000000000000000000001f1f00000000000000000000000000001f1e02020202020202020202020202021e0f08080808080808080808080808080f1f00000000000000000000000000001f1f00000000000000000000000000001f1e02020202020202020202020202021e2054544c2020000204062053636f7265536e616b6521010305070302011514131211252423222135343340f080ff1010200100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000&s=300)

## Usage

### C
```c
#define LCD_WIDTH 20
#define LCD_HEIGHT 4

VrEmuLcd      *lcd = vrEmuLcdNew(LCD_WIDTH, LCD_HEIGHT, EmuLcdRomA00);

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
  }
}
```

### Web

#### HTML (local)
```html
<script src="src/vrEmuLcd.js"></script>
<script src="bin/vrEmuLcdWasm.js"></script>
```
#### HTML (live)
```html
<script src="https://visrealm.github.io/vrEmuLcd/src/vrEmuLcd.js"></script>
<script src="https://visrealm.github.io/vrEmuLcd/bin/vrEmuLcdWasm.js"></script>
```

## Example

```js
<canvas id="lcd"></canvas>
...
<script>
    var canv = document.getElementById('lcd');
    var ctx = canv.getContext('2d');

    vrEmuLcd.setLoadedCallback(function () {

      // create a new LCD object
      var lcd = vrEmuLcd.newLCD(16, 2, vrEmuLcd.CharacterRom.Eurpoean);

      // set up the display
      lcd.sendCommand(LCD_CMD_DISPLAY | LCD_CMD_DISPLAY_ON);

      lcd.writeString("Hello, World!");

      lcd.render(ctx, 0, 0, 800, 400);
    });
</script>
```


## Building

### Native (desktop)
```bash
cmake -S . -B build/native -DCMAKE_BUILD_TYPE=Release
cmake --build build/native --config Release
ctest --test-dir build/native --build-config Release
```

### WebAssembly with CMake

Configure with the Emscripten toolchain and build the `vrEmuLcdWasm` target to produce the WebAssembly bundle:

```bash
emcmake cmake -S . -B build/wasm -DCMAKE_BUILD_TYPE=Release
cmake --build build/wasm --target vrEmuLcdWasm --config Release
```

Artifacts will be written to `build/wasm/bin/` (the loader script is `vrEmuLcdWasm.js` with an accompanying `.wasm` file). Use `-DVR_EMU_LCD_BUILD_WASM=OFF` if you want to skip the wasm target when using Emscripten.

If Emscripten is not installed, a platform-independent bootstrap helper can install it and print the toolchain file path. Examples:

```bash
# POSIX shells
TOOLCHAIN=$(python tools/bootstrap_emscripten.py --emsdk-dir build/emsdk)
cmake -S . -B build/wasm -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" -DCMAKE_BUILD_TYPE=Release
cmake --build build/wasm --target vrEmuLcdWasm --config Release
```

```powershell
# PowerShell
$toolchain = python tools/bootstrap_emscripten.py --emsdk-dir build/emsdk
cmake -S . -B build/wasm -DCMAKE_TOOLCHAIN_FILE=$toolchain -DCMAKE_BUILD_TYPE=Release
cmake --build build/wasm --target vrEmuLcdWasm --config Release
```

Use `--force` to ignore an existing `EMSCRIPTEN` environment and re-install locally, or `--version <tag>` to pin a specific emsdk version.
    

## LCD API

#### constuctor
```js
var lcd = vrEmuLcd.newLCD(columns, rows, charSet);
```
- `columns`: - number of columns
- `rows`: - number of rows
- `charSet`: - character set. One of: `vrEmuLcd.CharacterRom.European`, `vrEmuLcd.CharacterRom.Japanese`

#### sendCommand(commandByte)
Send a command to the instruction register of the lcd
Command constants are defined:
- `LCD_CMD_CLEAR` - clear the display

- `LCD_CMD_HOME` - reset display to home position

- `LCD_CMD_ENTRY_MODE` - entry mode (the following to be bitwise-OR'd)
  - `LCD_CMD_ENTRY_MODE_INCREMENT` - automatically increment the cursor or display
  - `LCD_CMD_ENTRY_MODE_DECREMENT` - automatically decrement the cursor or display
  - `LCD_CMD_ENTRY_MODE_SHIFT` - automaticallt shift the entire display instead of the cursor

- `LCD_CMD_DISPLAY` - display mode (the following to be bitwise-OR'd)
  - `LCD_CMD_DISPLAY_ON` - turn the display on
  - `LCD_CMD_DISPLAY_CURSOR` - display a cursor (bottom row)
  - `LCD_CMD_DISPLAY_CURSOR_BLINK` - display a blink cursor (flashing entire character block)

- `LCD_CMD_SHIFT` - move the cursor or scroll display (the following to be bitwise-OR'd)
  - `LCD_CMD_SHIFT_CURSOR` - shift the cursor (default)
  - `LCD_CMD_SHIFT_DISPLAY` - shift the display
  - `LCD_CMD_SHIFT_LEFT` - shift the cursor or display left (default)
  - `LCD_CMD_SHIFT_RIGHT` - shift the cursor or display right

- `LCD_CMD_SET_CGRAM_ADDR` - set the CGRAM address (actual address uses lower 6 bits)

- `LCD_CMD_SET_DRAM_ADDR` - set the CGRAM address (actual address uses lower 7 bits)

#### writeByte(dataByte)
Write a byte to the data register of the lcd

#### writeString(str)
Write a string to the data register of the lcd

#### getDataOffset(screenX, screenY)
Return the ddram offset for the given screen location

#### readByte()
Read the current byte from cgram or ddram (determined by current address) 

#### readAddress()
Read the current address offset in cgram or ddram

#### pixelState(pixelX, pixelY)
Return the pixel state at the given location
- `-1` - no pixel (eg. margin)
- `0` - pixel off
- `1` - pixel on

#### colorScheme
Set/get the color scheme. eg:
```js
lcd.colorScheme = vrEmuLcd.Schemes.WhiteOnBlue;
```
Standard color schemes:
- `vrEmuLcd.Schemes.WhiteOnBlue` (default)
- `vrEmuLcd.Schemes.BlackOnBlue`
- `vrEmuLcd.Schemes.BlackOnGreen`
- `vrEmuLcd.Schemes.RedOnBlack`
- `vrEmuLcd.Schemes.BlueOnBlack`

or, provide your own. `{ BackColor: <backcolor>, PixelOnColor: <pixeloncolor>, PixelOffColor: <pixeloffcolor> }`

#### render(ctx, x, y, width, height)
Render to a 2d canvas context
- `ctx` - the canvas to render to

## License
This code is licensed under the [MIT](https://opensource.org/licenses/MIT "MIT") license

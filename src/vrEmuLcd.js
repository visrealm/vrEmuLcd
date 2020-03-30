/*
 * Troy's HD44780U Lcd Display Emulator
 *
 * Copyright (c) 2020 Troy Schrapel
 *
 * This code is licensed under the MIT license
 *
 * https://github.com/visrealm/VrEmuLcd
 *
 * vrEmuLcdWasm.js must be loaded AFTER this file
 */

vrEmuLcd = {

  // call this function to return a new LCD object
  // it will only be available once the wasm module is loaded
  newLCD: function() { alert("vrEmuLcdModule not yet loaded"); },

  // set your callback here. it will be called once the module 
  // is ready (or immediately if the module is ready now).
  // in your callback, vrEmuLcd will be valid and you can create
  //  a new lcd.
  setLoadedCallback: function(onLoadedCallback) {
    vrEmuLcd._onLoaded = onLoadedCallback;
    if (vrEmuLcd._isLoaded) {
      onLoadedCallback();
    }
  },

  CharacterRom: { 
    A00: 0,  Japanese: 0,
    A02: 1,  European: 1,
  },

  // PRIVATES
  _isLoaded: false,
  _onLoaded: function () {},
};

// COMMAND CONSTANTS
// ----------------------------------------------

// clear
const LCD_CMD_CLEAR = 0b00000001;

// home
const LCD_CMD_HOME = 0b00000010;
  
// entry mode
const LCD_CMD_ENTRY_MODE = 0b00000100;
const LCD_CMD_ENTRY_MODE_INCREMENT = 0b00000010;
const LCD_CMD_ENTRY_MODE_DECREMENT = 0b00000000;
const LCD_CMD_ENTRY_MODE_SHIFT = 0b00000001;
 
// display
const LCD_CMD_DISPLAY = 0b00001000;
const LCD_CMD_DISPLAY_ON = 0b00000100;
const LCD_CMD_DISPLAY_CURSOR = 0b00000010;
const LCD_CMD_DISPLAY_CURSOR_BLINK = 0b00000001;
  
// shift
const LCD_CMD_SHIFT = 0b00010000;
const LCD_CMD_SHIFT_CURSOR = 0b00000000;
const LCD_CMD_SHIFT_DISPLAY = 0b00001000;
const LCD_CMD_SHIFT_LEFT = 0b00000000;
const LCD_CMD_SHIFT_RIGHT = 0b00000100;
  
// cgram
const LCD_CMD_SET_CGRAM_ADDR = 0b01000000;

// ddram
const LCD_CMD_SET_DRAM_ADDR = 0b10000000;

// STANDARD COLOR SCHEMES
// ----------------------------------------------
vrEmuLcd.Schemes = {
  WhiteOnBlue: {
    BackColor: "#1f1fff",
    PixelOnColor: "#f0f0ff",
    PixelOffColor: "#0000e0"
  },
  BlackOnBlue: {
    BackColor: "#5CAAEA",
    PixelOnColor: "#000941",
    PixelOffColor: "#518BCA"
  },
  BlackOnGreen: {
    BackColor: "#7DBE00",
    PixelOnColor: "#000",
    PixelOffColor: "#6FB900"
  },
  RedOnBlack: {
    BackColor: "#212225",
    PixelOnColor: "#FB3349",
    PixelOffColor: "#202729"
  },
  BlueOnBlack: {
    BackColor: "#212225",
    PixelOnColor: "#B0F7FE",
    PixelOffColor: "#202729"
  }
}

vrEmuLcdModule = {
  onRuntimeInitialized: function ()
  {
    var _newLcd = vrEmuLcdModule.cwrap('vrEmuLcdNew', 'number', ['number','number','number']);

    var _destroyLCD = vrEmuLcdModule.cwrap('vrEmuLcdDestroy', null, ['number']);
    var _sendCommand = vrEmuLcdModule.cwrap('vrEmuLcdSendCommand', null, ['number','number']);
    var _writeByte = vrEmuLcdModule.cwrap('vrEmuLcdWriteByte', null, ['number','number']);
    var _writeString = vrEmuLcdModule.cwrap('vrEmuLcdWriteString', null, ['number','string']);
    var _getDataOffset = vrEmuLcdModule.cwrap('vrEmuLcdGetDataOffset', 'number', ['number','number','number']);
    var _readByte = vrEmuLcdModule.cwrap('vrEmuLcdReadByte', 'number', ['number']);
    var _readAddress = vrEmuLcdModule.cwrap('vrEmuLcdReadAddress', 'number', ['number']);
    var _updatePixels = vrEmuLcdModule.cwrap('vrEmuLcdUpdatePixels', null, ['number']);
    var _numPixelsX = vrEmuLcdModule.cwrap('vrEmuLcdNumPixelsX', 'number', ['number']);
    var _numPixelsY = vrEmuLcdModule.cwrap('vrEmuLcdNumPixelsY', 'number', ['number']);
    var _pixelState = vrEmuLcdModule.cwrap('vrEmuLcdPixelState', 'number', ['number','number','number']);

    // replace vrEmuLcd.newLCD() with the real function.
    // returns:
    //   an object which wraps the wasm lcd engine
    vrEmuLcd.newLCD = function(widthChars, heightChars, characterRom) {

      // default character rom
      if (characterRom == null)
      {
        characterRom = vrEmuLcd.CharacterRom.European;
      }

      // get the LCD object from the C engine
      var lcd = _newLcd(widthChars, heightChars, characterRom);
      
      // build the new LCD object wrapper
      return {
        
        // destroy the wasm lcd object
        destroy: function() {
          _destroyLCD(lcd);
        },

        // send a command to the instruction register of the lcd
        // equivalent to:
        //   1. setting RS to low (instruction register)
        //   2. setting R/W to low (write)
        //   3. setting D7-D0 to commandByte
        //   4. pulsing E
        sendCommand: function(commandByte) {
          _sendCommand(lcd, commandByte);
        },
        
        // write a byte to the data register of the lcd
        // equivalent to:
        //   1. setting RS to high (data register)
        //   2. setting R/W to low (write)
        //   3. setting D7-D0 to dataByte
        //   4. pulsing E
        writeByte: function(dataByte) {
          _writeByte(lcd, dataByte);
        },
        
        // write a string to the data register of the lcd
        // this is equivalent to calling writeByte() for each
        // character in your string
        writeString: function(str) {
          _writeString(lcd, str);
        },
        
        // return the ddram offset for the given screen location
        getDataOffset: function(screenX, screenY) {
          _getDataOffset(lcd, screenX, screenY);
        },
        
        // read the current byte from cgram or ddram (determined by current address) 
        // equivalent to:
        //   1. setting RS to high (data register)
        //   2. setting R/W to high (read)
        //   3. pulsing E
        //   4. returning D7-D0        
        readByte: function() {
          return _readByte(lcd);
        },
        
        // read the current address pointer offset in cgram or ddram
        // equivalent to:
        //   1. setting RS to low
        //   2. setting R/W to high (read)
        //   3. pulsing E
        //   4. returning D6-D0
        readAddress: function() {
          return _readAddress(lcd);
        },

        // display size in pixels
        numPixelsX: _numPixelsX(lcd),
        numPixelsY: _numPixelsY(lcd),

        // update the internal pixel data
        updatePixels: function() {
          _updatePixels(lcd);
        },
        
        // return the pixel state at the given location
        // returns:
        //   -1  - no pixel (eg. margin)
        //    0  - pixel off
        //    1  - pixel on
        pixelState: function(pixelX, pixelY) {
          return _pixelState(lcd, pixelX, pixelY);
        },
        
        // the color scheme used for rendering
        // can be replaced at any time and is used by render()
        colorScheme: vrEmuLcd.Schemes.WhiteOnBlue,

        // render the display to a canvas
        render: function(ctx, xPos, yPos, width, height) {
          var xbuffer = 7;
          var ybuffer = 3.5;
          var totalPixelsX = this.numPixelsX + (xbuffer * 2);
          var totalPixelsY = this.numPixelsY + (ybuffer * 2);
          var lcdScale = Math.min(height /totalPixelsY, width / totalPixelsX);

          ctx.fillStyle = this.colorScheme.BackColor;
          ctx.fillRect(xPos, yPos, totalPixelsX * lcdScale, totalPixelsY * lcdScale);
  
          this.updatePixels();
          for (var y = 0; y < this.numPixelsY; ++y) {

            for (var x = 0; x < this.numPixelsX; ++x) {
              var s = this.pixelState(x, y);
              switch (s) {
                case -1:
                  continue;
  
                case 0:
                  ctx.fillStyle = this.colorScheme.PixelOffColor;
                  break;
  
                case 1:
                  ctx.fillStyle = this.colorScheme.PixelOnColor;
                  break;
  
              }
              ctx.fillRect(xPos + (x + xbuffer) * lcdScale, yPos + (y + ybuffer) * lcdScale, lcdScale * 0.75, lcdScale * 0.75);
            }
          }
        }
      }
    }

    vrEmuLcd.isLoaded = true;
    vrEmuLcd._onLoaded();
   }
}
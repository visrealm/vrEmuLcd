:: Troy's HD44780U Lcd Display Emulator
:: 
:: Copyright (c) 2020 Troy Schrapel
:: 
:: This code is licensed under the MIT license
:: 
:: https://github.com/visrealm/VrEmuLcd
:: 
:: Pre-requisites:
:: This batch file must be run in an emscripten environment
:: eg. emsdk activate

emcc -o bin\vrEmuLcdWasm.js ^
  -Oz ^
  -D _EMSCRIPTEN src\vrEmuLcd.c ^
  -s EXPORT_NAME="'vrEmuLcdModule'" ^
  -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall','cwrap']"
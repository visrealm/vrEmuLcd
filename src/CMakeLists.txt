add_library(vrEmuLcd vrEmuLcd.c)

# Warning flags tuned per toolchain to avoid MSVC-style translations when using Emscripten on Windows.
set(_vr_warning_flags "")
if (CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
  set(_vr_warning_flags /W4)
elseif (NOT VR_EMU_LCD_USING_EMSDK)
  set(_vr_warning_flags -Wall -Wextra -Wpedantic)
elseif (VR_EMU_LCD_USING_EMSDK AND NOT CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
  set(_vr_warning_flags -Wall -Wextra -Wpedantic)
endif()
if (_vr_warning_flags)
  target_compile_options(vrEmuLcd PRIVATE ${_vr_warning_flags})
endif()

if (WIN32)
  if (BUILD_SHARED_LIBS)
     add_definitions(-DVR_EMU_LCD_COMPILING_DLL)
   endif()
endif()

target_include_directories (vrEmuLcd INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if ((VR_EMU_LCD_USING_EMSDK OR CMAKE_SYSTEM_NAME STREQUAL "Emscripten") AND VR_EMU_LCD_BUILD_WASM)
  add_executable(vrEmuLcdWasm vrEmuLcd.c)
  target_compile_options(vrEmuLcdWasm PRIVATE -Oz)
  if (_vr_warning_flags)
    target_compile_options(vrEmuLcdWasm PRIVATE ${_vr_warning_flags})
  endif()
  target_link_options(vrEmuLcdWasm PRIVATE
    -Oz
    "SHELL:-sEXPORT_NAME='vrEmuLcdModule'"
    "SHELL:-sEXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
    "SHELL:--no-entry"
  )
  set_target_properties(vrEmuLcdWasm PROPERTIES
    OUTPUT_NAME "vrEmuLcdWasm"
    SUFFIX ".js"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  )
endif()

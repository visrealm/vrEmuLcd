add_library(vrEmuLcd vrEmuLcd.c)

# Warning flags: strict for native; disabled for Emscripten to avoid MSVC flag translation.
# Per-target warning flags are omitted; rely on top-level settings to avoid MSVC flag translation issues.

if (WIN32)
  if (BUILD_SHARED_LIBS)
     add_definitions(-DVR_EMU_LCD_COMPILING_DLL)
   endif()
endif()

target_include_directories (vrEmuLcd INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if ((VR_EMU_LCD_USING_EMSDK OR CMAKE_SYSTEM_NAME STREQUAL "Emscripten") AND VR_EMU_LCD_BUILD_WASM)
  add_executable(vrEmuLcdWasm vrEmuLcd.c)
  target_compile_options(vrEmuLcdWasm PRIVATE -Oz)
  # Per-target warning flags are omitted; rely on top-level settings to avoid MSVC flag translation issues.
  target_link_options(vrEmuLcdWasm PRIVATE
    -Oz
    "SHELL:-sEXPORT_NAME='vrEmuLcdModule'"
    "SHELL:-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
    "SHELL:--no-entry"
  )
  set_target_properties(vrEmuLcdWasm PROPERTIES
    OUTPUT_NAME "vrEmuLcdWasm"
    SUFFIX ".js"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  )
endif()

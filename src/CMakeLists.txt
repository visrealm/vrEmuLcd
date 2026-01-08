add_library(vrEmuLcd vrEmuLcd.c)

if (WIN32)
  if (BUILD_SHARED_LIBS)
     add_definitions(-DVR_EMU_LCD_COMPILING_DLL)
   endif()
endif()

target_include_directories (vrEmuLcd INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if (EMSCRIPTEN AND VR_EMU_LCD_BUILD_WASM)
  add_executable(vrEmuLcdWasm vrEmuLcd.c)
  target_compile_options(vrEmuLcdWasm PRIVATE -Oz)
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

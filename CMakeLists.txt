cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)

# Detect Emscripten via CMake toolchain or EMSDK env var.
if (NOT DEFINED EMSDK AND DEFINED ENV{EMSDK})
  set(EMSDK "$ENV{EMSDK}")
endif()

set(VR_EMU_LCD_USING_EMSDK OFF)
if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten" OR DEFINED EMSDK)
  set(VR_EMU_LCD_USING_EMSDK ON)
endif()
set(VR_EMU_LCD_USING_EMSDK ${VR_EMU_LCD_USING_EMSDK} CACHE BOOL "Using Emscripten toolchain")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(VR_EMU_LCD_BUILD_WASM "Build WebAssembly artifacts when using Emscripten" ${VR_EMU_LCD_USING_EMSDK})

if (VR_EMU_LCD_USING_EMSDK)
  set(BUILD_SHARED_LIBS OFF)
  add_definitions(-D__EMSCRIPTEN__=1)
endif()

if (NOT BUILD_SHARED_LIBS)
	add_definitions(-DVR_EMU_LCD_STATIC)
endif()

project(vrEmuLcd)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

if (MSVC OR CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
  if (NOT VR_EMU_LCD_USING_EMSDK)
    add_compile_options(/W4 /WX)
  endif()
elseif(NOT VR_EMU_LCD_USING_EMSDK)
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

message(STATUS "VR_EMU_LCD_USING_EMSDK=${VR_EMU_LCD_USING_EMSDK} VR_EMU_LCD_BUILD_WASM=${VR_EMU_LCD_BUILD_WASM}")

include(CTest)

add_subdirectory(src)

if (PROJECT_IS_TOP_LEVEL AND NOT VR_EMU_LCD_USING_EMSDK)
  add_subdirectory(test)
endif()


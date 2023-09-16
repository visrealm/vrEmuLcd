cmake_minimum_required(VERSION 3.22)


set(CMAKE_C_STANDARD 11)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

if (DEFINED EMSCRIPTEN)
	set(BUILD_SHARED_LIBS OFF)
	add_definitions(-D__EMSCRIPTEN__=1)
endif()

if (NOT BUILD_SHARED_LIBS)
	add_definitions(-DVR_EMU_LCD_STATIC)
endif()

project(vrEmuLcd)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
  add_compile_options(-march=native)
endif()

include(CTest)

add_subdirectory(src)

if (PROJECT_IS_TOP_LEVEL)
	add_subdirectory(test)
endif()
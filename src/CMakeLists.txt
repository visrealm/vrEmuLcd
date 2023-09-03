
if (DEFINED EMSCRIPTEN)
	add_executable(vrEmuLcd vrEmuLcd.c)
	set_target_properties(vrEmuLcd PROPERTIES COMPILE_FLAGS "-Os -s SIDE_MODULE=1 ")
	set_target_properties(vrEmuLcd PROPERTIES LINK_FLAGS    "-Os -s EXPORT_NAME='vrEmuLcdModule' -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] --no-entry")
else()
	add_library(vrEmuLcd vrEmuLcd.c)
endif()

if (WIN32)
  if (BUILD_SHARED_LIBS)
     add_definitions(-DVR_EMU_LCD_COMPILING_DLL)
   endif()
endif()

target_include_directories (vrEmuLcd INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()

add_executable (vrEmuLcdTest vrEmuLcdTest.c)

target_link_libraries(vrEmuLcdTest vrEmuLcd)

set(TEST_RUNNER ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}vrEmuLcdTest)

add_test(lcd_hello_world ${TEST_RUNNER})
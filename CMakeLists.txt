cmake_minimum_required(VERSION 3.5)
set(${PROJECT_NAME} wav-player)
project(projekt C CXX)

set(MCU "atmega32")
set(F_CPU "8000000")
set(CMAKE_SYSTEM_NAME Generic)


set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)
set(AVR_DUDE avrdude)
set(OBJCOPY avr-objcopy)

set(CMAKE_C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -O3 -Wall -Wextra -Wpedantic -gstabs")
set(CMAKE_C_LINK_FLAGS "-mmcu=${MCU} -std=c11 -Wall -Wextra -DF_CPU=${F_CPU}")
set(AVR_DUDE_FLAGS -c usbasp -p m32)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
        # LCD library
        src/lib/uTFT-ST7735/uTFT_ST7735.c
        src/lib/uTFT-ST7735/uTFT_ST7735.h
        src/lib/uTFT-ST7735/glcdfont.c

        # SD card, FAT filesystem library
        src/lib/fat-fs/diskio.h
        src/lib/fat-fs/ffconf.h
        src/lib/fat-fs/sdmm.c
        src/lib/fat-fs/ff.c
        src/lib/fat-fs/ff.h
        src/lib/fat-fs/integer.h

        # sources
        src/main.c

        src/player/wav_file.h
        src/player/wav_file.c
        src/player/wav_player.h
        src/player/wav_player.c

        src/player/fifo_buffer.h
        src/player/fifo_buffer.c

        src/view/view.c
        src/view/view.h
        src/controller/controller.c
        src/controller/controller.h
        src/view/screen_utils.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


# AVR dude utils
add_custom_target(1MHz /bin/echo -e "write hfuse 0 0xd9\nwrite lfuse 0 0xe1" | ${AVR_DUDE} -B 3 -t)

add_custom_target(16MHz /bin/echo -e "write hfuse 0 0xd9\nwrite lfuse 0 0xff" | ${AVR_DUDE} -B 3 -t)

add_custom_target(hex ${OBJCOPY} -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex)

add_custom_target(upload ${AVR_DUDE} ${AVR_DUDE_FLAGS} -U flash:w:${PROJECT_NAME}.hex)

# Doxygen support
find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen")
endif (DOXYGEN_FOUND)


cmake_minimum_required(VERSION 2.6)
project(ciotfly)
set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_C_COMPILER /home/zgd/Documents/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc)

#set(CMAKE_CXX_COMPILER /home/zgd/Documents/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

LINK_LIBRARIES(-lpthread -lm -ldl)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


set(SOURCE_FILES
        src/analy.cpp
        src/analy.h
        src/iotfly.cpp
        src/serial_usb_com.cpp
        src/serial_usb_com.h

        src/get_light.cpp
        src/get_light.h

        src/run_light.cpp
        src/run_light.h
        src/creat_graph.cpp
        src/creat_graph.h
        src/analy_route.cpp
        src/analy_route.h
        src/send_commend.cpp
        src/send_commend.h
        src/generate_pos.cpp
        src/generate_pos.h
        src/Serial_RT.cpp
        src/Serial_RT.h
    #    src/client_tcp.cpp
     #   src/client_tcp.h
        )

add_executable(ciotfly ${SOURCE_FILES}  )

target_link_libraries(ciotfly ${OpenCV_LIBS})


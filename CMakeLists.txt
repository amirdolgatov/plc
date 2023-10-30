cmake_minimum_required(VERSION 3.26)
project(client)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_COMPILER /home/amir/Downloads/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

add_executable(client main.cpp
        DIOdriver.cpp
        DIODriver.h
        ModbusClient.h
        Container.h
        ModbusClient.cpp
        Jsonread.h)

add_executable(testing main.cpp)

target_link_libraries(client PUBLIC modbus)
target_link_libraries(client PUBLIC pthread)
target_link_libraries(client PUBLIC jsoncpp)

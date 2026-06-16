cmake_minimum_required(VERSION 3.10)
project(HotelManagement)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/Room.cpp
    src/StandardRoom.cpp
    src/DeluxeRoom.cpp
    src/SuiteRoom.cpp
    src/RoomManager.cpp
    src/Customer.cpp
    src/VIPCustomer.cpp
    src/Booking.cpp
    src/BookingException.cpp
    src/BookingManager.cpp
    src/Date.cpp
    src/Bill.cpp
    src/BillingManager.cpp
    src/Logger.cpp
    src/Validator.cpp
    src/FileHelper.cpp
    src/SystemIntegrator.cpp
)

add_executable(HotelManagement ${SOURCES})

# 测试可执行文件
set(TEST_SOURCES
    src/test_main.cpp
    src/Room.cpp
    src/StandardRoom.cpp
    src/DeluxeRoom.cpp
    src/SuiteRoom.cpp
    src/RoomManager.cpp
    src/Customer.cpp
    src/VIPCustomer.cpp
    src/Booking.cpp
    src/BookingException.cpp
    src/BookingManager.cpp
    src/Date.cpp
)

add_executable(TestHotel ${TEST_SOURCES})

cmake_minimum_required(VERSION 3.10)
project(HotelManagement)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/ui)

# 查找 Qt（优先 Qt6，其次 Qt5）
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 业务逻辑源文件
set(CORE_SOURCES
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

# Qt GUI 源文件
set(UI_SOURCES
    ui/LoginDialog.cpp
    ui/MainWindow.cpp
    ui/DashboardPage.cpp
    ui/RoomPage.cpp
    ui/CustomerPage.cpp
    ui/BookingPage.cpp
    ui/StatsPage.cpp
)

# 主可执行文件（Qt GUI 版）
add_executable(HotelManagement src/main_gui.cpp ${CORE_SOURCES} ${UI_SOURCES})
target_link_libraries(HotelManagement PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# CLI 测试可执行文件
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

cmake_minimum_required(VERSION 3.16)
project(HelloWorldQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake to find both Core and Widgets modules
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Make sure your source file is listed here
add_executable(hello_qt_app src/hello_qt.cpp)

# Link against both Qt6::Core and Qt6::Widgets
target_link_libraries(hello_qt_app PRIVATE Qt6::Core Qt6::Widgets)

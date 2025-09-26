cmake_minimum_required(VERSION 3.16)
project(HelloWorldQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

# Set source directory
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)


# Tell CMake to find both Core and Widgets modules
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
# Add custom module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(NVML REQUIRED)
message(STATUS "NVML LIBRARY: ${NVML_LIBRARIES}")
message(STATUS "NVML INCLUDE: ${NVML_INCLUDE_DIRS}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Make sure your source file is listed here
# else: set Sourcs/directories first and then add executables, see 
# https://stackoverflow.com/questions/13703647/how-to-properly-add-include-directories-with-cmake
add_executable(qt_app 
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/mainwindow.cpp
    ${SRC_DIR}/nvml_vram.cpp
    ${SRC_DIR}/vram_worker.cpp
)

# Link against both Qt6::Core and Qt6::Widgets
target_include_directories(qt_app PRIVATE ${NVML_INCLUDE_DIRS})
target_link_libraries(qt_app PRIVATE Qt6::Core Qt6::Widgets ${NVML_LIBRARIES})

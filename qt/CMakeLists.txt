cmake_minimum_required(VERSION 3.1.0)


project(maquette)

find_package(Qt5Core REQUIRED)
find_package(Qt5Multimedia REQUIRED)



# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

set(maquette_SRCS
    BoardPlayer.cpp
    main.cpp
)


# Tell CMake to create the executable
add_executable(maquette WIN32 ${maquette_SRCS})
target_link_libraries(maquette Qt5::Core Qt5::Multimedia -lwiringPi)

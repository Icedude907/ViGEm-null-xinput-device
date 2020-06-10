cmake_minimum_required(VERSION 3.10)

# set the project name
project(ViGEmNullGamepad)

# add the executable
add_executable(ViGEmNullGamepad src/main.cpp)

# link the libs
target_link_libraries(ViGEmNullGamepad 
    Setupapi
    ${CMAKE_SOURCE_DIR}/libs/ViGEmClient/lib/release/x64/ViGEmClient.lib)

# include the things
target_include_directories(ViGEmNullGamepad PRIVATE libs/ViGEmClient/include)

# set this
#set_property(TARGET ViGEmNullGamepad PROPERTY CXX_STANDARD 14)

# link properly
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MT")
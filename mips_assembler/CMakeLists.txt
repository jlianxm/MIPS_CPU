cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D DEBUG")

message(STATUS "The cmake_cxx_flags is: ${CMAKE_CXX_FLAGS}")

# Set the project name
project(mips_simulator LANGUAGES CXX)

# Create a sources variable with a link to all cpp files to compile
FILE(GLOB SOURCES src/*.cpp)


# Add an executable with the above sources
add_executable(mips_simulator ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(mips_simulator
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)

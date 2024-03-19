cmake_minimum_required(VERSION 3.15)

project(RFuncCore LANGUAGES CXX)


option(RFC_COMPILE_EXAMPLES "Enabling examples compilation" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../Binaries)


find_package(glm CONFIG REQUIRED)


set(CXX_SOURCE_DIR Source)

file(GLOB_RECURSE SOURCE_FILES
    ${CXX_SOURCE_DIR}/*.cpp
    ${CXX_SOURCE_DIR}/*.h
    ${CXX_SOURCE_DIR}/*.hpp
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)
target_include_directories(${PROJECT_NAME} PUBLIC ${CXX_SOURCE_DIR})

if (${RFC_COMPILE_EXAMPLES})
    add_subdirectory(Examples)
endif()


cmake_minimum_required(VERSION 3.15)

project(RFuncCoreExamples LANGUAGES CXX)



add_executable(Ex1 Ex1/main.cpp)
target_link_libraries(Ex1 PUBLIC RFuncCore)


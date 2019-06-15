cmake_minimum_required(VERSION 3.14)
project(My_Complier)

set(CMAKE_CXX_STANDARD 14)

add_executable(My_Complier c_minus.cpp c_minus.h get_token.cpp syntax.cpp expression.cpp vmachine.cpp vmachine.h)
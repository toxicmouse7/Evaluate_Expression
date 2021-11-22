cmake_minimum_required(VERSION 3.20)
project(Evaluate_Expression)

set(CMAKE_CXX_STANDARD 20)

add_executable(Evaluate_Expression main.cpp Evaluator.cpp Evaluator.h)

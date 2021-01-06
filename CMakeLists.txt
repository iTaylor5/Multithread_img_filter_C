cmake_minimum_required(VERSION 3.17)
project(TaylorFilters C)

set(CMAKE_C_STANDARD 99)

add_executable(TaylorFilters TaylorFilters.c BmpProcessor.c PixelProcessor.c)
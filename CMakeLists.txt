cmake_minimum_required(VERSION 3.8)

set(CMAKE_VERBOSE_MAKEFILE ON)

#include("cmake/HunterGate.cmake")
#HunterGate(
#        URL "https://github.com/ruslo/hunter/archive/v0.23.83.tar.gz"
#        SHA1 "12dec078717539eb7b03e6d2a17797cba9be9ba9"
#        LOCAL
#)

project(Milli-library)

add_subdirectory(src)
add_subdirectory(test)

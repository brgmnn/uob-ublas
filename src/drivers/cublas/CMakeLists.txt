cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0012 OLD)

project(uBLAS-driver-cublas)

add_definitions(-std=gnu99 -D BUILD_DRIVER -fpic)

string(REPLACE ":" ";" env-inc "$ENV{INCLUDE}")
string(REPLACE ":" ";" env-lib "$ENV{LD_LIBRARY_PATH}")
include_directories(
    ./
    ~/include
    $ENV{INCLUDEPATH}
)
link_directories(
    ~/lib
    ${env-lib}
)

set(driver-src
    cublas.c
)

add_library(ublas-driver-cublas SHARED ${driver-src})
target_link_libraries(ublas-driver-cublas cublas)


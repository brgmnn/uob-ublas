cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0012 OLD)

project(uBLAS-driver-atlas)

add_definitions(-std=gnu99 -D BUILD_DRIVER -fpic)

string(REPLACE ":" ";" env-inc "$ENV{INCLUDE}")
string(REPLACE ":" ";" env-lib "$ENV{LD_LIBRARY_PATH}")
include_directories(
    ./
    ~/include
    ${env-inc}
)
link_directories(
    ~/lib
    ${env-lib}
)

set(driver-src
    atlas.c
)

add_library(ublas-driver-atlas SHARED ${driver-src})
target_link_libraries(ublas-driver-atlas cblas atlas)


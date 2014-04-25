cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0012 OLD)

project(uBLAS-driver-plasma)

add_definitions(-std=gnu99 -D BUILD_DRIVER -fPIC)

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
    $ENV{LAPACK_DIR}
)

set(driver-src
    plasma.c
)

enable_language(Fortran)
#add_library(ublas-driver-plasma SHARED ${driver-src})
add_executable(ublas-driver-plasma ${driver-src})

target_link_libraries(ublas-driver-plasma plasma coreblasqw coreblas plasma quark cblas atlas lapacke tmg lapack pthread m)
#target_link_libraries(ublas-driver-plasma plasma lapack tmglib f77blas cblas atlas lapacke coreblas pthread m)
set_target_properties(ublas-driver-plasma PROPERTIES LINKER_LANGUAGE Fortran)


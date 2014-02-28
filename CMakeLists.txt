cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)

project(uBLAS)

set(with-atlas 1)
set(with-plasma 1)
add_definitions(-std=c99)


include_directories(src ../include/simple-profiler)
link_directories(../lib/simple-profiler)

set(ublas-src src/ublas.c)

if (${with-atlas})
    list(APPEND ublas-src src/ublas_lib_cblas.c)
endif()

if (${with-plasma})
    list(APPEND ublas-src src/ublas_lib_plasma.c)
endif()

add_library(ublas STATIC ${ublas-src})

if (${with-atlas})
    add_definitions(-D WITH_ATLAS)
    target_link_libraries(ublas cblas)
endif()

if (${with-plasma})
    # set(CMAKE_Fortran_LINK_EXECUTABLE gfortran)
    enable_language(Fortran)
    add_definitions(-D WITH_PLASMA)
    include_directories(../include/plasma)
    link_directories(../lib/plasma)
    target_link_libraries(ublas plasma coreblasqw coreblas plasma quark cblas lapacke tmg lapack refblas pthread m)
endif()


add_executable(measure src/tests/measure.c)
target_link_libraries(measure simpleprofiler ublas)
if (${with-plasma})
    set_target_properties(measure PROPERTIES LINKER_LANGUAGE Fortran)
endif()


cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)

project(uBLAS)

set(with-atlas 1)
# set(with-plasma 1)
add_definitions(-std=gnu99)


string(REPLACE ":" ";" env-inc "$ENV{INCLUDE}")
string(REPLACE ":" ";" env-lib "$ENV{LD_LIBRARY_PATH}")
include_directories(
    src
    ../include/simple-profiler
    ~/.include
    ${env-inc}
)
link_directories(
    ../lib/simple-profiler
    ~/.lib
    ${env-lib}
)

set(ublas-src
    src/ublas.c
    src/utils.c
    src/lib/tpl.c
)

if (${with-atlas})
    list(APPEND ublas-src src/ublas_lib_cblas.c)
endif()

if (${with-plasma})
    list(APPEND ublas-src src/ublas_lib_plasma.c)
endif()

add_library(ublas STATIC ${ublas-src})

if (${with-atlas})
    add_definitions(-D WITH_ATLAS)
    target_link_libraries(ublas cblas atlas)
endif()


if (${with-plasma})
    enable_language(Fortran)
    add_definitions(-D WITH_PLASMA)
    include_directories(../include/plasma)
    link_directories(../lib/plasma)
    target_link_libraries(ublas plasma coreblasqw coreblas plasma quark cblas lapacke tmg lapack refblas pthread m)
endif()


add_executable(measure src/tests/measure.c)
add_executable(accuracy src/tests/accuracy.c)

add_executable(train-fann src/tools/train-fann.c)
add_executable(test-fann src/tools/test-fann.c)

target_link_libraries(measure simpleprofiler ublas fann)
target_link_libraries(accuracy simpleprofiler ublas fann)
target_link_libraries(train-fann simpleprofiler ublas fann)
target_link_libraries(test-fann simpleprofiler ublas fann)


if (${with-plasma})
    set_target_properties(measure PROPERTIES LINKER_LANGUAGE Fortran)
    set_target_properties(accuracy PROPERTIES LINKER_LANGUAGE Fortran)
    set_target_properties(train-fann PROPERTIES LINKER_LANGUAGE Fortran)
    set_target_properties(test-fann PROPERTIES LINKER_LANGUAGE Fortran)
endif()


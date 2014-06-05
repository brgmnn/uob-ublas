cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0012 OLD)

project(uBLAS)

option(with-atlas "Atlas" On)
option(with-cublas "CuBLAS" Off)
option(with-mkl "MKL" Off)
option(with-plasma "Plasma" Off)

option(small-matrix-threshold "SmallMatrixThreshold" 1000000)
option(small-matrix-lib "SmallMatrixLibrary" "Atlas")

add_definitions(-std=gnu99)


string(REPLACE ":" ";" env-inc "$ENV{INCLUDE}")
string(REPLACE ":" ";" env-lib "$ENV{LD_LIBRARY_PATH}")
include_directories(
    src
    ../include/simple-profiler
    ~/include
    ${env-inc}
    $ENV{INCLUDEPATH}
)
link_directories(
    ../lib/simple-profiler
    ~/lib
    ${env-lib}
)

set(ublas-src
    src/ublas.c
	src/driver.c
    src/matrix.c
    src/tests.c
    src/utils.c
)

add_library(ublas STATIC ${ublas-src})
target_link_libraries(ublas dl fann)

if (${with-atlas} MATCHES ON)
    add_definitions(-D WITH_ATLAS)
	add_subdirectory(src/drivers/atlas)
endif()
if (${with-cublas} MATCHES ON)
    add_definitions(-D WITH_CUBLAS)
    add_subdirectory(src/drivers/cublas)
endif()
if (${with-mkl} MATCHES ON)
    add_definitions(-D WITH_MKL)
    add_subdirectory(src/drivers/mkl)
endif()
if (${with-plasma} MATCHES ON)
    add_definitions(-D WITH_PLASMA)
    add_subdirectory(src/drivers/plasma)
endif()


add_executable(accuracy src/tests/accuracy.c)
add_executable(query src/tests/query.c)
add_executable(run src/tests/run.c)
add_executable(demo src/tests/demo.c)
add_executable(train-fann src/tools/train-fann.c)
add_executable(unittests src/unittests/unittests.c)

target_link_libraries(accuracy simpleprofiler ublas)
target_link_libraries(query ublas)
target_link_libraries(run simpleprofiler ublas)
target_link_libraries(demo simpleprofiler ublas)
target_link_libraries(train-fann simpleprofiler ublas)
target_link_libraries(unittests ublas)


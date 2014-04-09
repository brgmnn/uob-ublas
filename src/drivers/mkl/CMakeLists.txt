cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0012 OLD)

project(uBLAS-driver-mkl)

include(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(icc "Intel C Compiler")
# CMAKE_FORCE_CXX_COMPILER(icpc "Intel C++ Compiler")
# CMAKE_FORCE_Fortran_COMPILER(ifort "Intel Fortran Compiler")

add_definitions(-std=gnu99 -D BUILD_DRIVER)
set (CMAKE_SHARED_LINKER_FLAGS "-Bsymbolic") 

string(REPLACE ":" ";" env-inc "$ENV{INCLUDE}")
string(REPLACE ":" ";" env-lib "$ENV{LD_LIBRARY_PATH}")

# this removes the ATLAS include path from includes as there are problems with redefinitions between
# MKL and ATLAS
list(REMOVE_AT env-inc list(FIND env-inc "*atlas*"))

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
    mkl.c
)

add_library(ublas-driver-mkl SHARED ${driver-src})
set_target_properties(ublas-driver-mkl PROPERTIES
	LINK_INTERFACE_LIBRARIES ""
)

# link libraries for blue crystal phase 3 intel compiler with multithreading
target_link_libraries(ublas-driver-mkl
	mkl_lapack95_lp64 mkl_intel_lp64 mkl_gnu_thread mkl_core mkl_avx mkl_def iomp5 pthread m)

# -lmkl_lapack -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -lmkl_sequential -lm
# -liomp5 -lpthread

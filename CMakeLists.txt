
project(rvv)

find_package(OpenBLAS REQUIRED PATHS "RISCV64_GENERIC")

add_executable(main main.cpp)

target_link_libraries(main PUBLIC OpenBLAS::OpenBLAS)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_BUILD_TYPE DEBUG)
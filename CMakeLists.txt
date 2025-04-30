
project(rvv-test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/drops-rvv)

if (CMAKE_CROSSCOMPILING)
    find_package(OpenCV REQUIRED PATHS "/home/zekailin00/Desktop/opencv/build-debug-rvv/drops-debug-rvv")
else()
    find_package(OpenCV REQUIRED)
endif()

if (CMAKE_CROSSCOMPILING)
    add_definitions(-DCMAKE_CROSSCOMPILING)
    set(NOFORTRAN "1")
    set(DYNAMIC_ARCH OFF)
    set(BUILD_WITHOUT_LAPACK OFF)
    set(BUILD_TESTING OFF)
    set(BUILD_BENCHMARKS OFF)
    set(TARGET "RISCV64_GENERIC")
    # add_definitions(-DGEMMINI_BACKEND)
endif()

# add_definitions(-DTIMING_DEBUG -DSMP_DEBUG -DDEBUG_PRINT_KERNEL)
add_definitions(-DEIGEN_USE_BLAS=1)
# find_package(OpenBLAS REQUIRED PATHS "/home/zekailin00/Desktop/rvv-test/drops-rvv/lib/cmake")
add_subdirectory(${CMAKE_SOURCE_DIR}/OpenBLAS)
add_subdirectory(${CMAKE_SOURCE_DIR}/eigen)
add_subdirectory(${CMAKE_SOURCE_DIR}/boost)

set(BIN_NAME matTest_${TARGET})

add_executable(rvvTest main.cpp)

target_compile_definitions(rvvTest PRIVATE EIGEN_USE_BLAS)
target_link_libraries(rvvTest PUBLIC
openblas_static 
# OpenBLAS::OpenBLAS
    Eigen3::Eigen
)
target_include_directories(rvvTest PUBLIC
    /home/zekailin00/Desktop/rvv-test/OpenBLAS/gemmini
)

add_executable(gemminiTest gemmini-test.cpp)
target_compile_definitions(gemminiTest PRIVATE EIGEN_USE_BLAS)
target_link_libraries(gemminiTest PUBLIC
    openblas_static 
    Eigen3::Eigen
)

target_include_directories(gemminiTest PUBLIC
    /home/zekailin00/Desktop/rvv-test/OpenBLAS/gemmini
)

# target_include_directories(rvvTest PUBLIC 
#     /home/zekailin00/Desktop/rvv-test/drops-rvv/include
# )

set(ASM_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/asm.txt")

# Add a post-build custom command
add_custom_command(TARGET rvvTest POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:rvvTest> > ${ASM_OUTPUT}
    COMMENT "Dumping assembly to ${ASM_OUTPUT}"
)

message("-- OpenCV: ${OpenCV_LIBS}")

project(rvv-test)

set(CMAKE_CXX_STANDARD 17)

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
endif()

add_definitions(-DEIGEN_USE_BLAS=1)
add_subdirectory(${CMAKE_SOURCE_DIR}/OpenBLAS)
add_subdirectory(${CMAKE_SOURCE_DIR}/eigen)
add_subdirectory(${CMAKE_SOURCE_DIR}/boost)

add_executable(rvvTest main.cpp)

target_compile_definitions(rvvTest PRIVATE EIGEN_USE_BLAS)
target_link_libraries(rvvTest PUBLIC
    openblas_static
    Eigen3::Eigen

    # Boost::date_time
    # Boost::filesystem
    # Boost::system
    # Boost::thread
    # Boost::math

    # ${OpenCV_LIBS}
)

set(ASM_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/asm.txt")

# Add a post-build custom command
add_custom_command(TARGET rvvTest POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:rvvTest> > ${ASM_OUTPUT}
    COMMENT "Dumping assembly to ${ASM_OUTPUT}"
)

message("-- OpenCV: ${OpenCV_LIBS}")
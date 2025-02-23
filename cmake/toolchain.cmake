set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

# /home/zekailin00/Desktop/riscv-gnu-toolchain/drops/linux
# /home/zekailin00/Desktop/riscv-gnu-toolchain/drops/linux
# /home/zekailin00/Desktop/riscv-gnu-toolchain/drops/newlib
# /home/zekailin00/Desktop/chipyard/.conda-env/riscv-tools
# /home/zekailin00/Desktop/rvv-test/Xuantie-900-gcc-linux-5.10.4-glibc-x86_64-V2.8.0
set(RISCV_TOOLCHAIN_ROOT "/home/zekailin00/Desktop/riscv-gnu-toolchain/drops/linux" CACHE PATH "Path to GCC for RISC-V cross compiler build directory")
set(CMAKE_SYSROOT "${RISCV_TOOLCHAIN_ROOT}/sysroot" CACHE PATH "RISC-V sysroot")
set(TARGET "RISCV64_ZVL256B")
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/drops/${TARGET}/" CACHE PATH "Install directory" FORCE)
set(NOFORTRAN "1")
set(DYNAMIC_ARCH ON)
set(EMBEDDED 1)


set(CMAKE_C_COMPILER ${RISCV_TOOLCHAIN_ROOT}/bin/riscv64-unknown-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${RISCV_TOOLCHAIN_ROOT}/bin/riscv64-unknown-linux-gnu-g++)


set(CMAKE_C_FLAGS "--sysroot=${CMAKE_SYSROOT} -march=rv64gcv -mabi=lp64d" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "--sysroot=${CMAKE_SYSROOT} -march=rv64gcv -mabi=lp64d" CACHE STRING "" FORCE)


set(PKG_CONFIG_EXECUTABLE "NOT-FOUND" CACHE PATH "Path to RISC-V pkg-config")

set(CMAKE_SHARED_LINKER_FLAGS_INIT "-L${CMAKE_SYSROOT}/lib")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-L${CMAKE_SYSROOT}/lib")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-L${CMAKE_SYSROOT}/lib")

set(CMAKE_C_STANDARD_LIBRARIES_INIT "-latomic" CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD_LIBRARIES_INIT "-latomic" CACHE STRING "" FORCE)

# export RISCV=/home/zekailin00/Desktop/riscv-gnu-toolchain/drops/linux
# export PATH=$PATH:/home/zekailin00/Desktop/riscv-gnu-toolchain/drops/linux/bin
# cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.cmake .. > log.txt
# make -j$(nproc) VERBOSE=1 > log2.txt
# cmake --install .
# qemu-riscv64 -L $RISCV/sysroot ./main
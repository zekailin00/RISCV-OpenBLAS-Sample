set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

###### Project setup ######
# Clone and initialize chipyard
# Upgrade RISC GNU compiler from 13 to 14 `mamba update riscv-tools`
# Upgrade Ubuntu from 18 to 24
# --`apt-get update`, `sudo apt update`, `sudo apt upgrade`, `do-release-upgrade`
# -- Keep running those commands until the version goes from 18 to 24
# -- OR use an ubuntu 24 image and replace the one in `chipyard/software/firemarshal/images/firechip/ubuntu-base/ubuntu-base.img`
# Clone riscv-gnu-toolchain and build qemu-system-riscv64;
# -- replacing version 5.0.0 from chipyard with version 9.2.0 from the repo in firemarshal json
# For firemarshal simulation: `"qemu-args": "-cpu rv64,v=true,vext_spec=v1.0",` to enable RVV
# -- the flags only works for newer versions of qemu


# /home/zekailin00/Desktop/riscv-gnu-toolchain/drops/linux
# /home/zekailin00/Desktop/chipyard/.conda-env/riscv-tools
# /home/zekailin00/Desktop/chipyard-t/.conda-env/riscv-tools

set(RISCV_TOOLCHAIN_ROOT "/home/zekailin00/Desktop/chipyard/.conda-env/riscv-tools" CACHE PATH "Path to GCC for RISC-V cross compiler build directory")
set(CMAKE_SYSROOT "${RISCV_TOOLCHAIN_ROOT}/sysroot" CACHE PATH "RISC-V sysroot")
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/drops/" CACHE PATH "Install directory" FORCE)


set(CMAKE_C_COMPILER ${RISCV_TOOLCHAIN_ROOT}/bin/riscv64-unknown-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${RISCV_TOOLCHAIN_ROOT}/bin/riscv64-unknown-linux-gnu-g++)

set(CMAKE_C_FLAGS "--sysroot=${CMAKE_SYSROOT} -fpermissive -march=rv64gcv -mabi=lp64d " CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "--sysroot=${CMAKE_SYSROOT} -fpermissive -march=rv64gcv -mabi=lp64d " CACHE STRING "" FORCE)

set(PKG_CONFIG_EXECUTABLE "NOT-FOUND" CACHE PATH "Path to RISC-V pkg-config")

set(CMAKE_SHARED_LINKER_FLAGS_INIT "-L${CMAKE_SYSROOT}/lib")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-L${CMAKE_SYSROOT}/lib")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-L${CMAKE_SYSROOT}/lib")

set(CMAKE_C_STANDARD_LIBRARIES_INIT "-latomic" CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD_LIBRARIES_INIT "-latomic" CACHE STRING "" FORCE)


# export RISCV=/home/zekailin00/Desktop/chipyard-t/.conda-env/riscv-tools
# export PATH=$PATH:/home/zekailin00/Desktop/chipyard-t/.conda-env/riscv-tools/bin
# export PATH=$PATH:/home/zekailin00/Desktop/riscv-gnu-toolchain/drops/linux/bin # with user-level qemu

############### opencv (Linked as prebuilt static libraries) ###############
# export PATH=$PATH:/home/zekailin00/Desktop/chipyard-t/.conda-env/riscv-tools/bin
# cmake -GNinja    -DCMAKE_BUILD_TYPE=Release    -DBUILD_SHARED_LIBS=OFF    -DWITH_OPENCL=OFF    -DCMAKE_TOOLCHAIN_FILE=../opencv/platforms/linux/riscv64-gcc.toolchain.cmake  -DOPENCV_EXTRA_MODULES_PATH=/home/zekailin00/Desktop/opencv_contrib/modules  -DRISCV_RVV_SCALABLE=ON  -DCMAKE_INSTALL_PREFIX=./drops  ..
# ninja -j$(nproc)
# cmake --install .
#############################################################################

# cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.cmake .. > log.txt
# make -j$(nproc) run_illixr_msckf VERBOSE=1 > log2.txt
# INFO: some libs from boost doesn't build with RISCV gcc, so only target run_illixr_msckf
# cmake --install .
# qemu-riscv64 -L $RISCV/sysroot ./main
# qemu-riscv64 -L $RISCV/sysroot -g 6969 build/ov_msckf/run_illixr_msckf data/mav0/cam0/data.csv data/mav0/cam1/data.csv data/mav0/imu0/data.csv data/mav0/cam0/data data/mav0/cam1/data
# qemu-riscv64 -L $RISCV/sysroot build-riscv/ov_msckf/run_illixr_msckf data/mav0/cam0/data.csv data/mav0/cam1/data.csv data/mav0/imu0/data.csv data/mav0/cam0/data data/mav0/cam1/data
# riscv64-unknown-linux-gnu-gdb build/ov_msckf/run_illixr_msckf

####### gdb debug commands 3######
# target remote localhost:6969
# disas
# break <file name>:<line number>
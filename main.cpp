#ifdef CMAKE_CROSSCOMPILING
    #include "hardware.h"
    #include <riscv_vector.h>
    void add_float_vectors(const float* a, const float* b, float* out, size_t n) {
        size_t i = 0;
        while (i < n) {
            size_t vl = __riscv_vsetvl_e32m1(n - i);                      // Set VL for 32-bit float, LMUL=1
            vfloat32m1_t va = __riscv_vle32_v_f32m1(&a[i], vl);           // Load a[i:i+vl]
            vfloat32m1_t vb = __riscv_vle32_v_f32m1(&b[i], vl);           // Load b[i:i+vl]
            vfloat32m1_t vsum = __riscv_vfadd_vv_f32m1(va, vb, vl);       // Add va + vb
            __riscv_vse32_v_f32m1(&out[i], vsum, vl);                     // Store result to out[i:i+vl]
            i += vl;
        }
    }
#endif

#include <Eigen/Dense>
#include <iostream>

int gemm_example() {
    // Define two 3x3 double precision matrices
    Eigen::MatrixXd A(3, 3);
    Eigen::MatrixXd B(3, 3);

    // Initialize matrix A
    A << 1.0, 2.0, 3.0,
         4.0, 5.0, 6.0,
         7.0, 8.0, 9.0;

    // Initialize matrix B
    B << 9.0, 8.0, 7.0,
         6.0, 5.0, 4.0,
         3.0, 2.0, 1.0;

    // Perform matrix multiplication: C = A * B
    Eigen::MatrixXd C = A * B;

    // Output the matrices
    std::cout << "Matrix A:\n" << A << "\n\n";
    std::cout << "Matrix B:\n" << B << "\n\n";
    std::cout << "Product of A and B (C = A * B):\n" << C << "\n";

    return 0;
}

void float_vecadd()
{
    float a[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    float b[5] = {0.1, 0.2, 0.3, 0.4, 0.5};
    float c[5] = {1.1, 2.2, 3.3, 4.4, 5.5};

    // add_float_vectors(a, b, c, 5);
    std::cout << "\nfloat vecadd:" << std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;
}

int main()
{
    __asm__ volatile ("addi x0, x1, 0");
    gemm_example();
    float_vecadd();

    // std::cout << cv::getBuildInformation().c_str() << std::endl;
    __asm__ volatile ("addi x0, x2, 0");
    return 0;
}

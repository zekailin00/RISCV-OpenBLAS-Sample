

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#pragma message "Macro value: " TOSTRING(__riscv_v_intrinsic)

#include <cblas.h>
#include <riscv_vector.h>
#include <iostream>
#include <omp.h>

int main() {
    __rvv_float32m4x2_t var;
    __riscv_vget_v_f32m4x2_f32m4(var, 0);

    std::cout << "hello" << std::endl;

    #pragma omp parallel
    {
        #pragma omp critical
        std::cout << "thread: " << omp_get_thread_num() << std::endl;
    }
    
    return 0;
}

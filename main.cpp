#ifdef CMAKE_CROSSCOMPILING
    #include "hardware.h"
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
#include <string>

static inline uint64_t read_cycles() {
    uint64_t cycles;
#ifdef CMAKE_CROSSCOMPILING
    asm volatile("rdcycle %0" : "=r" (cycles));
#else
    asm volatile("rdtsc" : "=A" (cycles));
#endif
    return cycles;
}


inline void gemv_n(const Eigen::MatrixXf& A, 
    const Eigen::VectorXf& x, Eigen::VectorXf& y, double alpha, double beta)
{
    y = alpha * A * x + beta * y;
}

inline void gemv_t(const Eigen::MatrixXf& A,
    const Eigen::VectorXf& x, Eigen::VectorXf& y, double alpha, double beta)
{
    y = alpha * A.transpose() * x + beta * y;
}

inline void gemv_n(const Eigen::MatrixXd& A, 
    const Eigen::VectorXd& x, Eigen::VectorXd& y, double alpha, double beta)
{
    y = alpha * A * x + beta * y;
}

inline void gemv_t(const Eigen::MatrixXd& A,
    const Eigen::VectorXd& x, Eigen::VectorXd& y, double alpha, double beta)
{
    y = alpha * A.transpose() * x + beta * y;
}

int gemm_example(int matSize = 50) {
    // Define 20x20 dynamically sized matrices
    Eigen::MatrixXd A(matSize, matSize);
    Eigen::MatrixXd B(matSize, matSize);
    Eigen::MatrixXd C(matSize, matSize);
    Eigen::MatrixXd T(matSize, matSize);
    Eigen::MatrixXd Sinv = Eigen::MatrixXd::Identity(matSize, matSize);
    Eigen::VectorXd X(matSize);
    Eigen::VectorXd Y(matSize);

    // Initialize A and B with some values (e.g., using sequential values)
    // Eigen::internal::setRandomSeed(42);
    srand(42);
    A.setRandom(); // Or use setZero(), setOnes(), etc. for specific patterns
    B.setRandom();
    C.setRandom();
    X.setRandom();
    Y.setRandom();

    // DGEMM operation: C = alpha * A * B + beta * C
    double alpha = 2.2;
    double beta = 0.6;

    C = alpha * A * B + beta * C;

    // Print the result
    std::cout << "Result of C = alpha * A * B + beta * C:\n" << C << std::endl;

    // Output the matrices
    std::cout << "Matrix A:\n" << A << "\n\n";
    std::cout << "Matrix B:\n" << B << "\n\n";
    std::cout << "Product of A and B (C = A * B):\n" << C << "\n";

    C = A.selfadjointView<Eigen::Lower>() * B;
    std::cout << "Result C:\n" << C << "\n";
    C = A * B.triangularView<Eigen::Upper>();
    std::cout << "Result C:\n" << C << "\n";
    C = A.selfadjointView<Eigen::Lower>().rankUpdate(B, 1.0);
    std::cout << "Result C:\n" << C << "\n";

    // gemv_n(A, X, Y, alpha, beta);  // y = alpha * A * x + beta * y
    std::cout << "Result y:\n" << Y << std::endl;
    // gemv_t(A, X, Y, alpha, beta);  // y = alpha * A * x + beta * y
    std::cout << "Result y:\n" << Y << std::endl;

    A <<
        1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
        0, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        0,  0, 21, 22, 23, 24, 25, 26, 27, 28,
        0,  0,  0, 31, 32, 33, 34, 35, 36, 37,
        0,  0,  0,  0, 41, 42, 43, 44, 45, 46,
        0,  0,  0,  0,  0, 51, 52, 53, 54, 55,
        0,  0,  0,  0,  0,  0, 61, 62, 63, 64,
        0,  0,  0,  0,  0,  0,  0, 71, 72, 73,
        0,  0,  0,  0,  0,  0,  0,  0, 81, 82,
        0,  0,  0,  0,  0,  0,  0,  0,  0, 91;

    // Eigen::LLT<Eigen::MatrixXd, 2> llt = A.selfadjointView<Eigen::Upper>().llt();
    // std::cout << "\nllt" << llt.matrixLLT() << std::endl;
    // llt.solveInPlace(Sinv);
    // std::cout << "Result sinv:\n" << Sinv << std::endl;

    return 0;
}

int gemm_example2(int matSize = 50) {
    // Define 20x20 dynamically sized matrices
    Eigen::MatrixXd A(matSize, matSize);
    Eigen::MatrixXd B(matSize, matSize);
    Eigen::MatrixXd C(matSize, matSize);
    Eigen::MatrixXd T(matSize, matSize);
    Eigen::MatrixXd Sinv = Eigen::MatrixXd::Identity(matSize, matSize);
    Eigen::VectorXd X(matSize);
    Eigen::VectorXd Y(matSize);

    // Initialize A and B with some values (e.g., using sequential values)
    // Eigen::internal::setRandomSeed(42);
    srand(42);
    A.setRandom(); // Or use setZero(), setOnes(), etc. for specific patterns
    B.setRandom();
    C.setRandom();
    X.setRandom();
    Y.setRandom();

    // DGEMM operation: C = alpha * A * B + beta * C
    double alpha = 2.2;
    double beta = 0.6;

    C = alpha * A * B + beta * C;

    // A <<
    //     1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
    //     0, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    //     0,  0, 21, 22, 23, 24, 25, 26, 27, 28,
    //     0,  0,  0, 31, 32, 33, 34, 35, 36, 37,
    //     0,  0,  0,  0, 41, 42, 43, 44, 45, 46,
    //     0,  0,  0,  0,  0, 51, 52, 53, 54, 55,
    //     0,  0,  0,  0,  0,  0, 61, 62, 63, 64,
    //     0,  0,  0,  0,  0,  0,  0, 71, 72, 73,
    //     0,  0,  0,  0,  0,  0,  0,  0, 81, 82,
    //     0,  0,  0,  0,  0,  0,  0,  0,  0, 91;
    
    A <<
        1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  2,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  3,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  4,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  5,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  6,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  7,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  8,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  9,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0, 10;

    Eigen::MatrixXd symA = A.selfadjointView<Eigen::Upper>();

    std::cout << "selfadjointView: \n" << symA << std::endl;

    Eigen::LLT<Eigen::MatrixXd, 2> llt = A.selfadjointView<Eigen::Upper>().llt();
    std::cout << "\nllt" << llt.matrixLLT() << std::endl;
    
    llt.solveInPlace(Sinv);
    std::cout << "Result sinv:\n" << Sinv << std::endl;

    return 0;
}

int gemm_benchmark(int matSize = 50, int seed = 0, int iteration = 10) {
    // Define 20x20 dynamically sized matrices
    Eigen::MatrixXd A(matSize, matSize);
    Eigen::MatrixXd B(matSize, matSize);
    Eigen::MatrixXd C(matSize, matSize);

    // Initialize A and B with some values (e.g., using sequential values)
    // Eigen::internal::setRandomSeed(42);
    srand(seed);
    A.setRandom(); // Or use setZero(), setOnes(), etc. for specific patterns
    B.setRandom();
    C.setRandom();

    // DGEMM operation: C = alpha * A * B + beta * C
    double alpha = 2.2;
    double beta = 0.6;

    // Print the result
    std::cout << "\nmatSize: " << matSize 
              << ", seed: " << seed 
              << ", iteration: " << iteration
              << std::endl;
    // std::cout << "\nResult of C = alpha * A * B + beta * C:\n" << C << std::endl;

    int M = A.rows();
    int K = A.cols();
    int N = B.cols();

    // Leading dimensions (for column-major matrices)
    int lda = M;
    int ldb = K;
    int ldc = M;

    volatile uint64_t start, end;
    uint64_t count[iteration];
    for (int i = 0; i < iteration; i++)
    {
        start = read_cycles();
        // std::cout << "\n A:\n" << A << std::endl;
        // std::cout << "\n B:\n" << B << std::endl;
        C = alpha * A * B + beta * C;
        // Get dimensions dynamically
        // cblas_dgemm(
        //     CblasColMajor, CblasNoTrans, CblasNoTrans,
        //     M, N, K,
        //     alpha,
        //     A.data(), lda,
        //     B.data(), ldb,
        //     beta,
        //     C.data(), ldc
        // );
        end = read_cycles();
        count[i] = end - start;
        // std::cout << "\nResult of C = alpha * A * B + beta * C:\n" << C << std::endl;
        // break;
    }
    uint64_t average = 0;
    std::cout << "C = alpha * A * B + beta * C\n";
    for (int i = 0; i < iteration; i++) {
        std::cout << count[i] << " ";
        average += count[i];
    }
    std::cout << "Average: " << average / iteration << std::endl;

    return 0;
}

int gemv_benchmark(int argc, char* argv[])
{
    int length      = std::stoi(argv[2]);
    int alpha       = std::stoi(argv[3]);
    int beta        = std::stoi(argv[4]);
    int seed        = std::stoi(argv[5]);
    int iteration   = std::stoi(argv[6]);

    printf("length    : %d\n", length );
    printf("alpha     : %d\n", alpha );
    printf("beta      : %d\n", beta      );
    printf("seed      : %d\n", seed      );
    printf("iteration : %d\n", iteration );

    Eigen::MatrixXf A(length, length);
    Eigen::VectorXf X(length);
    Eigen::VectorXf Y(length);

    srand(seed);
    A.setRandom(); 
    X.setRandom();
    Y.setRandom();

    volatile uint64_t start, end, average;
    uint64_t count[iteration];
    for (int i = 0; i < iteration; i++)
    {
        start = read_cycles();
        gemv_n(A, X, Y, alpha, beta);
        end = read_cycles();
        count[i] = end - start;
    }
    average = 0;
    std::cout << "gemv_n(A, X, Y, alpha, beta);\n";
    for (int i = 0; i < iteration; i++) {
        std::cout << count[i] << " ";
        average += count[i];
    }
    std::cout << "Average: " << average / iteration << std::endl;

    for (int i = 0; i < iteration; i++)
    {
        start = read_cycles();
        gemv_t(A, X, Y, alpha, beta);
        end = read_cycles();
        count[i] = end - start;
    }
    average = 0;
    std::cout << "gemv_t(A, X, Y, alpha, beta);\n";
    for (int i = 0; i < iteration; i++) {
        std::cout << count[i] << " ";
        average += count[i];
    }
    std::cout << "Average: " << average / iteration << std::endl;

    return 0;
}

int gemvTest(int argc, char* argv[])
{
    int length      = std::stoi(argv[2]);
    int alpha       = std::stoi(argv[3]);
    int beta        = std::stoi(argv[4]);
    int seed        = std::stoi(argv[5]);

    Eigen::MatrixXf A(length, length);
    Eigen::VectorXf X(length);
    Eigen::VectorXf Y(length);

    srand(seed);
    A.setRandom(); 
    X.setRandom();
    Y.setRandom();

    std::cout << "A:\n" << A << std::endl;
    std::cout << "X:\n" << X.transpose() << std::endl;
    std::cout << "Y:\n" << Y.transpose() << std::endl;

    gemv_n(A, X, Y, alpha, beta);  // y = alpha * A * x + beta * y
    std::cout << "gemv_n(A, X, Y, alpha, beta):\n" << Y.transpose() << std::endl;
    gemv_t(A, X, Y, alpha, beta);  // y = alpha * A * x + beta * y
    std::cout << "gemv_t(A, X, Y, alpha, beta):\n" << Y.transpose() << std::endl;

    return 0;
}

int gemm_benchmark2(int argc, char* argv[])
{
    int MAT_DIM_I   = std::stoi(argv[2]);
    int MAT_DIM_J   = std::stoi(argv[3]);
    int MAT_DIM_K   = std::stoi(argv[4]);
    int alpha       = std::stoi(argv[5]);
    int beta        = std::stoi(argv[6]);
    int transA      = std::stoi(argv[7]);
    int transB      = std::stoi(argv[8]);
    int seed        = std::stoi(argv[9]);
    int iteration   = std::stoi(argv[10]);

    printf("MAT_DIM_I : %d\n", MAT_DIM_I );
    printf("MAT_DIM_J : %d\n", MAT_DIM_J );
    printf("MAT_DIM_K : %d\n", MAT_DIM_K );
    printf("alpha     : %d\n", alpha     );
    printf("beta      : %d\n", beta      );
    printf("transA    : %d\n", transA    );
    printf("transB    : %d\n", transB    );
    printf("seed      : %d\n", seed      );
    printf("iteration : %d\n", iteration );
    
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> full_A;
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> full_B;
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> full_C(MAT_DIM_I, MAT_DIM_J);

    if (transA != 0)
        full_A.resize(MAT_DIM_K, MAT_DIM_I);
    else
        full_A.resize(MAT_DIM_I, MAT_DIM_K);

    if (transB != 0)
        full_B.resize(MAT_DIM_J, MAT_DIM_K);
    else
        full_B.resize(MAT_DIM_K, MAT_DIM_J);

    srand(seed);
    full_A.setRandom();
    full_B.setRandom();
    full_C.setRandom();

    volatile uint64_t start, end;
    uint64_t count[iteration];
    for (int i = 0; i < iteration; i++)
    {
        start = read_cycles();
        full_C = alpha * full_A * full_B + beta * full_C;
        end = read_cycles();
        count[i] = end - start;
    }
    uint64_t average = 0;
    std::cout << "C = alpha * A * B + beta * C\n";
    for (int i = 0; i < iteration; i++) {
        std::cout << count[i] << " ";
        average += count[i];
    }
    std::cout << "Average: " << average / iteration << std::endl;

    return 0;
}

void matTest(int argc, char* argv[])
{
    int MAT_DIM_I   = std::stoi(argv[2]);
    int MAT_DIM_J   = std::stoi(argv[3]);
    int MAT_DIM_K   = std::stoi(argv[4]);
    int alpha       = std::stoi(argv[5]);
    int beta        = std::stoi(argv[6]);
    int transA      = std::stoi(argv[7]);
    int transB      = std::stoi(argv[8]);
    int seed        = std::stoi(argv[9]);

    printf("MAT_DIM_I : %d\n", MAT_DIM_I );
    printf("MAT_DIM_J : %d\n", MAT_DIM_J );
    printf("MAT_DIM_K : %d\n", MAT_DIM_K );
    printf("alpha     : %d\n", alpha     );
    printf("beta      : %d\n", beta      );
    printf("transA    : %d\n", transA    );
    printf("transB    : %d\n", transB    );
    printf("seed      : %d\n", seed      );

    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> full_A;
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> full_B;
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> full_C(MAT_DIM_I, MAT_DIM_J);

    if (transA != 0)
        full_A.resize(MAT_DIM_K, MAT_DIM_I);
    else
        full_A.resize(MAT_DIM_I, MAT_DIM_K);

    if (transB != 0)
        full_B.resize(MAT_DIM_J, MAT_DIM_K);
    else
        full_B.resize(MAT_DIM_K, MAT_DIM_J);

    srand(seed);
    full_A.setRandom();
    full_B.setRandom();
    full_C.setRandom();

    std::cout << "A: \n" << full_A << std::endl;
    std::cout << "B: \n" << full_B << std::endl;
    std::cout << "C: \n" << full_C << std::endl;

    std::cout << "C = A * B + D: \n" <<
        alpha *
        ((transA != 0)? full_A.transpose(): full_A) *
        ((transB != 0)? full_B.transpose(): full_B) + 
        beta * full_C 
        << std::endl;
}

int main(int argc, char* argv[])
{
    // // __asm__ volatile ("addi x0, x1, 0");
    // std::cout <<"USE BLAS " << EIGEN_USE_BLAS << std::endl;

    // if (argc < 4) {
    //     std::cerr << "Usage: " << argv[0] << " <int1> <int2>\n";
    //     return 1;
    // }

#ifdef CMAKE_CROSSCOMPILING
    printHardware();
    printVlenb();
#endif

    // // gemm_example2(10);
    // gemm_example(10);
    // gemm_benchmark(std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]));

    if (std::stoi(argv[1]) == 0)
        gemvTest(argc, argv);
    else if (std::stoi(argv[1]) == 1)
        gemv_benchmark(argc, argv);
    else if (std::stoi(argv[1]) == 2)
        matTest(argc, argv);
    else if (std::stoi(argv[1]) == 3)
        gemm_benchmark2(argc, argv);

    // // std::cout << cv::getBuildInformation().c_str() << std::endl;
    // // __asm__ volatile ("addi x0, x2, 0");
    return 0;
}

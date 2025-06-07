// This program demonstrates basic usage of the Eigen library for linear algebra.
//
// ================================= IMPORTANT ==================================
// Eigen is a C++ template library for linear algebra: matrices, vectors,
// numerical solvers, and related algorithms. It is header-only.
//
// TO COMPILE THIS FILE:
// 1. Download Eigen: Go to https://eigen.tuxfamily.org and download the latest stable release.
// 2. Extract the archive (e.g., to /path/to/eigen-3.x.x/). The actual Eigen headers
//    will be inside a subdirectory, often named "Eigen" (e.g., /path/to/eigen-3.x.x/Eigen/).
// 3. Provide the path to the directory CONTAINING the "Eigen" folder to your compiler
//    using the -I flag. For example, if Eigen's "Eigen" folder is at
//    "/usr/local/include/eigen-3.4.0/Eigen/", you would use:
//    g++ 01-eigen-basics.cpp -I /usr/local/include/eigen-3.4.0 -o 01-eigen-basics
//    (Adjust the path according to where you extracted Eigen).
//
//    Alternatively, you can place the Eigen folder (the one containing Dense, Core, etc.)
//    into a standard include path, or a project-specific include directory.
// ==============================================================================

#include <iostream>
// Include the core Eigen headers. <Eigen/Dense> includes most commonly used features.
#include <Eigen/Dense> // Includes Core, Geometry, LU, Cholesky, SVD, QR, and more.

int main() {
    std::cout << "Eigen Version: " << EIGEN_WORLD_VERSION << "."
              << EIGEN_MAJOR_VERSION << "." << EIGEN_MINOR_VERSION << std::endl;

    // --- 1. Matrix Operations ---
    std::cout << "\n--- 1. Matrix Operations ---" << std::endl;

    // a) Declare matrices
    // Eigen::Matrix<Scalar, RowsAtCompileTime, ColsAtCompileTime, Options, MaxRowsAtCompileTime, MaxColsAtCompileTime>
    // Common typedefs:
    //   MatrixXd: Matrix of doubles, dynamic size. ('X' means dynamic, 'd' means double)
    //   MatrixXf: Matrix of floats, dynamic size.
    //   Matrix3f: 3x3 matrix of floats.
    //   Matrix4d: 4x4 matrix of doubles.

    Eigen::MatrixXd m_dynamic(2, 3); // A 2x3 matrix of doubles, size determined at runtime.
    Eigen::Matrix3f m3f;             // A 3x3 matrix of floats, size fixed at compile time.
    Eigen::Matrix<double, 2, 2> m22d; // A 2x2 matrix of doubles.

    // b) Initialize matrices
    std::cout << "\n--- Initialization ---" << std::endl;
    m_dynamic.setZero(); // Fill with zeros
    std::cout << "m_dynamic (setZero):\n" << m_dynamic << std::endl;

    m3f = Eigen::Matrix3f::Ones(); // Fill with ones
    std::cout << "m3f (Ones):\n" << m3f << std::endl;

    m22d = Eigen::Matrix2d::Identity(); // Identity matrix (1s on diagonal, 0s elsewhere)
    std::cout << "m22d (Identity):\n" << m22d << std::endl;

    Eigen::MatrixXf m_rand = Eigen::MatrixXf::Random(3,4); // Random values between -1 and 1
    std::cout << "m_rand (Random 3x4):\n" << m_rand << std::endl;


    // Comma initializer (convenient for small fixed-size matrices)
    m22d << 1.0, 2.0,  // Row 0
           3.0, 4.0;  // Row 1
    std::cout << "m22d (comma initialized):\n" << m22d << std::endl;

    Eigen::Matrix3d m3d_init;
    m3d_init << 1, 2, 3,
                4, 5, 6,
                7, 8, 9;
    std::cout << "m3d_init (3x3 comma initialized):\n" << m3d_init << std::endl;

    // c) Accessing elements (0-indexed)
    std::cout << "\n--- Accessing Elements ---" << std::endl;
    std::cout << "m22d(0, 1): " << m22d(0, 1) << std::endl; // Access element at row 0, col 1
    m22d(1, 0) = 10.5;
    std::cout << "m22d (after m22d(1,0) = 10.5):\n" << m22d << std::endl;

    // d) Basic arithmetic
    std::cout << "\n--- Basic Arithmetic ---" << std::endl;
    Eigen::Matrix2d m_a, m_b, m_res_2d;
    m_a << 1, 2, 3, 4;
    m_b << 5, 6, 7, 8;

    m_res_2d = m_a + m_b; // Matrix addition
    std::cout << "Matrix m_a + m_b:\n" << m_res_2d << std::endl;

    m_res_2d = m_a - m_b; // Matrix subtraction
    std::cout << "Matrix m_a - m_b:\n" << m_res_2d << std::endl;

    m_res_2d = m_a * 2.5; // Scalar multiplication
    std::cout << "Matrix m_a * 2.5:\n" << m_res_2d << std::endl;

    // Coefficient-wise operations (element by element)
    Eigen::Matrix2d m_cwise_prod = m_a.array() * m_b.array(); // Element-wise product
    std::cout << "Element-wise product (m_a.array() * m_b.array()):\n" << m_cwise_prod << std::endl;
    Eigen::Matrix2d m_cwise_div = m_a.array() / m_b.array();   // Element-wise division
    std::cout << "Element-wise division (m_a.array() / m_b.array()):\n" << m_cwise_div << std::endl;


    // e) Matrix multiplication
    std::cout << "\n--- Matrix Multiplication ---" << std::endl;
    Eigen::MatrixXd m_c(2, 3);
    m_c << 1, 2, 3,
           4, 5, 6;
    Eigen::MatrixXd m_d(3, 2);
    m_d << 7, 8,
           9, 1,
           2, 3;
    Eigen::MatrixXd m_res_mult = m_c * m_d; // 2x3 * 3x2 = 2x2 matrix
    std::cout << "Matrix m_c * m_d (2x3 * 3x2):\n" << m_res_mult << std::endl;
    // Check dimensions: m_d * m_c would be 3x2 * 2x3 = 3x3
    Eigen::MatrixXd m_res_mult2 = m_d * m_c;
    std::cout << "Matrix m_d * m_c (3x2 * 2x3):\n" << m_res_mult2 << std::endl;


    // f) Transpose
    std::cout << "\n--- Transpose ---" << std::endl;
    std::cout << "Matrix m_c:\n" << m_c << std::endl;
    Eigen::MatrixXd m_c_T = m_c.transpose(); // Transpose of m_c (becomes 3x2)
    std::cout << "m_c.transpose():\n" << m_c_T << std::endl;
    // Note: m_c.transpose() returns a proxy object. Assign it to a matrix or use .eval()
    // m_c.transposeInPlace(); // Modifies m_c directly if it's a square matrix or dynamic with correct aspect ratio


    // --- 2. Vector Operations ---
    std::cout << "\n\n--- 2. Vector Operations ---" << std::endl;
    // Vectors are just matrices with one column (column vectors) or one row (row vectors).
    // Eigen::VectorXd: Dynamic size column vector of doubles.
    // Eigen::RowVectorXd: Dynamic size row vector of doubles.
    // Eigen::Vector3f: Fixed size (3x1) column vector of floats.

    // a) Declare and initialize vectors
    Eigen::VectorXd v_dynamic(3); // A column vector of 3 doubles.
    v_dynamic << 1.0, 2.5, -0.5;
    std::cout << "v_dynamic (column vector):\n" << v_dynamic << std::endl;

    Eigen::Vector3f v3f(1.0f, 2.0f, 3.0f); // Constructor initialization
    std::cout << "v3f (3x1 float vector):\n" << v3f << std::endl;

    Eigen::RowVector2d rv2d(5.0, 6.0); // A row vector (1x2)
    std::cout << "rv2d (row vector):\n" << rv2d << std::endl;

    // b) Basic arithmetic
    std::cout << "\n--- Vector Arithmetic ---" << std::endl;
    Eigen::Vector3d v_a(1,2,3), v_b(4,5,6), v_res_3d;

    v_res_3d = v_a + v_b;
    std::cout << "Vector v_a + v_b:\n" << v_res_3d << std::endl;

    v_res_3d = v_a - v_b;
    std::cout << "Vector v_a - v_b:\n" << v_res_3d << std::endl;

    v_res_3d = v_a * 3.0; // Scalar multiplication
    std::cout << "Vector v_a * 3.0:\n" << v_res_3d << std::endl;

    // c) Dot product and Cross product
    std::cout << "\n--- Dot and Cross Product ---" << std::endl;
    double dot_product = v_a.dot(v_b); // (1*4 + 2*5 + 3*6) = 4 + 10 + 18 = 32
    std::cout << "Dot product of v_a and v_b: " << dot_product << std::endl;

    // Cross product is only for 3D vectors
    Eigen::Vector3d cross_product = v_a.cross(v_b);
    // v_a = (1,2,3), v_b = (4,5,6)
    // cross_product = (2*6 - 3*5, 3*4 - 1*6, 1*5 - 2*4)
    //               = (12 - 15, 12 - 6, 5 - 8) = (-3, 6, -3)
    std::cout << "Cross product of v_a and v_b:\n" << cross_product << std::endl;

    // Other useful vector operations
    std::cout << "Norm (magnitude) of v_a: " << v_a.norm() << std::endl;
    Eigen::Vector3d v_normalized = v_a.normalized(); // Returns a unit vector
    std::cout << "Normalized v_a:\n" << v_normalized << std::endl;
    std::cout << "Norm of normalized v_a: " << v_normalized.norm() << std::endl;


    // --- 3. Matrix-Vector Multiplication ---
    std::cout << "\n\n--- 3. Matrix-Vector Multiplication ---" << std::endl;
    // m3d_init (3x3) * v_a (3x1) = 3x1 vector
    // m3d_init:
    // 1, 2, 3
    // 4, 5, 6
    // 7, 8, 9
    // v_a: (1,2,3)
    // Result:
    // (1*1 + 2*2 + 3*3) = 1+4+9 = 14
    // (4*1 + 5*2 + 6*3) = 4+10+18 = 32
    // (7*1 + 8*2 + 9*3) = 7+16+27 = 50
    Eigen::Vector3d mv_result = m3d_init * v_a;
    std::cout << "Matrix m3d_init * vector v_a:\n" << mv_result << std::endl;


    std::cout << "\nEigen basics demonstration complete." << std::endl;
    return 0;
}

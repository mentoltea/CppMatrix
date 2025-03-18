#include "gtest/gtest.h"
#define MATRIX_IMPL
#include "../src/matrix.hpp"
#include <iostream>

TEST(calculation, idenity_multiply) {
    std::string filename = "matrices/m2det7rank3.txt";
	std::ifstream fd(filename);

	Matrix<double> A(fd);
    Matrix<double> I = Matrix<double>::Idenity(A.a);

    EXPECT_TRUE(A*I == A);
    EXPECT_TRUE(I*A == A);
}

TEST(calculation, idenity_det) {
    Matrix<double> I = Matrix<double>::Idenity(3);
    double eps = 1e-5;

    EXPECT_NEAR(I.det(), 1, eps);
}

TEST(calculation, idenity_rank) {
    Matrix<double> I = Matrix<double>::Idenity(3);
    double eps = 1e-5;
    
    EXPECT_EQ(I.rank(), I.a);
}

TEST(calculation, det_0) {
    std::string filename = "matrices/m1det0rank2.txt";
	std::ifstream fd(filename);
    Matrix<double> A(fd);
    double eps = 1e-5;
    
    EXPECT_NEAR(A.det(), 0, eps);
}

TEST(calculation, det_7) {
    std::string filename = "matrices/m2det7rank3.txt";
	std::ifstream fd(filename);
    Matrix<double> A(fd);
    double eps = 1e-5;
    
    EXPECT_NEAR(A.det(), 7, eps);
}

TEST(calculation, rank_2) {
    std::string filename = "matrices/m1det0rank2.txt";
	std::ifstream fd(filename);
    Matrix<double> A(fd);
    double eps = 1e-5;
    
    EXPECT_TRUE(A.rank() == 2);
}

TEST(calculation, rank_3) {
    std::string filename = "matrices/m2det7rank3.txt";
	std::ifstream fd(filename);
    Matrix<double> A(fd);
    double eps = 1e-5;
    
    EXPECT_TRUE(A.rank() == 3);
}

TEST(calculation, constant_multiplication) {
    double eps = 1e-5;
    Matrix<double> I = Matrix<double>::Idenity(3);

    EXPECT_TRUE( (I+I+I) == 3.0*I);
}

TEST(calculation, det_transcipt) {
    std::string filename = "matrices/m2det7rank3.txt";
	std::ifstream fd(filename);
    Matrix<double> A(fd);
    double eps = 1e-5;
    
    EXPECT_NEAR(A.det(), A.trans().det(), eps);
}


TEST(calculation, inverse) {
    std::string filename = "matrices/m2det7rank3.txt";
	std::ifstream fd(filename);
    Matrix<double> A(fd);
    double eps = 1e-5;

    Matrix<double> inv = A.inverse();
    Matrix<double> I = Matrix<double>::Idenity(A.a);

    EXPECT_TRUE(A*inv ==  I);
    EXPECT_TRUE(inv*A ==  I);
} 

TEST(calculation, det_multiplication) {
    double eps = 1e-5;

    std::string filename1 = "matrices/m2det7rank3.txt";
	std::ifstream fd1(filename1);
    Matrix<double> A(fd1);

    std::string filename2 = "matrices/m3det-4rank3.txt";
	std::ifstream fd2(filename2);
    Matrix<double> B(fd2);

    EXPECT_TRUE((A*B).det() == A.det()*B.det());
    EXPECT_TRUE((B*A).det() == A.det()*B.det());
}

TEST(calculation, power_i) {
    double eps = 1e-5;
    Matrix<double> A(2, 2);
    A[1][0] = -1;
    A[0][1] = 1;

    Matrix<double> I = Matrix<double>::Idenity(2);

    EXPECT_TRUE( (A^2) == -I);
    EXPECT_TRUE( ((A^2)^2) == I);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
#include "gtest/gtest.h"
#define MATRIX_IMPL
#include "matrix.hpp"
#include <iostream>

TEST(matrix_creation, from_sizes_0_int) {
    int a = 5;
	int b = 5;
	int c = 0;
	{
		Matrix<int> M(a, b, c);
		ASSERT_EQ(M.a, a);
		ASSERT_EQ(M.b, b);
		for (int y=0; y<a; y++) {
			for (int x=0; x<b; x++) {
				EXPECT_EQ(c,  M[y][x]);
			}
		}
	}
}

TEST(matrix_creation, from_sizes_0_double) {
    int a = 5;
	int b = 5;
	double c = 0;
	{
		Matrix<double> M(a, b, c);
		ASSERT_EQ(M.a, a);
		ASSERT_EQ(M.b, b);
		for (int y=0; y<a; y++) {
			for (int x=0; x<b; x++) {
				EXPECT_DOUBLE_EQ(c,  M[y][x]);
			}
		}
	}
}

TEST(matrix_creation, from_sizes_2_int) {
    int a = 5;
	int b = 5;
	int c = 2;
	{
		Matrix<int> M(a, b, c);
		ASSERT_EQ(M.a, a);
		ASSERT_EQ(M.b, b);
		for (int y=0; y<a; y++) {
			for (int x=0; x<b; x++) {
				EXPECT_EQ(c,  M[y][x]);
			}
		}
	}
}

TEST(matrix_creation, from_sizes_1_5_double) {
    int a = 5;
	int b = 5;
	double c = 1.5;
	{
		Matrix<double> M(a, b, c);
		ASSERT_EQ(M.a, a);
		ASSERT_EQ(M.b, b);
		for (int y=0; y<a; y++) {
			for (int x=0; x<b; x++) {
				EXPECT_DOUBLE_EQ(c,  M[y][x]);
			}
		}
	}
}

TEST(matrix_creation, from_file_id_4_4_int) {
    std::string filename = "matrices/idenity4_4.txt";
	std::ifstream fd(filename);
	{
		Matrix<int> M(fd);
		ASSERT_EQ(M.a, 4);
		ASSERT_EQ(M.b, 4);
		for (int y=0; y<M.a; y++) {
			for (int x=0; x<M.b; x++) {
				EXPECT_EQ(M[y][x], y==x);
			}
		}
	}
}

TEST(matrix_creation, from_file_id_4_4_double) {
    std::string filename = "matrices/idenity4_4.txt";
	std::ifstream fd(filename);
	double eps = 1e-5;
	{
		Matrix<double> M(fd);
		ASSERT_EQ(M.a, 4);
		ASSERT_EQ(M.b, 4);
		for (int y=0; y<M.a; y++) {
			for (int x=0; x<M.b; x++) {
				EXPECT_NEAR(M[y][x], y==x, eps);
			}
		}
	}
}

TEST(matrix_creation, with_0_rows) {
	int a = 0;
	int b = 1;
	try {
		Matrix<double> M(a, b);
		FAIL();
	} catch (std::runtime_error& e) {
		SUCCEED();
	}
}

TEST(matrix_creation, with_0_columns) {
	int a = 1;
	int b = 0;
	try {
		Matrix<double> M(a, b);
		FAIL();
	} catch (std::runtime_error& e) {
		SUCCEED();
	}
}

TEST(matrix_creation, from_initializer_list) {
	double eps = 1e-5;
	Matrix<double> M = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
		{10, 11, 12}
    };
	for (int y=0; y<M.a; y++) {
		for (int x=0; x<M.b; x++) {
			EXPECT_NEAR(M[y][x], 3*y+x+1, eps);
		}
	}
}

TEST(matrix_creation, from_initializer_list_0_size) {
	try {
		Matrix<double> M = {};
		FAIL();
	} catch (std::runtime_error& e) {
		try {
			Matrix<double> M = {
				{1, 2, 3},
				{}, 
				{7, 8, 9}
			};
			FAIL();
		} catch(std::runtime_error&) {
			SUCCEED();
		}
	}
}

TEST(matrix_creation, multiply_different_sizes) {
	Matrix<double> A(2, 3);
	Matrix<double> B(2, 2);
	try {
		B*A;
	} catch (std::runtime_error& e) {
		FAIL();
	}

	try {
		A*B;
		FAIL();
	} catch (std::runtime_error& e) {
		SUCCEED();
	}
}

TEST(matrix_creation, add_different_sizes) {
	Matrix<double> A(2, 3);
	Matrix<double> B(2, 2);
	try {
		A+B;
		FAIL();
	} catch (std::runtime_error& e) {
		try {
			A+=B;
			FAIL();
		} catch (std::runtime_error& e) {
			SUCCEED();
		}
	}
}

TEST(matrix_creation, subtract_different_sizes) {
	Matrix<double> A(2, 3);
	Matrix<double> B(2, 2);
	try {
		A-B;
		FAIL();
	} catch (std::runtime_error& e) {
		try {
			A-=B;
			FAIL();
		} catch (std::runtime_error& e) {
			SUCCEED();
		}
	}
}

TEST(matrix_creation, power_nonsquare) {
	Matrix<double> A(2, 3);
	try {
		A^3;
		FAIL();
	} catch (std::runtime_error& e) {
		SUCCEED();
	}
}

TEST(matrix_creation, det_nonsquare) {
	Matrix<double> A(2, 3);
	try {
		A.det();
		FAIL();
	} catch (std::runtime_error& e) {
		SUCCEED();
	}
}

TEST(matrix_creation, inverse_nonsquare) {
	Matrix<double> A(2, 3);
	try {
		A.inverse();
		FAIL();
	} catch (std::runtime_error& e) {
		SUCCEED();
	}
}

TEST(matrix_creation, inverse_0_det) {
	Matrix<double> A(2, 2);
	A[0][0] = 1;
	A[0][1] = 2;
	try {
		A.inverse();
		FAIL();
	} catch (std::runtime_error& e) {
		SUCCEED();
	}
}


TEST(matrix_creation, copy_constructor) {
	std::string filename = "matrices/m1det0rank2.txt";
	std::ifstream fd(filename);
	double eps = 1e-5;
	{
		Matrix<double> M(fd);
		
		Matrix<double> A(M);

		EXPECT_TRUE(A==M);
	}
}

TEST(matrix_creation, copy_assingment) {
	std::string filename = "matrices/m1det0rank2.txt";
	std::ifstream fd(filename);
	double eps = 1e-5;
	{
		Matrix<double> M(fd);
		
		Matrix<double> A(1,1);
		A = M;

		EXPECT_TRUE(A==M);
	}
}

TEST(matrix_creation, move_constructor) {
	std::string filename = "matrices/m1det0rank2.txt";
	std::ifstream fd(filename);
	double eps = 1e-5;
	{
		Matrix<double> M(fd);
		Matrix<double> copy = M;

		Matrix<double> A(std::move(copy));

		EXPECT_TRUE(A==M);
		EXPECT_TRUE(copy.get_data()==nullptr);
	}
}

TEST(matrix_creation, move_assingment) {
	std::string filename = "matrices/m1det0rank2.txt";
	std::ifstream fd(filename);
	double eps = 1e-5;
	{
		Matrix<double> M(fd);
		Matrix<double> copy = M;
		
		Matrix<double> A(1,1);
		A = std::move(copy) ;

		EXPECT_TRUE(A==M);
		EXPECT_TRUE(copy.get_data()==nullptr);
	}
}

TEST(matrix_creation, submatrix) {
	std::string filename = "matrices/m1det0rank2.txt";
	std::ifstream fd(filename);
	double eps = 1e-5;
	{
		Matrix<double> A(fd);
		Matrix<double> M = A.submatrix(1, 2, 1, 2);
		ASSERT_EQ(M.a, 2);
		ASSERT_EQ(M.b, 2);
		
		EXPECT_NEAR(M[0][0], 2, eps);
		EXPECT_NEAR(M[0][1], 1, eps);		

		EXPECT_NEAR(M[1][0], 0, eps);
		EXPECT_NEAR(M[1][1], 1, eps);
	}
}

TEST(matrix_creation, submatrix_full) {
	std::string filename = "matrices/m1det0rank2.txt";
	std::ifstream fd(filename);
	double eps = 1e-5;
	{
		Matrix<double> A(fd);
		Matrix<double> M = A.submatrix(0, A.a, 0, A.b);
		ASSERT_EQ(M.a, 3);
		ASSERT_EQ(M.b, 3);
		
		EXPECT_TRUE(M == A);
	}
}

TEST(matrix_creation, minor) {
    std::string filename = "matrices/m1det0rank2.txt";
	std::ifstream fd(filename);
	double eps = 1e-5;
	{
		Matrix<double> A(fd);
		Matrix<double> M = A.minor(1,1);
		ASSERT_EQ(M.a, 2);
		ASSERT_EQ(M.b, 2);
		
		EXPECT_NEAR(M[0][0], 1, eps);
		EXPECT_NEAR(M[0][1], 3, eps);		

		EXPECT_NEAR(M[1][0], -1, eps);
		EXPECT_NEAR(M[1][1], 1, eps);
	}
}

TEST(matrix_creation, idenity) {
	double eps = 1e-5;
	{
		Matrix<double> I = Matrix<double>::Idenity(3);
		ASSERT_EQ(I.a, 3);
		ASSERT_EQ(I.b, 3);
		for (int y=0; y<I.a; y++) {
			for (int x=0; x<I.b; x++) {
				EXPECT_NEAR(I[y][x], y==x, eps);
			}
		}
	}
}

TEST(matrix_creation, transcripted) {
	std::string filename = "matrices/m2det7rank3.txt";
	std::ifstream fd(filename);
	double eps = 1e-5;
	{
		Matrix<double> A(fd);
		Matrix<double> T = A.trans();

		for (int y=0; y<A.a; y++) {
			for (int x=0; x<A.b; x++) {
				EXPECT_NEAR(A[y][x], T[x][y], eps);
			}
		}
	}
}

TEST(matrix_creation, double_transcripted) {
	std::string filename = "matrices/m2det7rank3.txt";
	std::ifstream fd(filename);
	double eps = 1e-5;
	{
		Matrix<double> A(fd);
		EXPECT_TRUE(A == A.trans().trans());
	}
}


TEST(matrix_saving, idenity_negative_save) {
	Matrix<double> A = Matrix<double>::Idenity(3);

	std::ofstream fdout("matrices/idenity_negative.txt");
	(-2.0*A).save(fdout);
	fdout.close();
	
	std::ifstream fdin("matrices/idenity_negative.txt");
	Matrix<double> B(fdin);
	fdin.close();

	EXPECT_TRUE((-2.0*A) == B);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
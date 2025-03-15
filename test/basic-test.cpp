#include "../gtest/include/gtest/gtest.h"
#define MATRIX_IMPL
#include "../src/matrix.hpp"
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

// TEST(matrix_creation, from_file_id_4_4_int) {
//     std::string filename = "matrices/idenity4_4.txt";
// 	std::ifstream fd(filename);
// 	{
// 		Matrix<int> M(fd);
// 		ASSERT_EQ(M.a, 4);
// 		ASSERT_EQ(M.b, 4);
// 		for (int y=0; y<M.a; y++) {
// 			for (int x=0; x<M.b; x++) {
// 				EXPECT_EQ(M[y][x], y==x);
// 			}
// 		}
// 	}
// }

// TEST(matrix_creation, from_file_id_4_4_double) {
//     std::string filename = "matrices/idenity4_4.txt";
// 	std::ifstream fd(filename);
// 	double eps = 1e-5;
// 	{
// 		Matrix<double> M(fd);
// 		ASSERT_EQ(M.a, 4);
// 		ASSERT_EQ(M.b, 4);
// 		for (int y=0; y<M.a; y++) {
// 			for (int x=0; x<M.b; x++) {
// 				EXPECT_NEAR(M[y][x], y==x, eps);
// 			}
// 		}
// 	}
// }


int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
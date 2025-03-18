CC=g++
CFLAGS=
GTEST_INCLUDE_PATH=./gtest/include
GTEST_LIB_PATH=./gtest/lib

all: basic_test

basic_test: test/basic-test.cpp
	$(CC) test/basic-test.cpp -I$(GTEST_INCLUDE_PATH) -L$(GTEST_LIB_PATH) -lgtest -o basic_test

clean:
	rm basic_test.*
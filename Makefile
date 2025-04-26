CC=g++
CFLAGS=
GTEST_INCLUDE_PATH=./gtest/include
GTEST_LIB_PATH=./gtest/lib
SRC_INCLUDE_PATH=./src

all: basic_test calculation_test lib

basic_test: test/basic-test.cpp $(SRC_INCLUDE_PATH)/matrix.hpp
	$(CC) $(CFLAGS) test/basic-test.cpp -I$(GTEST_INCLUDE_PATH) -I$(SRC_INCLUDE_PATH) -L$(GTEST_LIB_PATH) -lgtest -lpthread -o basic_test

calculation_test: test/calculation-test.cpp $(SRC_INCLUDE_PATH)/matrix.hpp
	$(CC) $(CFLAGS) test/calculation-test.cpp -I$(GTEST_INCLUDE_PATH) -I$(SRC_INCLUDE_PATH) -L$(GTEST_LIB_PATH) -lgtest -lpthread -o calculation_test

clean:
	rm basic_test.* calculation_test.* matrices/idenity_negative.txt matrix.o libmatrix.a
CC=g++
CFLAGS=
GTEST_INCLUDE_PATH=./gtest/include
GTEST_LIB_PATH=./gtest/lib

all: basic_test calculation_test

basic_test: test/basic-test.cpp
	$(CC) $(CFLAGS) test/basic-test.cpp -I$(GTEST_INCLUDE_PATH) -L$(GTEST_LIB_PATH) -lgtest -lpthread -o basic_test

calculation_test: test/calculation-test.cpp
	$(CC) $(CFLAGS) test/calculation-test.cpp -I$(GTEST_INCLUDE_PATH) -L$(GTEST_LIB_PATH) -lgtest -lpthread -o calculation_test

clean:
	rm basic_test.* calculation_test.* matrices/idenity_negative.txt
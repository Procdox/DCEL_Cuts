#include "pch.h"
#include "..\DCEL_CUTS\DCEL_fll.h"
#include "gtest\gtest.h"

TEST(FLL_Tests, construction) {
	FLL<int> test_object;
}

TEST(FLL_Tests, push_and_pop) {
	int list[] = { 4, 7, 2, 4, 7, 9, 0, 3, 1 };

	FLL<int> test_object;

	for (int kk = 0; kk < 9; ++kk) {
		test_object.push(list[kk]);
	}

	int kk = 0;

	do{
		EXPECT_EQ(list[8 - kk++], test_object.pop());

	}while (!test_object.empty());

	EXPECT_EQ(kk, 9);
}

TEST(FLL_Tests, append_and_pop) {
	int list[] = { 4, 7, 2, 4, 7, 9, 0, 3, 1 };

	FLL<int> test_object;

	for (int kk = 0; kk < 9; ++kk) {
		test_object.append(list[kk]);
	}

	int kk = 0;

	do {
		EXPECT_EQ(list[kk++], test_object.pop());

	} while (!test_object.empty());

	EXPECT_EQ(kk, 9);
}

TEST(FLL_Tests, push_and_iterate) {
	int list[] = { 4, 7, 2, 4, 7, 9, 0, 3, 1 };

	FLL<int> test_object;

	for (int kk = 0; kk < 9; ++kk) {
		test_object.push(list[kk]);
	}

	auto focus = test_object.getHead();
	int kk = 0;

	while(focus != nullptr){

		EXPECT_EQ(list[8 - kk++], focus->getValue());

		focus = focus->getNext();
	}

	EXPECT_EQ(kk, 9);
}

TEST(FLL_Tests, append_and_iterate) {
	int list[] = { 4, 7, 2, 4, 7, 9, 0, 3, 1 };

	FLL<int> test_object;

	for (int kk = 0; kk < 9; ++kk) {
		test_object.append(list[kk]);
	}

	auto focus = test_object.getHead();
	int kk = 0;

	while (focus != nullptr) {

		EXPECT_EQ(list[kk++], focus->getValue());

		focus = focus->getNext();
	}

	EXPECT_EQ(kk, 9);
}

TEST(FLL_Tests, push_and_tail) {
	int list[] = { 4, 7, 2, 4, 7, 9, 0, 3, 1 };

	FLL<int> test_object;

	for (int kk = 0; kk < 9; ++kk) {
		test_object.push(list[kk]);
	}

	EXPECT_EQ(test_object.getTail()->getValue(), 4);
}

TEST(FLL_Tests, empty) {
	FLL<int> test_object;

	EXPECT_EQ(test_object.empty(), true);
}

TEST(FLL_Tests, not_empty) {
	FLL<int> test_object;

	test_object.push(6);

	EXPECT_EQ(test_object.empty(), false);
}

TEST(FLL_Tests, remove) {
	int list[] = { 4, 7, 2, 4, 7, 9, 0, 3, 1 };
	int result[] = { 1, 3, 0, 9, 4, 2, 7, 4 };


	FLL<int> test_object;

	for (int kk = 0; kk < 9; ++kk) {
		test_object.push(list[kk]);
	}

	EXPECT_TRUE(test_object.remove(7));

	auto focus = test_object.getHead();
	int kk = 0;

	while (focus != nullptr) {

		EXPECT_EQ(result[kk++], focus->getValue());

		focus = focus->getNext();
	}

	EXPECT_EQ(kk, 8);
}

TEST(FLL_Tests, remove_on_empty) {
	FLL<int> test_object;

	EXPECT_EQ(test_object.removeAll(6), false);
}

TEST(FLL_Tests, removeAll_on_empty) {
	FLL<int> test_object;

	EXPECT_EQ(test_object.removeAll(6), 0);
}

TEST(FLL_Tests, remove_creates_empty) {
	FLL<int> test_object;

	test_object.push(6);

	EXPECT_EQ(test_object.remove(6), true);

	EXPECT_EQ(test_object.empty(), true);
}

TEST(FLL_Tests, removeAll_creates_empty) {
	FLL<int> test_object;

	test_object.push(6);
	test_object.push(6);
	test_object.push(6);

	EXPECT_EQ(test_object.removeAll(6), 3);

	EXPECT_EQ(test_object.empty(), true);
}

TEST(FLL_Tests, remove_all) {
	int list[] = { 4, 7, 2, 4, 7, 9, 0, 3, 1 };
	int result[] = { 1, 3, 0, 9, 4, 2, 4 };


	FLL<int> test_object;

	for (int kk = 0; kk < 9; ++kk) {
		test_object.push(list[kk]);
	}

	EXPECT_EQ(test_object.removeAll(7),2);

	auto focus = test_object.getHead();
	int kk = 0;

	while (focus != nullptr) {

		EXPECT_EQ(result[kk++], focus->getValue());

		focus = focus->getNext();
	}

	EXPECT_EQ(kk, 7);
}
#include <memory>
#include <gtest/gtest.h>

#include <utils/buffer>

using namespace com::nealrame;

char test_data_str[] = "test data string";

TEST(buffer, DefaultConstructor) {
	utils::buffer buf;
	
	EXPECT_EQ(nullptr, buf.data());
	EXPECT_EQ(0, buf.length());
}

TEST(buffer, Constructor) {
	utils::buffer buf(test_data_str, sizeof(test_data_str));
	
	EXPECT_EQ(test_data_str, buf.data());
	EXPECT_EQ(sizeof(test_data_str), buf.length());
}

TEST(buffer, Set) {
	utils::buffer buf;
	buf.set(test_data_str, sizeof(test_data_str));

	EXPECT_EQ(test_data_str, buf.data());
	EXPECT_EQ(sizeof(test_data_str), buf.length());
}

TEST(buffer, Copy) {
	char data[sizeof(test_data_str)];
	utils::buffer buf(data, sizeof(test_data_str));

	buf.copy(test_data_str, sizeof(test_data_str), 0);

	EXPECT_NE(nullptr, buf.data());
	EXPECT_EQ(sizeof(test_data_str), buf.length());
	EXPECT_EQ(0, memcmp(test_data_str, buf.data(), buf.length()));
}

TEST(buffer, BoundCopy) {
	uint8_t data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	utils::buffer buf(data, sizeof(data));
	buf.copy(test_data_str, 6, 1);

	EXPECT_NE(nullptr, buf.data());
	EXPECT_EQ(sizeof(data), buf.length());
	EXPECT_EQ(0, data[0]);
	EXPECT_EQ(0, data[7]);
	EXPECT_EQ(0, memcmp(test_data_str, data + 1, 6));
}

TEST(buffer, OutOfBoundCopy) {
	uint8_t data0[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t data1[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

	utils::buffer buf(data0 + 1, sizeof(data0) - 2);
	buf.copy(data1, sizeof(data1), 0);

	EXPECT_NE(nullptr, buf.data());
	EXPECT_EQ(sizeof(data0) - 2, buf.length());
	EXPECT_EQ(0, data0[0]);
	EXPECT_EQ(0, data0[7]);
	EXPECT_EQ(0, memcmp(data1, data0 + 1, 6));
}

TEST(buffer, Fill) {
	uint8_t data0[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t data1[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

	utils::buffer buf(data0, sizeof(data0));
	buf.fill(1, sizeof(data0), 0);

	EXPECT_NE(nullptr, buf.data());
	EXPECT_EQ(sizeof(data0), buf.length());
	EXPECT_EQ(0, memcmp(data1, data0, sizeof(data0)));
}

TEST(buffer, BoundFill) {
	uint8_t data0[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t data1[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

	utils::buffer buf(data0, sizeof(data0));
	buf.fill(1, 6, 1);

	EXPECT_NE(nullptr, buf.data());
	EXPECT_EQ(sizeof(data0), buf.length());
	EXPECT_EQ(0, data0[0]);
	EXPECT_EQ(0, data0[7]);
	EXPECT_EQ(0, memcmp(data1, data0 + 1, 6));
}

TEST(buffer, OutOfBoundFill) {
	uint8_t data0[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t data1[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

	utils::buffer buf(data0 + 1, sizeof(data0) - 2);
	buf.fill(1, sizeof(data0), 0);

	EXPECT_NE(nullptr, buf.data());
	EXPECT_EQ(sizeof(data0)-2, buf.length());
	EXPECT_EQ(0, data0[0]);
	EXPECT_EQ(0, data0[7]);
	EXPECT_EQ(0, memcmp(data1, data0 + 1, 6));
}

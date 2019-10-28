#include "tqueue.h"

#include <gtest.h>
#include <queue>


TEST(TQueue, can_create_queue_with_positive_length)
{
	ASSERT_NO_THROW(TQueue<int>(5));
}

TEST(TQueue, cant_create_queue_stack_with_negative_length)
{
	ASSERT_ANY_THROW(TQueue<int>(-5));
}


TEST(TQueue, can_push_and_get_element) {
	TQueue<int> qu(5);
	const int a = 15;
	ASSERT_NO_THROW(qu.push(a));
	EXPECT_EQ(a, qu.top());
}

TEST(TQueue, can_push_and_get_element_2) {
	TQueue<int> qu(5);
	const int a = 15, b=20;
	ASSERT_NO_THROW(qu.push(a));
	ASSERT_NO_THROW(qu.push(b));
	EXPECT_EQ(a, qu.top());
}

TEST(TQueue, can_push_and_get_element_3) {
	TQueue<int> qu(2);
	const int a = 15, b = 20, c=7;
	ASSERT_NO_THROW(qu.push(a));
	ASSERT_NO_THROW(qu.push(b));
	ASSERT_NO_THROW(qu.push(c));
	EXPECT_EQ(a, qu.top());
}

TEST(TQueue, can_push_and_get_element_4) {
	TQueue<int> qu(2);
	const int a = 15, b = 20, c = 7;
	ASSERT_NO_THROW(qu.push(a));
	ASSERT_NO_THROW(qu.push(b));
	ASSERT_NO_THROW(qu.push(c));
	EXPECT_EQ(a, qu.top());
}

TEST(TQueue, empty_queue_is_empty) {
	TQueue<int> qu(2);
	const int a = 15;
	ASSERT_NO_THROW(qu.push(a));
	ASSERT_NO_THROW(qu.pop());
	EXPECT_EQ(1, qu.IsEmpty());
}

TEST(TQueue, new_queue_is_empty) {
	TQueue<int> qu(2);
	EXPECT_EQ(1, qu.IsEmpty());
}

TEST(TQueue, queue_is_empty) {
	TQueue<int> qu(5);
	const int a = 15;
	for (int i = 0; i < 17; ++i) {
		ASSERT_NO_THROW(qu.push(a));
		ASSERT_NO_THROW(qu.pop());
		ASSERT_EQ(1, qu.IsEmpty());
	}
}

TEST(TQueue, ultimate_test) {
	TQueue<int> qu(2);
	std::queue<int> exp_qu;
	for (int i = 0; i < 15; ++i) {
		qu.push(i);
		exp_qu.push(i);
	}
	for (int i = 0; i < 15; ++i) {
		EXPECT_EQ(qu.top(), exp_qu.front());
		qu.pop();
		exp_qu.pop();
	}
}
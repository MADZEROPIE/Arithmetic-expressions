#include "tstack.h"

#include <gtest.h>

#ifndef STACK_BY_LIST //ћожно добавл€ть DISABLED к тестам, а можно делать так 

TEST(TStack, can_create_stack_with_positive_length)
{
	ASSERT_NO_THROW(TStack<int>(5));
}

TEST(TStack, cant_create_stack_with_negative_length)
{
	ASSERT_ANY_THROW(TStack<int>(-15));
}
#endif // STACK_BY_LIST

TEST(TStack, can_push_and_get_element) {
	TStack<int> st(5);
	const int a = 15;
	ASSERT_NO_THROW(st.push(a));
	EXPECT_EQ(a,st.top());
}

TEST(TStack, can_delete_element) {
	TStack<int> st(5);
	const int a = 15;
	ASSERT_NO_THROW(st.push(a));
	EXPECT_EQ(a, st.top());
	ASSERT_NO_THROW(st.pop());
}

TEST(TStack, cant_get_element_from_empty_stack) {
	TStack<int> st(5);
	ASSERT_ANY_THROW(st.top());
}

TEST(TStack, cant_delete_element_from_empty_stack) {
	TStack<int> st(5);
	ASSERT_ANY_THROW(st.pop());
}

TEST(TStack,empty_stack_is_empty) {
	TStack<int> st(5);
	EXPECT_EQ(1, st.IsEmpty());
}


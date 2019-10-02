#include "tstack.h"

#include <gtest.h>
//#ifdef STACK_BY_LIST //ћожно добавл€ть DISABLED к тестам, а можно делать так 

//#else

TEST(TStack, can_create_stack_with_positive_length)
{
	ASSERT_NO_THROW(TStack<int>(5));
}

TEST(TStack, cant_create_stack_with_negative_length)
{
	ASSERT_ANY_THROW(TStack<int>(-15));
}

//#endif // STACK_BY_LIST
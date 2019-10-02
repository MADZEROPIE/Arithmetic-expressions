#include <iostream>
#include "tstack.h"
#include "tqueue.h"


int main()
{
	setlocale(LC_ALL, "Russian");
	TStack<int> a(10);
	for (int i = 1; i < 5; ++i) a.push(i * 2);
	TStack<int> b(17);
	b = a;
	for (int i = 1; i < 5; ++i) cout << b.pop() << ' ';

	return 0;
}


#include <iostream>
#include "tstack.h"
#include "tqueue.h"


int main()
{
	setlocale(LC_ALL, "Russian");
	TQueue<int> a(10);
	for (int i = 1; i < 11; ++i) a.push(i * 2);
	for (int i = 1; i < 11; ++i) cout << a.pop() << ' ';
	return 0;
}


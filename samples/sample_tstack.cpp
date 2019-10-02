#include <iostream>
#include "tstack.h"


int main()
{
    int i, j;
	setlocale(LC_ALL, "Russian");
	TStack<int> a(10);
	for (int i = 1; i < 11; ++i) a.Put(i * 2);
	for (int i = 1; i < 11; ++i) cout << a.Get() << ' ';
	return 0;
}


#include <iostream>
#include "tstack.h"
#include "tqueue.h"


int main()
{
	setlocale(LC_ALL, "Russian");
	TStack<long long int> a(10);
	for (long long int i = 1; i < 46555560; ++i) a.push(i * (long long int) 2);
	TStack<long long int> b(a);
	//for (long long i = 1; i < 46555561; ++i) b.push(i * long long(3));
	//b=a;
	for (int i = 1; i < 25; ++i) cout << b.pop() << ' ';

	return 0;
}


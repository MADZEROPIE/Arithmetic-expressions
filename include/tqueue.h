#pragma once

#include "tstack.h"

template<typename T>
class TQueue {
protected:
	TStack<T> st1, st2;
public:
	TQueue(int Size = 15) :st1(Size), st2(Size) {}
	~TQueue() {}
	void push(const T& Val) {
		st1.push(Val);
	}
	T pop(void) {
		shift();
		return st2.pop();
	}
	void shift() { //Перемещение элементов из 1 стека во второй
		if (st2.IsEmpty()) {
			while (!st1.IsEmpty())
			st2.push(st1.pop());
		}
	}
};

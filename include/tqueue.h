#pragma once

#include "tstack.h"

template<typename T>
class TQueue_by_stack {
protected:
	TStack<T> st1, st2;

	void shift() { //ѕеремещение элементов из 1 стека во второй
		if (st2.IsEmpty()) {
			while (!(st1.IsEmpty())) {
				st2.push(st1.top());
				st1.pop();
			}
		}
	}
public:
	TQueue_by_stack(int Size = 15) :st1(Size), st2(Size) {}
	~TQueue_by_stack() {}

	void push(const T& Val) {
		st1.push(Val);
	}

	void pop(void) {
		shift();
		return st2.pop();
	}

	T& top(void) {
		shift();
		return st2.top();
	}
	
};

template<typename T>
class TQueue {
protected:
	T* arr;
	size_t head;
	size_t tail;
	size_t size;
	size_t DataCount=0; //нужно дл€ провеки на пустоту, иначе непон€тно как провер€ть
public:
	TQueue(int Size=10) {
		if (Size < 0) throw exception();
		arr = new T[Size];
		size = Size;
		head =  0;
		tail = 1;
	}
	~TQueue()
	{
		delete[] arr;
	}

	void push(const T& data) {
		if (head == tail) {
			T* tmp = new T[size + size / 3 + 1];
			for (int i = 0; i < size; ++i) {
					tmp[i] = arr[(head + i) % size];
			}
			delete[] arr;
			arr = tmp;
			head = 0; tail = size;
			size = size + size / 3 + 1;
		}
		arr[(tail-1)%size] = data;
		tail = (tail + 1) % size;
		++DataCount;
	}
	T& top() {
		if (IsEmpty()) throw exception();
		return arr[head];
	}
	void pop() {
		if (IsEmpty()) throw exception();
		head = (head + 1) % size;
		--DataCount;
	}
	bool IsEmpty() { 
		return DataCount==0; //корректно, но лишн€€ пам€ть 
	}
};

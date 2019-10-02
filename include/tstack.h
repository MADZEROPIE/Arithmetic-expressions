#pragma once
#include <iostream>

using namespace std;
#define STACK_BY_LIST //Закоментировать для реализации стека через массив

#ifndef STACK_BY_LIST 

template <typename T>
class TStack {
protected: // поля
	T* pMem; // указатель на массив элементов
	int MemSize; // размер памяти 
	int DataCount; // количество элементов

public:
	TStack (int Size = 15);//конструктор
	~TStack(); //деструктор
	int IsEmpty ( void ) const ; // контроль пустоты
	int IsFull ( void ) const ; // контроль переполнения
	void push ( const T& Val );// добавить значение
	void clear() { DataCount = 0; }
	virtual T pop ( void ) ; // извлечь значение
};



template<typename T>
inline TStack<T>::TStack(int Size)
{
	if (Size < 0) throw exception("Incorrect lenght");
	pMem = new T[Size];
	DataCount = 0;
	MemSize = Size;
}

template<typename T>
inline TStack<T>::~TStack()
{
	delete[] pMem;
}

template<typename T>
inline int TStack<T>::IsEmpty(void) const
{
	return (DataCount==0);
}

template<typename T>
inline int TStack<T>::IsFull(void) const
{
	return (DataCount == MemSize);
}

template<typename T>
inline void TStack<T>::push(const T& Val)
{
	if (IsFull()) {
		delete[] pMem;
		MemSize = MemSize + MemSize / 3 + 1;
		pMem = new T[MemSize];
	}
	pMem[DataCount++] = Val;
}

template<typename T>
inline T TStack<T>::pop(void)
{
	if (IsEmpty()) throw exception("No elements here");
	return pMem[(DataCount--) -1];
}

#else
template <typename T>
class TStack {
protected: // поля
	
	class Node
	{
	public:
		Node* pPr;
		T data;
		Node(T data, Node* pPr = NULL) {
			this->data = data;
			this->pPr = pPr;
		}
	};
	int MemSize; // размер памяти 
	int DataCount; // количество элементов
	Node* Head;


public:
	TStack(int Size = 15) {
		Head = NULL;
		MemSize = 0;
		DataCount = 0;
	}//конструктор
	~TStack() {
		clear();
	} //деструктор
	bool IsEmpty(void) const { return DataCount == 0; } // контроль пустоты
	bool IsFull(void) const {
		return false;
	}; // контроль переполнения, не нужен в этой реализации
	void push(const T& Val) {
		Node* node = new Node(Val,Head);
		Head = node;
		++DataCount;
	};// добавить значение
	void clear() {
		while (Head != NULL) pop();
	}
	virtual T pop(void) {
		if(IsEmpty()) throw exception("Incorrect lenght");
		Node* tmp = Head->pPr;
		T tmp2 = Head->data;
		delete Head;
		Head = tmp;
		--DataCount;
		return tmp2;
	}// извлечь значение
};


#endif // ! 

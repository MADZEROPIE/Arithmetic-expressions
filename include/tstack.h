#pragma once
#include <iostream>

using namespace std;
//#define STACK_BY_LIST //Закоментировать для реализации стека через массив.
// Быстродействие - довольно забавная штука. Для обработки 46,5 миллионов элементов массив  работает ~3 секунд, список ~27 секунд и при этом список весит в ~8(???) раз больше. (время с выводом на консоль 25 элементов)
// Вывод: Список, несмотря на асимптотику O(1) в худшем случае, - это Альянс. Сложная шутка, но зато правда.

#ifndef STACK_BY_LIST 

template <typename T>
class TStack {
protected: // поля
	T* pMem; // указатель на массив элементов
	int MemSize; // размер памяти 
	int DataCount; // количество элементов

public:
	TStack (int Size = 15);//конструктор
	TStack(const TStack& st);
	~TStack(); //деструктор
	int IsEmpty ( void ) const ; // контроль пустоты
	int IsFull ( void ) const ; // контроль переполнения
	void push ( const T& Val );// добавить значение
	void clear() { DataCount = 0; }
	int size() { return DataCount; }
	TStack<T>& operator=(const TStack& st);
	T top() { return pMem[DataCount - 1]; }
	T pop ( void ) ; // извлечь значение
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
inline TStack<T>::TStack(const TStack<T>& st)
{
	MemSize = st.MemSize; DataCount = st.DataCount;
	pMem = new T[st.MemSize];
	for (int i = 0; i < DataCount; ++i)pMem[i] = st.pMem[i];
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
		T* tmp = new T[MemSize];
		for (int i = 0; i < DataCount; ++i) tmp[i]= pMem[i];
		delete[] pMem;
		MemSize = MemSize + MemSize / 3 + 1;
		pMem = new T[MemSize];
		for (int i = 0; i < DataCount; ++i) pMem[i] = tmp[i];
		delete[] tmp;
	}
	pMem[DataCount++] = Val;
}

template<typename T>
inline TStack<T>& TStack<T>::operator=(const TStack& st)
{	
	if (this != &st) {
		DataCount = st.DataCount;
		if (MemSize != st.MemSize) {
			delete[] pMem;
			pMem = new T[st.MemSize];
			MemSize = st.MemSize;
		}
		for (int i = 0; i < MemSize; ++i) pMem[i] = st.pMem[i];
	}
	return *this;
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
	int DataCount; // количество элементов совпадает с размером памяти
	Node* Head; //верхний элемент


public:
	TStack(int Size = 15) {
		Head = NULL;
		DataCount = 0;
	}//конструктор

	TStack(const TStack& st) {
		DataCount = st.DataCount;
		Head = NULL;
		if (DataCount) {
			Node* node = new Node(st.Head->data, Head);
			Head = node;
			Node* pst = (st.Head)->pPr;
			while (pst != NULL) {
				node->pPr = new Node(pst->data);
				pst = pst->pPr;
				node = node->pPr;
			}
		}
	}

	~TStack() {
		clear();
	} //деструктор

	bool IsEmpty(void) const { return DataCount == 0; } // контроль пустоты
	bool IsFull(void) const { return false; } // контроль переполнения, не нужен в этой реализации
	
	TStack<T>& operator=(const TStack& st) {
		if (this != &st) {
			this->clear(); //Сводим задачу к предыдущей. Можно попытаться оптимизировать и не удалять Node, а перезаписывать в нём data.
			DataCount = st.DataCount;
			Head = NULL;
			if (DataCount) {
				Node* node = new Node(st.Head->data, Head);
				Head = node;
				Node* pst = (st.Head)->pPr;
				while (pst != NULL) {
					node->pPr = new Node(pst->data);
					pst = pst->pPr;
					node = node->pPr;
				}
			}
		}
		return *this;
	}

	void push(const T& Val) {
		Node* node = new Node(Val,Head);
		Head = node;
		++DataCount;
	};// добавить значение

	void clear() {
		while (Head != NULL) pop();
	}

	int size() { return DataCount; }

	T pop(void) {
		if(IsEmpty()) throw exception();
		Node* tmp_node = Head->pPr;
		T tmp_data = Head->data;
		delete Head;
		Head = tmp_node;
		--DataCount;
		return tmp_data;
	}// извлечь значение

	T top(void) {
		if (IsEmpty()) throw exception();
		return Head->data;
	}
};


#endif // ! 

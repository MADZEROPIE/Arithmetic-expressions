#pragma once
#include <iostream>

using namespace std;
#define STACK_BY_LIST //��������������� ��� ���������� ����� ����� ������

#ifndef STACK_BY_LIST 

template <typename T>
class TStack {
protected: // ����
	T* pMem; // ��������� �� ������ ���������
	int MemSize; // ������ ������ 
	int DataCount; // ���������� ���������

public:
	TStack (int Size = 15);//�����������
	~TStack(); //����������
	int IsEmpty ( void ) const ; // �������� �������
	int IsFull ( void ) const ; // �������� ������������
	void push ( const T& Val );// �������� ��������
	void clear() { DataCount = 0; }
	virtual T pop ( void ) ; // ������� ��������
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
protected: // ����
	
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
	int MemSize; // ������ ������ 
	int DataCount; // ���������� ���������
	Node* Head;


public:
	TStack(int Size = 15) {
		Head = NULL;
		MemSize = 0;
		DataCount = 0;
	}//�����������
	~TStack() {
		clear();
	} //����������
	bool IsEmpty(void) const { return DataCount == 0; } // �������� �������
	bool IsFull(void) const {
		return false;
	}; // �������� ������������, �� ����� � ���� ����������
	void push(const T& Val) {
		Node* node = new Node(Val,Head);
		Head = node;
		++DataCount;
	};// �������� ��������
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
	}// ������� ��������
};


#endif // ! 

#pragma once
#include <iostream>

using namespace std;

const int MaxMemSize = 25; // максимальный размер памяти для стека

typedef int TELEM; // тип элемента 

class TStack {
protected: // поля
	TELEM* pMem; // указатель на массив элементов
	int MemSize; // размер памяти 
	int DataCount; // количество элементов
	int Hi; // индекс вершины стека
	virtual int GetNextIndex (int index); // получить следующий индекс
public:
	TStack (int Size = MaxMemSize);//конструктор
	~TStack(); //деструктор
	int IsEmpty ( void ) const ; // контроль пустоты
	int IsFull ( void ) const ; // контроль переполнения
	void Put ( const TELEM&Val );// добавить значение
	virtual TELEM Get ( void ) ; // извлечь значение
};

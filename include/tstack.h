#pragma once
#include <iostream>

using namespace std;

const int MaxMemSize = 25; // ������������ ������ ������ ��� �����

typedef int TELEM; // ��� �������� 

class TStack {
protected: // ����
	TELEM* pMem; // ��������� �� ������ ���������
	int MemSize; // ������ ������ 
	int DataCount; // ���������� ���������
	int Hi; // ������ ������� �����
	virtual int GetNextIndex (int index); // �������� ��������� ������
public:
	TStack (int Size = MaxMemSize);//�����������
	~TStack(); //����������
	int IsEmpty ( void ) const ; // �������� �������
	int IsFull ( void ) const ; // �������� ������������
	void Put ( const TELEM&Val );// �������� ��������
	virtual TELEM Get ( void ) ; // ������� ��������
};

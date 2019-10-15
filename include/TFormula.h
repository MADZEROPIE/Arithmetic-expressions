#pragma once
#include "tstack.h"
#include <string>

enum state_enum {ORIGIN_STATE, WAIT_FOR_NUM ,ERROR , SUCCESS}; //������������ ���������

//template <typename T> // � ����� ����� ������ ������???, ��������� �� �� �����
class TFormula {
private:
	string orig_exp;
	string postfix_exp;
public:
	void make_postfix();
	bool check_exp();
	double calc();
};

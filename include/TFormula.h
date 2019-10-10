#pragma once
#include "tstack.h"
#include <string>

//template <typename T> // а зачем здесь вообще шаблон???
class TFormula {
private:
	string orig_exp;
	string postfix_exp;
public:
	void make_postfix();
	bool check_exp();
	double calc();
};
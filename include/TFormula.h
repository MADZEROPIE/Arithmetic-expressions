#pragma once
#include "tstack.h"
#include <string>
#include <vector>
#include <cmath>

typedef double real;

enum state_enum {ORIGIN_STATE=0, WAIT_FOR_OP ,ERROR , SUCCESS}; //Перечисление состояний
enum operation_enum {open_bracket, close_bracket, op_plus, op_minus, op_mult, op_div, op_pow, op_exp, op_cos,op_sin };

class Lexer {
	friend ostream& operator<< (ostream& out, const Lexer& op);
	virtual void f() {}
}; //Для up-cast

class TFormula {
private:
	string orig_exp;
	vector<Lexer*> arr;
	state_enum current_state = ORIGIN_STATE;
public:
	TFormula(const string& a) { orig_exp = a; }
	bool check_exp();
	void make_postfix();
	real calc();
	void show();
};




class Lexer_real:public Lexer {
	real a;
public:
	Lexer_real(const real& b) { a = b; }
	friend ostream& operator<< (ostream& out, const Lexer_real& num) {
		return out << num.a;
	}
};

class Lexer_operation : public Lexer {
	short int priority;
	operation_enum code;
public:
	friend ostream& operator<< (ostream& out, const Lexer_operation& op);
	Lexer_operation(const char& op);
};
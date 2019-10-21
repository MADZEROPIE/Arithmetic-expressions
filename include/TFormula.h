#pragma once
#include "tstack.h"
#include <string>
#include <vector>
#include <cmath>

typedef double real;

enum state_enum {ORIGIN_STATE=0, WAIT_FOR_OP ,ERROR , SUCCESS}; //������������ ���������
enum operation_enum {open_bracket, close_bracket, op_plus, op_minus, op_mult, op_div, op_pow, op_exp, op_cos,op_sin };

class Lexer {
	friend ostream& operator<< (ostream& out, const Lexer& op);
	virtual void f() {}
}; //��� up-cast

class TFormula {
private:
	string orig_exp;
	vector<Lexer*> arr;
	vector<Lexer*> post_arr;
	state_enum current_state = ORIGIN_STATE;
public:
	TFormula(const string& a) { orig_exp = a; }
	bool check_exp();
	void make_postfix();
	real calc();
	void show_lex();
	void show_postfix();
};



class Lexer_real:public Lexer {
public:
	real a;

	Lexer_real(const real& b) { a = b; }
	friend ostream& operator<< (ostream& out, const Lexer_real& num) {
		return out << num.a;
	}
};

class Lexer_operation : public Lexer {
public:
	short int priority;
	operation_enum code;

	friend ostream& operator<< (ostream& out, const Lexer_operation& op);
	bool operator==(char op);
	bool operator>(const Lexer_operation& b) { return priority > b.priority; }

	Lexer_operation(const char& op);
};
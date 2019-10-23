#pragma once
#include "tstack.h"
#include <string>
#include <vector>
#include <cmath>
#include <map>

typedef double real;

enum state_enum { WAIT_FOR_INPUT, ORIGIN_STATE, WAIT_FOR_OP ,ERROR , CHECK_DONE, POSTFIX_DONE }; //Перечисление состояний
enum operation_enum {open_bracket, close_bracket, op_plus, op_minus, op_mult, op_div, op_pow, op_un_plus, op_un_min, op_exp, op_cos, op_sin, op_ln };

class Lexer {
public:
	friend ostream& operator<< (ostream& out, const Lexer& op);
	virtual ~Lexer() {}
}; //Для up-cast

class TFormula {
private:
	vector<string> orig_exp_arr;
	vector<vector<Lexer*> >arr;
	vector<vector<Lexer*> > post_arr;
	map<string, Lexer*> op_list; // Список операций НЕ РАБОТАЕТ!!!
	//map<string, Lexer*> name_list; //Имена переменных НЕ РАБОТАЕТ!!!
	state_enum current_state = ORIGIN_STATE;
public:
	TFormula();
	TFormula(const string& a):TFormula() { orig_exp_arr.push_back (a); }
	TFormula(const vector<string>& vec): TFormula() { orig_exp_arr = vec; }
	~TFormula();
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
	~Lexer_real() {}
};

class Lexer_operation : public Lexer {
public:
	short int priority;
	operation_enum code;

	friend ostream& operator<< (ostream& out, const Lexer_operation& op);
	bool operator==(char op);
	bool operator>(const Lexer_operation& b) { return priority > b.priority; }

	Lexer_operation(const char& op);
	Lexer_operation(const operation_enum& op);
	~Lexer_operation() {}
};
#pragma once
#include "tstack.h"
#include "tqueue.h"
#include <string>
#include <vector>
#include <cmath>
#include <map>

typedef double real;

enum state_enum { WAIT_FOR_INPUT, ORIGIN_STATE, WAIT_FOR_OP ,ERROR , CHECK_DONE, POSTFIX_DONE }; //Перечисление состояний
enum operation_enum {open_bracket, close_bracket, op_set, op_plus, op_minus, op_mult, op_div, op_pow, op_un_plus, op_un_min, op_exp, op_cos, op_sin, op_ln }; //Перечисление операций
//Добавить операцию make_coffee или coffee, при выполнении которой throw "cup of coffee"

class Lexeme {
public:
	friend ostream& operator<< (ostream& out, const Lexeme& op);
	virtual ~Lexeme() {}
}; //Для up-cast

class Lexeme_real :public Lexeme {
public:
	real a;
	Lexeme_real() {}
	Lexeme_real(const real& b) { a = b; }
	friend ostream& operator<< (ostream& out, const Lexeme_real& num) {
		return out << num.a;
	}
	~Lexeme_real() {}
};

class Lexeme_var : public Lexeme_real {
protected:
	string name;
public:
	Lexeme_var(const string& _name) { name = _name; }
	friend ostream& operator<< (ostream& out, const Lexeme_var& var) {
		return out << var.name;
	}
	~Lexeme_var() {}
};

class Lexeme_operation : public Lexeme {
public:
	short int priority;
	operation_enum code;

	friend ostream& operator<< (ostream& out, const Lexeme_operation& op);
	Lexeme_operation(const operation_enum& op);
	~Lexeme_operation() {}
};

class TFormula {
private:
	vector<string> orig_exp_arr;
	vector<vector<Lexeme*> >arr;
	vector<vector<Lexeme*> > post_arr;
	map<string, Lexeme*> op_list;
	map<string, Lexeme*> name_list; 
	state_enum current_state = ORIGIN_STATE;
	string make_name(string& a, int& i);
	void make_op_list();
public:
	TFormula();
	TFormula(const string& a) :TFormula() { orig_exp_arr.push_back(a); current_state = ORIGIN_STATE; }
	TFormula(const vector<string>& vec): TFormula() { orig_exp_arr = vec; current_state = ORIGIN_STATE;	}
	~TFormula();
	real& operator[](const string& a) { return static_cast<Lexeme_real*>(name_list[a])->a; }
	bool check_exp();
	void make_postfix();
	real calc();
	void show_lex();
	void show_postfix();
	friend istream& operator<< (istream& inp, TFormula& form);
	
};
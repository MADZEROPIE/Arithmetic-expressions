#pragma once
#include "tstack.h"
#include "tqueue.h"
#include <string>
#include <vector>
#include <cmath>
#include <map>

#define prefix_stringify(something) std::string(#something)

typedef double real;

enum state_enum { WAIT_FOR_INPUT, ORIGIN_STATE, WAIT_FOR_OP ,ERROR , CHECK_DONE, POSTFIX_DONE }; //ѕеречисление состо€ний
enum operation_enum {open_bracket, close_bracket, op_set, op_plus, op_minus, op_mult, op_div, op_pow, op_un_plus, op_un_min, op_exp, op_cos, op_sin, op_ln }; //ѕеречисление операций
//ƒобавить операцию make_coffee или coffee, при выполнении которой throw "cup of coffee"

class Lexeme {
public:
	friend std::ostream& operator<< (std::ostream& out, const Lexeme& op);
	virtual ~Lexeme() {}
}; //ƒл€ up-cast

class Lexeme_real :public Lexeme {
public:
	real a;
	Lexeme_real() {}
	Lexeme_real(const real& b) { a = b; }
	friend std::ostream& operator<< (std::ostream& out, const Lexeme_real& num) {
		return out << num.a;
	}
	~Lexeme_real() {}
};

class Lexeme_var : public Lexeme_real {
protected:
	std::string name;
public:
	Lexeme_var(const std::string& _name) { name = _name; }
	Lexeme_var(const std::string& _name, real num) { name = _name; a = num; }//ј можно делегировать конструкторы

	friend std::ostream& operator<< (std::ostream& out, const Lexeme_var& var) {
		return out << var.name;
	}

	~Lexeme_var() {}
};

class Lexeme_operation : public Lexeme {
public:
	short int priority;
	operation_enum code;

	friend std::ostream& operator<< (std::ostream& out, const Lexeme_operation& op);
	Lexeme_operation(const operation_enum& op);
	~Lexeme_operation() {}
};

class TFormula {
private:
	std::vector<std::string> orig_exp_arr; // ћассив исходных строк
	std::vector<std::vector<Lexeme*> >arr; // ћассив массива лексем, где arr[i] соответстует лексемам i строки исходного выражени€
	std::vector<std::vector<Lexeme*> > post_arr; //arr в постфиксной форме
	std::map<std::string, Lexeme*> op_list; //дерево операций
	std::map<std::string, Lexeme*> name_list;  //дерево переменных
	state_enum current_state; // “екущее состо€ние
	std::string make_name(std::string& a, int& i); //—оздание имени из строки начина€ и i-ого эл-та
	void make_op_list(); //задает дерево операций

public:
	TFormula();
	TFormula(const std::string& a) :TFormula() { orig_exp_arr.push_back(a); current_state = ORIGIN_STATE; }
	TFormula(const std::vector<std::string>& vec): TFormula() { orig_exp_arr = vec; current_state = ORIGIN_STATE;	}
	~TFormula();

	real& operator[](const std::string& name) { return static_cast<Lexeme_real*>(name_list[name])->a; } //¬озращает значение переменной c именем name

	bool check_exp();
	void make_postfix();
	real calc();
	real& add_var(const std::string& name, real num=0.0);

	void clear();
	void show_lex(); //вывод лексем
	void show_postfix(); //вывод лексем в постфиксной форме
	friend std::istream& operator>> (std::istream& inp, TFormula& form);
	
};
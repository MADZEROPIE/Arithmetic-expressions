#include "TFormula.h"

void TFormula::make_postfix()
{
	if (current_state != CHECK_DONE) return;
	for (int i = 0; i < arr.size(); ++i){
			TStack<Lexer_operation*> stack;
		for (int j = 0; j < arr[i].size(); ++j) {
			if (dynamic_cast<Lexer_real*>(arr[i][j])) post_arr[i].push_back(arr[i][j]);
			else {
				Lexer_operation* op = dynamic_cast<Lexer_operation*> (arr[i][j]);
				if (stack.IsEmpty()) stack.push(op);
				else if (op->code == open_bracket) stack.push(op);
				else if (op->code == close_bracket) {
					while (stack.top()->code != open_bracket) {
						post_arr[i].push_back(dynamic_cast<Lexer*>(stack.top()));
						stack.pop();
					}
					stack.pop();
				}
				else if (*op > * stack.top()) stack.push(op);
				else {
					post_arr[i].push_back(dynamic_cast<Lexer*>(stack.top()));
					stack.pop();
					stack.push(op);
				}
			}
		}
		while (!stack.IsEmpty()) {
			post_arr[i].push_back(dynamic_cast<Lexer*>(stack.top()));
			stack.pop();
		}
	}
	current_state = POSTFIX_DONE;
}

TFormula::TFormula() //Копипаста - это плохо...
{
	Lexer* op = new Lexer_operation(open_bracket);
	op_list.emplace("(", op);
	op = new Lexer_operation(close_bracket);
	op_list.emplace(")", op);
	op = new Lexer_operation(op_plus);
	op_list.emplace("+", op);
	op = new Lexer_operation(op_minus);
	op_list.emplace("-", op);
	op = new Lexer_operation(op_mult);
	op_list.emplace("*", op);
	op = new Lexer_operation(op_div);
	op_list.emplace("/", op);
	 op = new Lexer_operation(op_pow);
	op_list.emplace("^", op);
	op = new Lexer_operation(op_un_min);
	op_list.emplace("u-", op);
	op = new Lexer_operation(op_un_plus);
	op_list.emplace("u+", op);
	op = new Lexer_operation(op_sin);
	op_list.emplace("sin", op);
	op = new Lexer_operation(op_cos);
	op_list.emplace("cos", op);
	op = new Lexer_operation(op_ln);
	op_list.emplace("ln", op);
}

TFormula::~TFormula()
{
	for(int i=0;i<arr.size();++i) for (auto elem : arr[i]) 
		if(dynamic_cast<Lexer_real*>(elem) ) 
			delete elem;
	//for (auto it = name_list.begin(); it != name_list.end(); ++it) delete it->second;
	for (auto it = op_list.begin(); it != op_list.end(); ++it) delete it->second;

	//Все указатели лежат в arr, поэтому не надо удалять элементы в post_arr
}

bool TFormula::check_exp()
{
	arr.resize(orig_exp_arr.size());
	post_arr.resize(orig_exp_arr.size());
	for (int i = 0; i < orig_exp_arr.size(); ++i) {
		TStack<char> brackets;
		string orig_exp = orig_exp_arr[i];
		int k = 0;
		for (; k < orig_exp.size(); ++k) {
			// Состояние 0
			if (current_state == ORIGIN_STATE) {
				if (orig_exp[k] >= '0' && orig_exp[k] <= '9') {
					real tmp = (orig_exp[k] - '0');
					int j = k + 1; real after_dot = 0.1;
					for (bool dot = false; (j < orig_exp.size()) && ((orig_exp[j] >= '0' && orig_exp[j] <= '9') || (orig_exp[j] == '.' || orig_exp[j] == ',')); ++j) {
						if (orig_exp[j] == '.' || orig_exp[j] == ',') {
							if (!dot) {
								dot = true;
								if (!((j + 1) < orig_exp.size() && orig_exp[j + 1] >= '0' && orig_exp[j + 1] <= '9')) { current_state = ERROR;  return false; }
							}
							else { current_state = ERROR;  return false; }
						}
						else if (!dot) {
							tmp = tmp * 10 + (orig_exp[j] - '0');
						}
						else {
							tmp = tmp + after_dot * (orig_exp[j] - '0');
							after_dot *= 0.1;
						}
					}
					k = j - 1;
					Lexer* num = new Lexer_real(tmp);
					arr[i].push_back(num);
					current_state = WAIT_FOR_OP;
				}
				else if (orig_exp[k] == '(') {
					brackets.push('(');
					//Lexer* op = new Lexer_operation(orig_exp[k]);

					arr[i].push_back(op_list["("]);
				}
				else if (orig_exp[k] == '-' || orig_exp[k] == '+') {
					if (k + 1 >= orig_exp.size()) { current_state = ERROR; k = orig_exp.size(); }
					else {
						if (orig_exp[k] == '-') {
							//Lexer* op = new Lexer_operation(op_un_min);
							arr[i].push_back(op_list["u-"]);
						}
						else {
							//Lexer* op = new Lexer_operation(op_un_plus);
							arr[i].push_back(op_list["u+"]);
						}
					}
				}
				else { current_state = ERROR; k = orig_exp.size(); }
			}
			//Закончен ввод числа, ожидание операции
			else if (current_state == WAIT_FOR_OP) {
				if (orig_exp[k] == '+' || orig_exp[k] == '-' || orig_exp[k] == '*' || orig_exp[k] == '/' || orig_exp[k] == '^') {
					Lexer* op = new Lexer_operation(orig_exp[k]);
					arr[i].push_back(op);
					int j = k + 1;
					if (j < orig_exp.size()) {
						if ((orig_exp[j] >= '0' && orig_exp[j] <= '9') || orig_exp[j] == '(') current_state = ORIGIN_STATE;
						else { current_state = ERROR; k = orig_exp.size(); }
					}
					else { current_state = ERROR; k = orig_exp.size(); }
				}
				else if (orig_exp[k] == ')') {
					//Lexer* op = new Lexer_operation(orig_exp[k]);
					arr[i].push_back(op_list[")"]);
					if (brackets.IsEmpty()) { current_state = ERROR; k = orig_exp.size(); }
					else if (brackets.top() == '(') brackets.pop();
					else { current_state = ERROR; k = orig_exp.size(); }
				}
				else { current_state = ERROR; k = orig_exp.size(); }
			}
		}
		if (current_state == ERROR || !(brackets.IsEmpty())) return false;
		else current_state = ORIGIN_STATE;
	}
	if (current_state == ORIGIN_STATE) current_state=CHECK_DONE;
	return current_state==CHECK_DONE;
}

real TFormula::calc()
{
	if (current_state != POSTFIX_DONE) return 0.0;
	real res = 0.0;
	if (post_arr.size() != 0) {
		TStack<Lexer_real*> stack;
		for (int k = 0; k < post_arr.size(); ++k) {
			for (int j = 0; j < post_arr[k].size(); ++j) {
				if (dynamic_cast<Lexer_real*>(post_arr[k][j])) stack.push(dynamic_cast<Lexer_real*>(post_arr[k][j]));
				else {
					Lexer_operation* op = dynamic_cast<Lexer_operation*> (post_arr[k][j]);
					if (op->code == op_un_min) stack.top()->a = -stack.top()->a;
					else if (op->code != op_un_plus) {
						real tmp = stack.top()->a;
						stack.pop();
						switch (op->code)
						{
						case op_plus:
							stack.top()->a += tmp;
							break;
						case op_minus:
							stack.top()->a -= tmp;
							break;
						case op_mult:
							stack.top()->a *= tmp;
							break;
						case op_div:
							stack.top()->a /= tmp;
							break;
						case op_pow:
							stack.top()->a = pow(stack.top()->a, tmp);
							break;
						default:
							break;
						}
					}
				}
			}
			res = stack.top()->a;
		}
	}
	return res;
}

void TFormula::show_lex()
{
	for (int k = 0; k < arr.size();++k) {
		for(int j=0;j<arr[k].size();++j) cout << *arr[k][j] << ' ';
		cout << endl;
	}
	cout << endl;
}

void TFormula::show_postfix()
{
	for (int k = 0; k < post_arr.size(); ++k) {
		for (int j = 0; j < post_arr[k].size(); ++j) cout << *post_arr[k][j] << ' ';
		cout << endl;
	}
	cout << endl;
}

bool Lexer_operation::operator==(char op)
{
	bool res;
	switch (op)
	{
	case '(':
		res= (code == open_bracket);
		break;
	case ')':
		res = (code == close_bracket);
		break;
	case '+':
		res = (code == op_plus);
		break;
	case '-':
		res = (code == op_minus);
		break;
	case '*':
		res = (code == op_mult);
		break;
	case '/':
		res = (code == op_div);
		break;
	case '^':
		res = (code == op_pow);
		break;
	default:
		res = false;
		break;
	}
	return res;
}

Lexer_operation::Lexer_operation (const char& op) {
	switch (op)
	{
	case '(':
		priority = 0;
		code = open_bracket;
		break;
	case ')':
		priority = 0;
		code = close_bracket;
		break;
	case '+':
		priority = 1;
		code = op_plus;
		break;
	case '-':
		priority = 1;
		code = op_minus;
		break;
	case '*':
		priority = 2;
		code = op_mult;
		break;
	case '/':
		priority = 2;
		code = op_div;
		break;
	case '^':
		priority = 3;
		code = op_pow;
		break;
	default:
		priority = -1;
		code = op_pow;
		throw exception();
		break;
	}
}

Lexer_operation::Lexer_operation(const operation_enum& op)
{
	code = op;
	if (op == open_bracket || op == close_bracket) priority = 0;
	else if (op == op_plus || op == op_minus) priority = 1;
	else if (op == op_mult || op == op_div) priority = 2;
	else if (op == op_pow) priority = 3;
	else priority = 4;
}

ostream& operator<<(ostream& out,const Lexer& lex)
{
	if (const Lexer_operation * op = dynamic_cast<const Lexer_operation*> (&lex))
		out << *op;
	else if (const Lexer_real * num = dynamic_cast<const Lexer_real*> (&lex))
		out << *num;
	return out;
}

ostream& operator<<(ostream& out, const Lexer_operation& op)
{
	switch (op.code)
	{
	case open_bracket:
		out << '(';
		break;
	case close_bracket:
		out << ')';
		break;
	case op_plus: op_un_plus:
		out << '+';
		break;
	case op_minus: op_un_min:
		out << '-';
		break;
	case op_mult:
		out << '*';
		break;
	case op_div:
		out << '/';
		break;
	case op_pow:
		out << '^';
		break;
	default:
		throw exception();
		break;
	}
	return out;
}

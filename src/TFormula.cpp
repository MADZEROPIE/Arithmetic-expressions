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
				else if (op->priority >  stack.top()->priority) stack.push(op);
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
	current_state = WAIT_FOR_INPUT;
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
	op = new Lexer_operation(op_exp);
	op_list.emplace("ln", op);
	op = new Lexer_operation(op_set);
	op_list.emplace("=", op);

}

TFormula::~TFormula()
{
	for(int i=0;i<arr.size();++i) 
		for (auto elem : arr[i]) 
			if(dynamic_cast<Lexer_real*>(elem) && (!(dynamic_cast<Lexer_var*>(elem) )))
				delete elem;
	for (auto it = name_list.begin(); it != name_list.end(); ++it) delete it->second;
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
		bool wait_for_num = false;
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
					wait_for_num = false;
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
				else if ((orig_exp[k] >= 'A' && orig_exp[k] <= 'Z') || (orig_exp[k] >= 'a' && orig_exp[k] <= 'z')) {
					string name=make_name(orig_exp,k);
					if (op_list.find(name) != op_list.end()) { arr[i].push_back(op_list[name]); wait_for_num = true; }
					else if (name_list.find(name) != name_list.end()) {
						arr[i].push_back(name_list[name]); current_state = WAIT_FOR_OP;
					}
					else if (k + 1 < orig_exp.size() && orig_exp[k+1] == '=') {
						Lexer* num = new Lexer_var(name);
						name_list.emplace(name, num);
						arr[i].push_back(num);
						arr[i].push_back(op_list["="]);
						++k;
					}
					else { current_state = ERROR; k = orig_exp.size(); }
				}
				else { current_state = ERROR; k = orig_exp.size(); }
			}
			//Закончен ввод числа, ожидание операции
			else if (current_state == WAIT_FOR_OP) {
				if (wait_for_num) {
					current_state = ERROR; k = orig_exp.size();
				}
				else if (orig_exp[k] == '+' || orig_exp[k] == '-' || orig_exp[k] == '*' || orig_exp[k] == '/' || orig_exp[k] == '^') {
					string op = { orig_exp[k] };
					//Lexer* op = new Lexer_operation(orig_exp[k]);
					arr[i].push_back(op_list[op]);
					int j = k + 1;
					if (j >= orig_exp.size()) { current_state = ERROR; k = orig_exp.size(); }
					else if (orig_exp[j] == '+' || orig_exp[j] == '-' || orig_exp[j] == '*' || orig_exp[j] == '/' || orig_exp[j] == '^') { current_state = ERROR; k = orig_exp.size(); }
					else current_state = ORIGIN_STATE;
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
		if (current_state == ERROR || !(brackets.IsEmpty()) || wait_for_num) {
			current_state = ERROR; return false;
		}
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
					if (op->code >= op_un_plus) {
						switch (op->code)
						{
						case op_un_min:
							stack.top()->a = -stack.top()->a;
							break;
						case op_exp:
							stack.top()->a = exp(stack.top()->a);
							break;
						case op_cos:
							stack.top()->a = cos(stack.top()->a);
							break;
						case op_sin:
							stack.top()->a = sin(stack.top()->a);
							break;
						case op_ln:
							if (stack.top()->a <= 0.0) throw exception();
							stack.top()->a = log(stack.top()->a);
							break;
						default:
							break;
						}
						
					}
					else {
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
							if (tmp == 0.0) throw exception();
							stack.top()->a /= tmp;
							break;
						case op_pow:
							stack.top()->a = pow(stack.top()->a, tmp);
							break;
						case op_set:
							stack.top()->a = tmp;
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

string TFormula::make_name(string a, int& j)
{
	string name;
	for (; j < a.size() && ((a[j] >= 'A' && a[j] <= 'Z') || (a[j] >= 'a' && a[j] <= 'z')); ++j) {
		name.push_back(a[j]);
	}
	--j;
	return name;
}

Lexer_operation::Lexer_operation(const operation_enum& op)
{
	code = op;
	if (op == open_bracket || op == close_bracket) priority = 0;
	else if(op==op_set) priority = 0;
	else if (op == op_plus || op == op_minus) priority = 2;
	else if (op == op_mult || op == op_div) priority = 3;
	else if (op == op_pow) priority = 4;
	else priority = 5;
}

ostream& operator<<(ostream& out,const Lexer& lex)
{
	if (const Lexer_operation * op = dynamic_cast<const Lexer_operation*> (&lex))
		out << *op;
	else if (const Lexer_real * var = dynamic_cast<const Lexer_var*> (&lex))
		out << *var;
	else if (const Lexer_real * num = dynamic_cast<const Lexer_real*> (&lex))
		out << *num;
	else throw exception();
	return out;
}

istream& operator<<(istream& inp, TFormula& form)
{
	while (!cin.eof()) {
		string arr;
		cin >> arr;
		form.orig_exp_arr.push_back(arr);
	}
	return inp;
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
	case op_cos:
		out << "cos";
		break;
	case op_sin:
		out << "sin";
		break;
	case op_exp:
		out << "exp";
		break;
	case op_ln:
		out << "ln";
		break;
	case op_set:
		out << "=";
		break;
	default:
		throw exception();
		break;
	}
	return out;
}


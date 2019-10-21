#include "TFormula.h"

void TFormula::make_postfix()
{

}

bool TFormula::check_exp()
{
	TStack<char> brackets;
	int i = 0;
	for (; i < orig_exp.size(); ++i) {
		// Состояние 0
		if (current_state == ORIGIN_STATE) {
			if (orig_exp[i] >= '0' && orig_exp[i] <= '9') {
				real tmp = (orig_exp[i] - '0');
				int j = i + 1; real after_dot = 0.1;
				for (bool dot = false; (j < orig_exp.size()) && ((orig_exp[j] >= '0' && orig_exp[j] <= '9') || (orig_exp[j] == '.' || orig_exp[j] == ',')); ++j) {
					if (orig_exp[j] == '.' || orig_exp[j] == ',') {
						if (!dot) {
							dot = true;
							if (!((j + 1) < orig_exp.size() && orig_exp[j + 1] >= '0' && orig_exp[j + 1] <= '9')) { current_state = ERROR;  return false; }
						}
						else { current_state = ERROR;  return false; }
					}
					else if (!dot) {
						tmp = tmp*10 + (orig_exp[j] - '0');
					}
					else {
						tmp = tmp + after_dot * (orig_exp[j] - '0');
						after_dot *= 0.1;
					}
				}
				i = j - 1;
				Lexer* num = new Lexer_real(tmp);
				arr.push_back(num);
				current_state = WAIT_FOR_OP;
			}
			else if (orig_exp[i] == '(') {
				brackets.push('(');
				Lexer* op = new Lexer_operation(orig_exp[i]);
				arr.push_back(op);
			}
			else if (orig_exp[i] == '-' || orig_exp[i] == '+') {
				int j = i + 1;
				if (j < orig_exp.size()) {
					if (orig_exp[j] >= '0' && orig_exp[j] <= '9') {
					real tmp = (orig_exp[j++]-'0');
					for (; (j < orig_exp.size()) && (orig_exp[j] >= '0' && orig_exp[j] <= '9'); ++j)
						tmp = tmp * 10 + orig_exp[j] - '0';
					i = j - 1;
					if (orig_exp[i] == '-') tmp = -tmp;
					Lexer* num = new Lexer_real(tmp);
					arr.push_back(num);
					}
					//else if(orig_exp[i] == '(')
				}
				else { current_state = ERROR; i = orig_exp.size(); }
			}
			else {current_state = ERROR; i = orig_exp.size();}
		}
		//Закончен ввод числа, ожидание операции
		else if (current_state == WAIT_FOR_OP) {
			if (orig_exp[i] == '+'|| orig_exp[i] == '-'|| orig_exp[i] == '*'|| orig_exp[i] == '/'|| orig_exp[i] == '^') {
				Lexer* op = new Lexer_operation(orig_exp[i]);
				arr.push_back(op);
				int j = i + 1;
				if (j < orig_exp.size()) {
					if((orig_exp[j] >= '0' && orig_exp[j] <= '9')|| orig_exp[j] == '(') current_state=ORIGIN_STATE;
					else { current_state = ERROR; i = orig_exp.size(); }
				}
				else { current_state = ERROR; i = orig_exp.size(); }
			}
			else if (orig_exp[i] == ')') {
				Lexer* op = new Lexer_operation(orig_exp[i]);
				arr.push_back(op);
				if (brackets.IsEmpty()) { current_state = ERROR; i = orig_exp.size(); }
				else if (brackets.top() == '(') brackets.pop();
				else { current_state = ERROR; i = orig_exp.size(); }
			}
			else { current_state = ERROR; i = orig_exp.size(); }
		}
	}
	if (current_state != ERROR && brackets.IsEmpty()) current_state = SUCCESS;
	return current_state==SUCCESS;
}

real TFormula::calc()
{
	return 0.0;
}

void TFormula::show()
{
	for (auto elem : arr) {
		cout << *elem << ' ';
	}
	cout << endl;
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
	case op_plus:
		out << '+';
		break;
	case op_minus:
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

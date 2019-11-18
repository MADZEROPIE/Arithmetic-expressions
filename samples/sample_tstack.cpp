#include <iostream>
#include "tstack.h"
#include "tqueue.h"
#include "TFormula.h"

bool check(char* arr) {
	TStack<char> stack;
	for (int i = 0; arr[i] != '\0'; ++i) {
		if (arr[i] == '{' || arr[i] == '(' || arr[i] == '[') stack.push(arr[i]);
		else if (arr[i] == '}' || arr[i] == ')' || arr[i] == ']') {
			if (stack.IsEmpty()) return false;
			if ((stack.top() == '{' && arr[i] == '}') ||
				(stack.top() == '(' && arr[i] == ')') ||
				(stack.top() == '[' && arr[i] == ']'))

					stack.pop();
			else return false;
		}
	}
	return true;
}




int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");
	//if (argc > 1) cout << check(argv[1])<<endl;
	//TStack_min<int> a;
	//a.push(3); a.push(7); a.push(3); a.push(2);
	//cout << a.find_min() << endl;
	//a.pop(); a.pop(); 
	//cout << a.find_min() << endl;
	//a.pop();
	//cout << a.find_min() << endl;
	TFormula a;
	std::cout << "Введите арифметические выражения: " << std::endl;
	std::cin >> a;
	a.check_exp();
	a.make_postfix();
	a.show_postfix();
	std::cout<<a.calc();
	return 0;
}


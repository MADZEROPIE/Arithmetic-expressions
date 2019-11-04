#include "TFormula.h"

#include <gtest.h>

TEST(TFormula, can_create_arifmetical_expression)
{
	ASSERT_NO_THROW(TFormula("11"));
}

TEST(TFormula, can_check_correct_arifmetical_expression_0)
{
	TFormula a("(9)+1");
	ASSERT_EQ(true, a.check_exp());
}

TEST(TFormula, can_check_correct_arifmetical_expression_1)
{
	TFormula a("(9+15)*10");
	ASSERT_EQ(true, a.check_exp());
}

TEST(TFormula, can_check_correct_arifmetical_expression_2)
{
	TFormula a("(9.631+15)*10/1089+3^7");
	ASSERT_EQ(true, a.check_exp());
}

TEST(TFormula, can_check_correct_arifmetical_expression_3)
{
	TFormula a("-10+2^(2*3)/72");
	ASSERT_EQ(true, a.check_exp());
}

TEST(TFormula, cant_check_incorrect_arifmetical_expression_1)
{
	TFormula a("(9.631.+15)*10/1089+3^7");
	ASSERT_EQ(false, a.check_exp());
}

TEST(TFormula, cant_check_incorrect_arifmetical_expression_2)
{
	TFormula a(")(9.631+15)*10/1089+3^7");
	ASSERT_EQ(false, a.check_exp());
}

TEST(TFormula, cant_check_incorrect_arifmetical_expression_3)
{
	TFormula a("1.13631+.15*10/1089+3^7");
	ASSERT_EQ(false, a.check_exp());
}

TEST(TFormula, cant_check_incorrect_arifmetical_expression_4)
{
	TFormula a("75+485/+4");
	ASSERT_EQ(false, a.check_exp());
}
TEST(TFormula, cant_check_incorrect_arifmetical_expression_5)
{
	TFormula a("cos");
	ASSERT_EQ(false, a.check_exp());
}

TEST(TFormula, cant_check_incorrect_arifmetical_expression_6)
{
	TFormula a("cos()");
	ASSERT_EQ(false, a.check_exp());
}

TEST(TFormula, cant_check_incorrect_arifmetical_expression_7)
{
	TFormula a("cos(12)+A");
	ASSERT_EQ(false, a.check_exp());
}

TEST(TFormula, can_calculate_simple_arifmetical_expression_1)
{
	TFormula a("75+405+4");
	a.check_exp();
	a.make_postfix();
	ASSERT_EQ(484,a.calc());
}

TEST(TFormula, can_calculate_simple_arifmetical_expression_2)
{
	TFormula a("10+(50+4)*2");
	a.check_exp();
	ASSERT_NO_THROW(a.make_postfix());
	ASSERT_EQ(118, a.calc());
}

TEST(TFormula, can_calculate_simple_arifmetical_expression_3)
{
	TFormula a("10+2^4");
	a.check_exp();
	ASSERT_NO_THROW(a.make_postfix());
	ASSERT_EQ(26, a.calc());
}

TEST(TFormula, can_calculate_simple_arifmetical_expression_4)
{
	TFormula a("10+2^(2*3)");
	ASSERT_NO_THROW(a.check_exp());
	ASSERT_NO_THROW(a.make_postfix());
	ASSERT_EQ(74, a.calc());
}

TEST(TFormula, can_calculate_simple_arifmetical_expression_5)
{
	TFormula a("10+2^(2*3)/72");
	a.check_exp();
	ASSERT_NO_THROW(a.make_postfix());
	double res = 10.0 + 64.0 / 72.0;
	ASSERT_DOUBLE_EQ(res, a.calc());
}

TEST(TFormula, can_calculate_simple_arifmetical_expression_6)
{
	TFormula a("-10.2+2^(2*3)/72");
	a.check_exp();
	ASSERT_NO_THROW(a.make_postfix());
	double res = (-10.2) + 64.0 / 72.0;
	ASSERT_DOUBLE_EQ(res, a.calc());
}

TEST(TFormula, can_calculate_simple_arifmetical_expression_7)
{
	TFormula a("-(-10.2)*5+2^(2*3)/72");
	a.check_exp();
	ASSERT_NO_THROW(a.make_postfix());
	double res = -(-10.2)*5 + 64.0 / 72.0;
	ASSERT_DOUBLE_EQ(res, a.calc());
}

TEST(TFormula, can_set_variable_1)
{
	vector<string> vec = { "res=3" };
	TFormula a(vec);
	a.check_exp();
	ASSERT_NO_THROW(a.make_postfix());
	double res = 3;
	ASSERT_NO_THROW(a.calc());
	ASSERT_DOUBLE_EQ(res, a["res"]);
}

TEST(TFormula, can_set_variable_2)
{
	vector<string> vec = { "res=-(-10.2)*5+2^(2*3)/72+cos(5)" };
	TFormula a(vec);
	a.check_exp();
	ASSERT_NO_THROW(a.make_postfix());
	double res = -(-10.2) * 5 + pow(2 , (2.0 * 3.0)) / 72.0 + cos(5);
	ASSERT_NO_THROW(a.calc());
	ASSERT_DOUBLE_EQ(res, a["res"]);
}

TEST(TFormula, can_set_variable_3)
{
	vector<string> vec = { "a=3","myb=7*2+a" };
	TFormula form(vec);
	form.check_exp();
	ASSERT_NO_THROW(form.make_postfix());
	double a = 3.0;
	double myb = 7.0 * 2.0 + a;
	ASSERT_NO_THROW(form.calc());
	ASSERT_DOUBLE_EQ(a, form["a"]);
	ASSERT_DOUBLE_EQ(myb, form["myb"]);

}

TEST(TFormula, can_set_variable_4)
{
	vector<string> vec = { "a=3","myb=7*2+a","cos(1+174)-a+myb^2" };
	TFormula form(vec);
	form.check_exp();
	ASSERT_NO_THROW(form.make_postfix());
	double a = 3.0;
	double myb = 7.0 * 2.0 + a;
	ASSERT_NO_THROW(form.calc());
	ASSERT_DOUBLE_EQ(a, form["a"]);
	ASSERT_DOUBLE_EQ(myb, form["myb"]);

}

TEST(TFormula, can_add_variable_1)
{
	vector<string> vec = { "res+3" };
	TFormula a(vec);
	double res = 4.0;
	a.add_var("res", 4.0);
	ASSERT_EQ(true, a.check_exp());
	ASSERT_NO_THROW(a.make_postfix());
	ASSERT_NO_THROW(a.calc());
	ASSERT_DOUBLE_EQ(4.0, a["res"]);
}

TEST(TFormula, can_calculate_difficult_arifmetical_expression_1)
{
	vector<string> vec = { "-(-10.2)*5+2^(2*3)/72","-(-10.2)*5+2^(2*3)/72" };
	TFormula a(vec);
	a.check_exp();
	ASSERT_NO_THROW(a.make_postfix());
	double res = -(-10.2) * 5 + 64.0 / 72.0;
	ASSERT_DOUBLE_EQ(res, a.calc());
}

TEST(TFormula, can_calculate_difficult_arifmetical_expression_2)
{
	vector<string> vec = { "cos(1)" };
	TFormula a(vec);
	ASSERT_EQ(true, a.check_exp());
	ASSERT_NO_THROW(a.make_postfix());
	double res = cos(1);
	ASSERT_DOUBLE_EQ(res, a.calc());
}

TEST(TFormula, can_calculate_difficult_arifmetical_expression_3)
{
	vector<string> vec = { "a=3","cos(1+174)-a" };
	TFormula a(vec);
	ASSERT_EQ(true, a.check_exp());
	ASSERT_NO_THROW(a.make_postfix());
	double res = cos(1+174)-3;
	ASSERT_DOUBLE_EQ(res, a.calc());
}

TEST(TFormula, can_calculate_difficult_arifmetical_expression_4)
{
	vector<string> vec = { "a=3","myb=7*2+a","cos(1+174)-a+myb^2" };
	TFormula a(vec);
	ASSERT_EQ(true, a.check_exp());
	ASSERT_NO_THROW(a.make_postfix());
	double res = cos(1 + 174) - 3+pow((7*2+3),2);
	ASSERT_DOUBLE_EQ(res, a.calc());
}

TEST(TFormula, can_calculate_difficult_arifmetical_expression_5)
{
	vector<string> vec = { "a=3","myb=7*2+a","cos(1+174)-a+myb^2+sin(2)+ln75" };
	TFormula a(vec);
	ASSERT_EQ(true, a.check_exp());
	ASSERT_NO_THROW(a.make_postfix());
	double res = cos(1 + 174) - 3 + pow((7 * 2 + 3), 2)+sin(2)+log(75);
	ASSERT_DOUBLE_EQ(res, a.calc());
}

TEST(TFormula, can_calculate_difficult_arifmetical_expression_6)
{
	vector<string> vec = { "a=2+(myb=3)","a+myb" };
	TFormula a(vec);
	ASSERT_EQ(true, a.check_exp());
	ASSERT_NO_THROW(a.make_postfix());
	double res = 8;
	ASSERT_DOUBLE_EQ(res, a.calc());
}
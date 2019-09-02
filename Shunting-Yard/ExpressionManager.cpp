/*
 * ExpressionManager.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: rdmclean
 */
#include "ExpressionManager.h"
using namespace std;

//ExpressionManager::ExpressionManager(){}
//ExpressionManager::~ExpressionManager(){}

int ExpressionManager::Precedence(string ch)
{
	if(ch == "+" || ch == "-")
			return 1;
		else if(ch == "*" || ch == "/" || ch == "%")
			return 2;
		else
			return 0;
}

bool ExpressionManager::is_op(string ch)
{
	if(ch == "+" || ch == "-"
			|| ch == "*" || ch == "/"
			|| ch == "%")
			return true;
		else
			return false;
}

bool ExpressionManager::is_int(string ch)
{
	for(int i = 0; i < ch.size(); i++)
		{
			if(isdigit(ch[i]) == false)
				return false;
		}
		return true;
}

bool ExpressionManager::is_open(string ch)
{ return OPEN.find(ch) != string::npos;}

bool ExpressionManager::is_close(string ch)
{ return CLOSE.find(ch) != string::npos;}

bool ExpressionManager::isBalanced(string expression)
{
	stringstream ss;
	ss << expression;
	string temp_s;
	stack<string> s;
	stack<string> n;
	int num_int = 0;
	int num_op = 0;
	while(ss.eof() == false)
	{
		ss >> temp_s;
	//	cout << "This time around " << temp_s << endl;
		if(is_open(temp_s))
		{//	cout << "Used open push" << endl;
			s.push(temp_s);
		}
		else if(is_close(temp_s))
		{ //cout << "Closed Parentheses\n";
			if(s.empty() == true) {return false;}
			if(CLOSE.find(temp_s) == OPEN.find(s.top()))
			{s.pop();}
			else {return false;}
		}
		else if(is_int(temp_s))
		{ //cout << "Found a Digit\n";
			num_int++;
			n.push(temp_s);
		}
		else if(is_op(temp_s))
		{
		//	cout << "Found an operator\n";
			num_op++;
			n.push(temp_s);
		}
		else
		{ //cout << "PROBLEM\n";
			return false;
		}
	}
	if(num_int > 0)
	{
	//	cout << "OPERATORS: " << num_op << " NUMBERS: " << num_int << endl;

		if((num_op + 1) == num_int){;}
		else {return false;}
	}
	if(s.empty()== false) {return false;}
	return true;
}

string ExpressionManager::postfixToInfix(string postfixExpression)
{
	if (isBalanced(postfixExpression) == false){return "invalid";}
	stringstream ss;
	ss << postfixExpression;
	string temp_s;
	stack<string> s;
	while(ss.eof() == false)
	{
		ss >> temp_s;
		if(is_op(temp_s) == true)
		{
			string temp_op = temp_s;
			if(s.empty()) {return "invalid";}
			string num2 = s.top();
			s.pop();
			if(s.empty()) {return "invalid";}
			string num1 = s.top();
			s.pop();
			string redone = "( " + num1 + " " + temp_op + " " + num2 + " )";
			s.push(redone);
		}
		else
		{
			s.push(temp_s);
		}
	}
	return s.top();
}

string ExpressionManager::infixToPostfix(string infixExpression)

{
	stringstream ss;
	ss << infixExpression;
//	cout << "Expression " << infixExpression << endl;
	if (isBalanced(infixExpression) == false)
	{
		return "invalid";
	}
	if (infixExpression == "-")
		return "invalid";

	string temp_s;
	stack<string> s;
	string final;
	while(ss.eof() == false)
	{	if(s.empty() != true)
	//	cout << "TOP " << s.top() << endl;
		ss >> temp_s;
	//	cout << "SYMBOL " << temp_s << endl;
		if(is_open(temp_s))
		{
	//		cout << "OPEN PARENTHESES\n";
			s.push(temp_s);
		} // Open parentheses on stack
		else if(is_close(temp_s)) // If Closed Parentheses
		{
	//		cout << "CLOSED PARENTHESES\n";
			while(is_open(s.top()) == false)
			{
				if(s.empty())
				{
					return "invalid";
				}
				final = final + " " + s.top();
				s.pop();
		//		cout << "NEW TOP " << s.top() << endl;
			}
			if(is_open(s.top()) == true)
			{
			//	cout << s.top() << endl;
				s.pop();
			}
		}
		else if(is_op(temp_s))
		{
		//	cout << "FOUND OP\n";
			if(s.empty() == true)
				s.push(temp_s);
			else if(Precedence(temp_s) > Precedence(s.top()))
			{
		//		cout << "PRECEDENCE GREATER\n";
				s.push(temp_s);
			}
			else if(Precedence(temp_s) <= Precedence(s.top()))
			{
		//		cout << "PRECEDENCE LESS\n";
				final = final + " " + s.top();
		//		cout << "I FOUND THE ERROR " << s.top() << endl;
				s.pop(); // THIS IS A BIG ERROR. WHY????
				if(Precedence(temp_s) <= Precedence(s.top()))
				{
					final = final + " " + s.top();
					s.pop();
				}
				s.push(temp_s);
			}
		}
		else if(is_int(temp_s))
		{
		//	cout << "INT FOUND\n";
			if(final == "")
				final += temp_s;
			else
				final = final + " " + temp_s;
		}
		else
		{
		//	cout << "JUST FOR FUN\n";
			return "invalid";
		}
	}
	while(s.empty() == false)
//	while(s.size() > 1)
	{
			final = final + " " + s.top();
		//	cout << s.top() << "TEST" << endl;
			s.pop();
	}

	//cout << "FINAL STRING " << final << endl;
	return final.substr(0,final.length()-1); // Returns the final string
}

string ExpressionManager::postfixEvaluate(string postfixExpression)
{
	stringstream ss;
	ss << postfixExpression;
	string temp_s;
	stack<string> s;
	int temp_1 = 0;
	int temp_2 = 0;
	int result = 0;
	string answer;
	string fun;
	string final;
	if (isBalanced(postfixExpression) == false)
		{
		//	cout << "BALANCE PROBLEM\n";
			return "invalid";
		}
	while(ss.eof() == false)
	{
		ss >> temp_s;
	//	cout << "SYMBOL: " << temp_s << endl;
		if(is_int(temp_s)) // put numbers on the stack
		{
			s.push(temp_s);
		}
		else if(is_op(temp_s))
		{
			if(s.empty() == true){return "invalid";}
		//	cout << "WHAT temp_2 SHOULD BE: " << s.top() << endl;
			fun = s.top();
		//	cout << s.top() << " " << fun << endl;
			s.pop();
			istringstream(fun) >> temp_2;
			if(s.empty() == true){return "invalid";}
		//	cout << "WHAT temp_1 SHOULD BE: " << s.top() << endl;
			fun = s.top();
		//	cout << s.top() << " " << fun << endl;
			s.pop();
			istringstream(fun) >> temp_1;
		//	cout << temp_1 << " " << temp_s << " " << temp_2 << endl;
			if(temp_s == "*")
			{
				result = temp_1 * temp_2;
			}
			else if(temp_s == "/")
			{
				if(temp_2 == 0){return "invalid";}
				result = temp_1 / temp_2;
			}
			else if(temp_s == "%")
			{
				if(temp_2 == 0){return "invalid";}
				result = temp_1 % temp_2;
			}
			else if(temp_s == "+")
			{
				result = temp_1 + temp_2;
			}
			else if(temp_s == "-")
			{
				result = temp_1 - temp_2;
			}
			else {return "invalid";}

			stringstream convert;
			convert << result;
			answer = convert.str();
		//	cout << "ANSWER: " << answer; // NOW WORKING CORRECTLY!!
			s.push(answer);
		//	cout << " " << s.top() << endl;
		}
	}
	while(s.empty() == false)
	{
		if(final == "")
			final += s.top();
		else
			final = final + " " + s.top();
		s.pop();
	}
	return final;

}

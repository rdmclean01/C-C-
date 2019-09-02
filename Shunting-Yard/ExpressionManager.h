/*
 * ExpressionManager.h
 *
 *  Created on: Oct 8, 2014
 *      Author: rdmclean
 */

#ifndef EXPRESSIONMANAGER_H_
#define EXPRESSIONMANAGER_H_
#include "ExpressionManagerInterface.h"
#include <stack>
#include <sstream>
using namespace std;


class ExpressionManager: public ExpressionManagerInterface
{
	public:


		ExpressionManager(){} // what needs to go here?
		virtual ~ExpressionManager(){}

		// The set of opening parentheses
		const string OPEN = "([{";
		// The Corresponding Set of closing parentheses
		const string CLOSE = ")]}";
		// The operators
	//	const string OPERATE = "+-*/%";
		// The precedence for everything
	//	const int PRECEDENCE[] = { 1, 1, 2, 2, 2 }

int Precedence(string ch);

bool is_op(string ch);

bool is_int(string ch);
		/** Function to determine whether a character is one the the opening
		 * parentheses (defined in constant OPEN).
		 *  @ param ch Character to be tested
		 *  @return true If the character is an opening parenthesis
		 */
bool is_open(string ch);

/**Function to determine whether a character is one of the closing
 * parentheses (defined in constant OPEN).
 * @param ch Character to be tested
 * @return true If the character is a closing parenthesis
 */
bool is_close(string ch);

/*
	* Checks whether an expression is balanced on its parentheses
	*
	* - The given expression will have a space between every number or operator
	*
	* @return true if expression is balanced
	* @return false otherwise
	*/
bool isBalanced(string expression); // PART 1


	/**
	 * Converts a postfix expression into an infix expression
	 * and returns the infix expression.
	 *
	 * - The given postfix expression will have a space between every number or operator.
	 * - The returned infix expression must have a space between every number or operator.
	 * - Redundant parentheses are acceptable i.e. ( ( 3 * 4 ) + 5 ).
	 * - Check lab requirements for what will be considered invalid.
	 *
	 * return the string "invalid" if postfixExpression is not a valid postfix expression.
	 * otherwise, return the correct infix expression as a string.
	 */
string postfixToInfix(string postfixExpression); //PART 3


	/*
	 * Converts an infix expression into a postfix expression
	 * and returns the postfix expression
	 *
	 * - The given infix expression will have a space between every number or operator.
	 * - The returned postfix expression must have a space between every number or operator.
	 * - Check lab requirements for what will be considered invalid.
	 *
	 * return the string "invalid" if infixExpression is not a valid infix expression.
	 * otherwise, return the correct postfix expression as a string.
	 */
string infixToPostfix(string infixExpression); // PART 2

	/*
	 * Evaluates a postfix expression returns the result as a string
	 *
	 * - The given postfix expression will have a space between every number or operator.
	 * - Check lab requirements for what will be considered invalid.
	 *
	 * return the string "invalid" if postfixExpression is not a valid postfix Expression
	 * otherwise, return the correct evaluation as a string
	 */
string postfixEvaluate(string postfixExpression); // PART 4

	private:



};





#endif /* EXPRESSIONMANAGER_H_ */

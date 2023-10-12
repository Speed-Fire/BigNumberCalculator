/**
* @file Evaluator.h
* File with description of class Evaluator
*/

#pragma once

#include <queue>
#include <stack>
#include <string>
#include <map>
#include <list>

#include "BigDouble.h"

/// 
/// \class Evaluator
/// \brief Evaluate math expressions.
/// 
/// Receives a string expression, evaluate it and return result.
/// Only one object of this class can exist.
/// 
class Evaluator
{
	/// 
	/// \enum TokenType
	/// Represents enumeration of token types for
	/// reverse polish notation.
	/// 
	enum class TokenType
	{
		number = 0,
		constant,
		operation,
		function,
		variable,
		openBracket,
		closeBracket,
		error
	};

	/// \var prohibitedVarNames 
	/// The list with prohibited variable names
	std::vector<std::string> prohibitedVarNames;

	/// \var variables
	/// The list with variables
	std::map<std::string, IBigNum_ptr> variables;

	/// \var constants
	/// The list with constants
	std::map<std::string, IBigNum_ptr> constants;

	/// \var outputQ
	/// Contains all tokens ordered in reverse polish notation.
	std::queue<std::pair<std::string, TokenType>> outputQ;

	/// \var funcStack
	/// Temporary stack for containing operation and bracket tokens.
	std::stack<std::pair<std::string, TokenType>> funcStack;

	/// \var Current index of symbol in processing string.
	/// @see Evaluate
	size_t iter;

	/// \var Position where evaluator starts considering processing string as math expression.
	/// @see Evaluate
	size_t start;

	/// Standard constructor.
	Evaluator();

	/// Initializes some constants to constants map.
	void initConstants();

	/// Checks if variable name satisfying all rules and
	/// prepares it for using (delete spaces in front and in back).
	/// @param[in, out] name variable name.
	/// @return empty string if all's alright, else string with explicit error.
	std::string checkVarName(std::string& name) const;

	// making RPN methods

	/// Checks if symbol satisfying all rules.
	/// @param symb symbol to be checked.
	/// @return true if all's alright, otherwise false.
	bool checkSymbol(const char symb) const;

	/// Reads token from processing string.
	/// Uses class member iter as iterator in string.
	/// @see Evaluate
	/// @param[in] str processing string. 
	/// @return token and its type.
	std::pair<std::string, TokenType> readToken(const std::string& str);

	/// Check operator's priority.
	/// @param[in] op operator.
	/// @return operator's priority.
	int getOperatorPriority(const std::string& op) const;

	// evaluating RPN methods

	/// Makes big number from string.
	/// @param[in] res string to be converted.
	/// @return pointer to big number.
	IBigNum_ptr makeBigNum(const std::string& res);

	/// Computes two big numbers with specified operator.
	/// @param num1 pointer to first big number.
	/// @param num2 pointer to second big number.
	/// @param[in] op operator.
	/// @return pointer to resulting big number.
	IBigNum_ptr compute(IBigNum_ptr num1, IBigNum_ptr num2,const std::string& op);

	// evaluating methods

	/// Converts math expression to reverse polish notation.
	/// @param[in] str processing string.
	/// @return empty string if all's alright, otherwise string with error.
	std::string makeRPN(const std::string& str);

	/// Evaluates reverse polish notation from outputQ.
	/// @return pointer to resulting big number
	IBigNum_ptr evalRPN();

	/// Resets outputQ, funcStack and iter.
	void reset();

public:

	Evaluator(const Evaluator&) = delete;
	void operator=(const Evaluator&) = delete;

	/// Create or get already existing instance of Evaluator
	/// @return evaluator instance
	static Evaluator& getInstance()
	{
		static Evaluator instance;
		return instance;
	}

	/// Evaluates expression.
	/// @param[in] str processing string.
	/// @param[out] isError flag for errors.
	/// @return result of evaluating or string with error.
	std::string Evaluate(const std::string& str, bool& isError);

	/// Makes list with variables names and its values.
	/// @return list.
	std::list<std::pair<std::string, IBigNum_ptr>> getAllVars() const;

	/// Deletes all variables.
	void clearAllVars();
};
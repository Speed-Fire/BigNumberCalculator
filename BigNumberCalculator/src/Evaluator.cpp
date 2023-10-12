#include "../hdrs/Evaluator.h"

#include <iostream>
#include <stdexcept>

void Evaluator::initConstants()
{
	constants.emplace("pi", makeBigNum("3.1415926535897932"));
	constants.emplace("e", makeBigNum("2.7182818284590452"));
}

std::string Evaluator::checkVarName(std::string& name) const
{
	while (!name.empty() && isspace(name[0]))
		name.erase(name.begin());

	while (!name.empty() && isspace(name.back()))
		name.erase(name.size() - 1);

	if (name.find_first_of(" \n\t/*()+-") != std::string::npos ||
		name.empty() ||
		isdigit(name[0]))
		return "Incorrect variable name!!!";

	for (auto& ch : name)
		if (!checkSymbol(ch))
			return "Incorrect variable name!!!";

	for (auto& nm : prohibitedVarNames)
		if (nm == name)
			return "Prohibited variable name!!!";

	if (constants.find(name) != constants.end())
		return "Prohibited variable name!!!";

	return "";
}

bool Evaluator::checkSymbol(const char symb) const
{
	if ((symb < '0' || symb > '9') &&
		(symb < 'a' || symb > 'z') &&
		(symb < 'A' || symb > 'Z') &&
		symb != '+' &&
		symb != '-' &&
		symb != '/' &&
		symb != '%' &&
		symb != '*' &&
		symb != '(' &&
		symb != ')')
		return false;

	return true;
}

std::pair<std::string, Evaluator::TokenType> Evaluator::readToken(const std::string& str)
{
	if (iter >= str.size())
		return { "", Evaluator::TokenType::number };

	if (!checkSymbol(str[iter]))
		return { "Incorrect symbol!!!", Evaluator::TokenType::error };

	bool firstNegNum = false;
	if (iter == start && str[iter] == '-')
	{
		firstNegNum = true;
		iter++;
		while (isspace(str[iter])) iter++;
	}

	size_t dotsCount = 0;
	std::string numb, op;

	switch (str[iter])
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
		if (iter == start)
			return { "Missing operand!!!", Evaluator::TokenType::error };

		op.push_back(str[iter++]);
		return { op, Evaluator::TokenType::operation };
	case '(':
		iter++;
		return { "(", Evaluator::TokenType::openBracket };
	case ')':
		iter++;
		return { ")", Evaluator::TokenType::closeBracket };
	case'0':
	case'1':
	case'2':
	case'3':
	case'4':
	case'5':
	case'6':
	case'7':
	case'8':
	case'9':
		if (firstNegNum)
			numb.push_back('-');

		for (; iter < str.size(); iter++)
		{
			if (str[iter] == '.') dotsCount++;

			if(dotsCount > 1)
				return { "Too much dots in number!!!", Evaluator::TokenType::error };

			if ((str[iter] < '0' || str[iter] > '9') &&
				str[iter] != '.')
			{
				if (str[iter] != '+' &&
					str[iter] != '-' &&
					str[iter] != '*' &&
					str[iter] != '/' &&
					str[iter] != '%' &&
					str[iter] != ')' &&
					!isspace(str[iter]))
					return { "Incorrect number!!!", Evaluator::TokenType::error };
				else
					break;
			}
			numb += str[iter];
		}

		return { numb, Evaluator::TokenType::number };
	default:
		std::string sth;
		for (; iter < str.size(); iter++)
		{
			if ((str[iter] < 'a' || str[iter] > 'z') && 
				(str[iter] < 'A' || str[iter] > 'Z') &&
				(str[iter] < '0' || str[iter] > '9'))
				break;
			sth += str[iter];
		}

		if (constants.find(sth) != constants.end())
			return { sth, Evaluator::TokenType::constant };

		return { sth, Evaluator::TokenType::variable };
	}

	return { "", Evaluator::TokenType::number };
}

int Evaluator::getOperatorPriority(const std::string& op) const
{
	if (op == "/" || op == "*" || op == "%")
		return 2;
	if (op == "+" || op == "-")
		return 1;
	return 0;
}

IBigNum_ptr Evaluator::makeBigNum(const std::string& res)
{
	IBigNum_ptr num;

	if (res.find('.') == std::string::npos)
		num.reset(new BigInt(res));
	else
		num.reset(new BigDouble(res));

	return num;
}

IBigNum_ptr Evaluator::compute(IBigNum_ptr num1, IBigNum_ptr num2,const std::string& op)
{
	if (op == "+")
		return (*num1) + num2;
	else if (op == "-")
		return (*num1) - num2;
	else if (op == "*")
		return (*num1) * num2;
	else if (op == "/")
		return (*num1) / num2;
	else if (op == "%")
		return (*num1) % num2;

	return IBigNum_ptr();
}

std::string Evaluator::makeRPN(const std::string& str)
{
	int count = 0;
	bool lastOperand = false;
	bool firstSpaceCutting = true;
	bool firstStep = true;

	// Shunting Yard algorithm
	while (true)
	{
		while (iter < str.size() && isspace(str[iter]))
		{
			iter++;
			if (firstSpaceCutting)
				start++;
		}
		firstSpaceCutting = false;

		auto token = readToken(str);

		if (firstStep &&
			token.second != Evaluator::TokenType::openBracket &&
			token.second != Evaluator::TokenType::variable &&
			token.second != Evaluator::TokenType::constant &&
			token.second != Evaluator::TokenType::number)
		{
			reset();
			return "Incorrect begin of expression!!!";
		}
		firstStep = false;

		if (token.first.empty())
		{
			if (count)
			{
				reset();
				return "Incorrect expression!!!";
			}

			while (!funcStack.empty())
			{
				if (funcStack.top().second == Evaluator::TokenType::openBracket)
				{
					reset();

					return "Missing a close bracket!!!";
				}

				outputQ.push(funcStack.top());
				funcStack.pop();
			}

			break;
		}

		if (lastOperand &&
			token.second != Evaluator::TokenType::closeBracket &&
			token.second != Evaluator::TokenType::operation &&
			token.second != Evaluator::TokenType::error)
		{
			reset();
			return "Missing operation!!!";
		}

		switch (token.second)
		{
		case Evaluator::TokenType::variable:
		case Evaluator::TokenType::constant:
			if (count == 2)
			{
				reset();
				return "Incorrect expression!!!";
			}
			outputQ.push(token);
			count = 0;
			lastOperand = true;
			break;
		case Evaluator::TokenType::number:
			if (count == 2)
			{
				funcStack.pop();
				token.first.insert(0, "-");
			}
			outputQ.push(token);
			count = 0;
			lastOperand = true;
			break;
		case Evaluator::TokenType::function:
		case Evaluator::TokenType::openBracket:
			funcStack.push(token);
			break;
		case Evaluator::TokenType::operation:
			if (count == 1 && token.first != "-")
			{
				reset();
				return "Incorrect expression!!!";
			}

			while (!funcStack.empty() && count == 0 &&
				funcStack.top().second == Evaluator::TokenType::operation &&
				getOperatorPriority(funcStack.top().first) >= getOperatorPriority(token.first))
			{
				outputQ.push(funcStack.top());
				funcStack.pop();
			}
			count++;
			funcStack.push(token);
			lastOperand = false;
			break;
		case Evaluator::TokenType::closeBracket:
			if (count)
			{
				reset();
				return "Incorrect expression!!!";
			}

			if (funcStack.empty())
			{
				reset();

				return "Missing an open bracket!!!";
			}

			while (!funcStack.empty() &&
				funcStack.top().second != Evaluator::TokenType::openBracket)
			{
				outputQ.push(funcStack.top());
				funcStack.pop();

				if (funcStack.empty())
				{
					reset();

					return "Missing an open bracket!!!";
				}
			}

			funcStack.pop();
			lastOperand = true;

			if (!funcStack.empty() &&
				funcStack.top().second == Evaluator::TokenType::function)
			{
				outputQ.push(funcStack.top());
				funcStack.pop();
			}
			break;
		case Evaluator::TokenType::error:
			reset();
			return token.first;
		}
	}

	return "";
}

IBigNum_ptr Evaluator::evalRPN()
{
	std::stack<IBigNum_ptr> nums;

	while (!outputQ.empty())
	{
		switch (outputQ.front().second)
		{
		case Evaluator::TokenType::constant:
			nums.push(constants[outputQ.front().first]);
			break;
		case Evaluator::TokenType::variable:
			if (variables.find(outputQ.front().first) == variables.end())
				throw std::runtime_error("Unknown variable!!!");

			nums.push(variables[outputQ.front().first]);
			break;
		case Evaluator::TokenType::number:
			nums.push(makeBigNum(outputQ.front().first));
			break;
		case Evaluator::TokenType::operation:
		{
			IBigNum_ptr num2 = nums.top();
			nums.pop();
			IBigNum_ptr num1 = nums.top();
			nums.pop();

			nums.push(compute(num1, num2, outputQ.front().first));
		}
			break;
		default:
			break;
		}

		outputQ.pop();
	}

	return nums.top();
}

void Evaluator::reset()
{
	outputQ = std::queue<std::pair<std::string, Evaluator::TokenType>>();
	funcStack = std::stack<std::pair<std::string, Evaluator::TokenType>>();
	iter = 0;
}

Evaluator::Evaluator()
	:
	iter(0)
{
	initConstants();

	prohibitedVarNames.push_back("cls");
	prohibitedVarNames.push_back("test");
	prohibitedVarNames.push_back("exit");
	prohibitedVarNames.push_back("clv");
	prohibitedVarNames.push_back("clh");
	prohibitedVarNames.push_back("load");
	prohibitedVarNames.push_back("save");
	prohibitedVarNames.push_back("setPath");
	prohibitedVarNames.push_back("getPath");
}

std::string Evaluator::Evaluate(const std::string& str, bool& isError)
{
	isError = false;

	// Check for variable assigning
	std::string varName;

	if (size_t tmp = str.find('='); tmp != std::string::npos)
	{
		varName = str.substr(0, tmp);
		if (std::string er = checkVarName(varName); er != "")
		{
			isError = true;
			reset();
			return er;
		}

		iter = tmp + 1;
	}
	start = iter;

	if (std::string r = makeRPN(str); r != "")
	{
		isError = true;
		reset();
		return r;
	}

	if (varName != "" && outputQ.empty())
	{
		isError = true;
		reset();
		return "Missing expression!!!";
	}

	// Calculating
	IBigNum_ptr res;

	try
	{
		res = evalRPN();
	}
	catch (std::runtime_error& ex)
	{
		reset();
		isError = true;
		return ex.what();
	}

	reset();

	// Variable assignment or output
	if (!varName.empty())
	{
		if (variables.find(varName) != variables.end())
			variables[varName] = res;
		else
			variables.emplace(varName, res);

		return "";
	}
	else
		return res->toString();
}

std::list<std::pair<std::string, IBigNum_ptr>> Evaluator::getAllVars() const
{
	std::list<std::pair<std::string, IBigNum_ptr>> res;

	for (auto& var : variables)
		res.push_back(var);

	return res;
}

void Evaluator::clearAllVars()
{
	variables.clear();
}

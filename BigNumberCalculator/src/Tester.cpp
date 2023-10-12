#include "../hdrs/Tester.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>

Tester::Tester()
{
#ifndef __LINUX_COMPILATION__
	filePath = "examples.tst";
#else
	filePath = "examples/examples.tst";
#endif
}

bool Tester::readData(std::vector<std::pair<std::string, std::string>>& exprs)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cout << pref << "File can't be opened!" << std::endl;
		return true;
	}

	std::pair<std::string, std::string> p;

	size_t iter = 0;
	int fl = 0;
	std::string tmp;

	while (std::getline(file, tmp))
	{
		if (tmp.empty() || isspace(tmp))
		{
			iter -= -1;
			continue;
		}

		if (fl % 2 == 0)
		{
			p.first = tmp;
			fl++;
		}
		else
		{
			while (isspace(tmp[0])) tmp.erase(tmp.begin());

			if (starts_with(tmp, "var "))
			{
				if (toWords(tmp).size() != 3)
				{
					std::cout << pref << "Row " << iter << ": expected variable result is incorrect!" << std::endl;
					file.close();
					return true;
				}
			}
			else if (toWords(tmp).size() != 1)
			{
				std::cout << pref << "Row " << iter << ": expression is not a single number!" << std::endl;
				file.close();
				return true;
			}

			p.second = tmp;
			exprs.push_back(p);
			fl--;
		}
		iter++;
		tmp = "";
	}

	file.close();
	return false;
}

size_t Tester::testData(std::vector<std::pair<std::string, std::string>>& exprs)
{
	size_t erCount = 0;

	for (auto& expr : exprs)
	{
		bool er = false;
		std::string res;

		std::string answ = expr.second;

		if (starts_with(answ, "var "))
		{
			res = Evaluator::getInstance().Evaluate(expr.first, er);

			if (!er)
			{
				auto words = toWords(answ);

				if (Evaluator::getInstance().Evaluate(words[1], er).compare(words[2]) == 0) continue;
			}
		}
		else
		{
			res = Evaluator::getInstance().Evaluate(expr.first, er);
			if (res.compare(answ) == 0) continue;
		}

		std::cout << pref << "Answer doesn't match expected answer:" << std::endl;
		std::cout << "\tExpr: " << expr.first << std::endl;
		std::cout << "\tExpected answ: " << answ << std::endl;
		std::cout << "\tAnsw         : " << res << std::endl << std::endl;

		erCount++;
	}

	return erCount;
}

bool Tester::starts_with(const std::string& str, const std::string& pref) const
{
	return (str.size() >= pref.size()) && (str.compare(0, pref.size(), pref) == 0);
}

Tester& Tester::getInstance()
{
	static Tester test;
	return test;
}

void Tester::testProgram()
{
	std::vector<std::pair<std::string, std::string>> expressions;

	if (readData(expressions)) 
	{
		std::cout << pref << "Test finished." << std::endl;
		return;
	}

	size_t errs = testData(expressions);

	std::cout << pref << expressions.size() - errs << " of " << expressions.size() << " are successful." << std::endl;
	std::cout << pref << "Test finished." << std::endl;

	Evaluator::getInstance().clearAllVars();
}

std::vector<std::string> toWords(const std::string& sentence)
{
	std::vector<std::string> words;

	std::stringstream tmp;
	tmp << sentence;

	std::string tmp2;
	while (tmp >> tmp2)
		words.push_back(tmp2);

	return words;
}

bool isspace(std::string& str)
{
	for (auto ch : str)
		if (!isspace(ch))
			return false;
	return true;
}

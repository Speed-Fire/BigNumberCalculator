#include "../hdrs/Historizer.h"
#include "../hdrs/Evaluator.h"

#include <fstream>
#include <iostream>
#include <functional>
#include <sstream>

std::string Historizer::saveVars()
{
	std::string res = "";
	auto vars = Evaluator::getInstance().getAllVars();
	for (auto& var : vars)
	{
		res += var.first + "=" + var.second->toString() + "\n";
	}
	res += ";\n";

	return res;
}

std::string Historizer::saveExprs()
{
	std::string res;
	for (auto& el : history)
		res += el + "\n";
	return res;
}

Historizer::Historizer()
{
#ifndef __LINUX_COMPILATION__
	filePath = "calc.hist";
#else
	filePath = "examples/calc.hist";
#endif // !__LINUX_COMPILATION__
}

Historizer& Historizer::getInstance()
{
	static Historizer a;
	return a;
}

void Historizer::clearCurrentHistory()
{
	history.clear();
}

void Historizer::save()
{
	std::ofstream f(filePath);
	std::stringstream ss;
	if (f.is_open())
	{
		ss << saveVars();
		ss << saveExprs();

		std::hash<std::string> hasher;
		size_t _hash = hasher(ss.str());

		f << _hash << "\n" << ss.str();

		f.close();
	}
}

void Historizer::load()
{
	std::ifstream f(filePath);
	std::stringstream ss;

	std::string pref = "hist:~# ";
	std::string tmp;

	if (f.is_open())
	{
		std::getline(f, tmp);
		ss << tmp;
		size_t _expHash = 0;
		ss >> _expHash;
		ss.clear();
		ss.str("");

		while (std::getline(f, tmp), tmp != "")
			ss << tmp << "\n";

		std::hash<std::string> hasher;
		size_t _hash = hasher(ss.str());

		if (_expHash != _hash)
		{
			std::cout << pref << "The file was corrupted!!!" << std::endl;
			return;
		}

		clearCurrentHistory();
		Evaluator::getInstance().clearAllVars();

		bool er = false;
		while (std::getline(ss, tmp), tmp != ";")
			Evaluator::getInstance().Evaluate(tmp, er);

		while (std::getline(ss, tmp), tmp != "")
		{
			addExpression(tmp);

			size_t k = tmp.find(':');
			if (k == std::string::npos)
			{
				std::cout << pref << tmp << std::endl;
				continue;
			}

			std::cout << pref << tmp.substr(0, k) << std::endl;
			std::cout << pref << tmp.substr(k + 1, tmp.length() - 1) << std::endl;
		}

		f.close();
	}
}

void Historizer::addExpression(const std::vector<std::string>& words, const std::string& answer)
{
	std::string res;
	for (auto& word : words)
		res += word + " ";
	if (answer != "")
		res += ":" + answer;

	history.push_back(res);
}

void Historizer::addExpression(const std::string& expr)
{
	history.push_back(expr);
}

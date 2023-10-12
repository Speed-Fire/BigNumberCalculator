/**
* @file Calculator_main.cpp
* The main file of the application, where all actions are producing.
*/

#ifndef __LINUX_COMPILATION__
#pragma once
#include "../hdrs/Calculator_main.h"

/// Alias to clear screen command.
#define __MY__CLS__ "cls"
#else
#define __MY__CLS__ "clear"
#endif

#include "../hdrs/Tester.h"
#include "../hdrs/Historizer.h"
#include "../hdrs/Evaluator.h"

#include <stdlib.h>
#include <iostream>

#ifndef __LINUX_COMPILATION__
#include <assert.h>
/// Tests class BigInt
void BigInt_test();

/// Tests class BigDouble
void BigDouble_test();

/// Tests classes BigInt and BigDouble gotten as IBigNum
void BigNumber_test();

/// Tests class Evaluator
void Evaluator_test();
#endif

/// Prints welcoming text of application.
void printWelcome();

/// Handles given expression.
/// @param[in] command expression.
void commandHandler(const std::string& command);

/// Tests for almost all my classes.
/// If __LINUX__COMPILATION__ defined does nothing.
void test();

/// The main function, where everything begins.
#ifdef __LINUX_COMPILATION__
int main()
#else
int Calculator()
#endif
{
	test();
	system(__MY__CLS__);

	printWelcome();
	std::cout << "calc:~# " << std::flush;

	std::string command;
	while (std::getline(std::cin, command), command != "exit")
	{
		commandHandler(command);
		std::cout << "calc:~# " << std::flush;
		command = "";
	}
	system(__MY__CLS__);

	return 0;
}

void printWelcome()
{
	std::cout << "\t\tWelcome to my super calculator!" << std::endl;
	std::cout << "\tWith it you can calculate almost any math expressions!" << std::endl;
	std::cout << "  Here are some useful commands:" << std::endl;
	std::cout << "   load - load calculator history" << std::endl;
	std::cout << "   save - save calculator history" << std::endl;
	std::cout << "   cls - clear screen" << std::endl;
	std::cout << "   clv - clear all variables" << std::endl;
	std::cout << "   clh - clear current history" << std::endl;
	std::cout << "   test - test file examples.tst" << std::endl;
	std::cout << "   exit - exit from calculator" << std::endl;
	std::cout << "  All another sentences are considered as math expressions!" << std::endl << std::endl;
}

void commandHandler(const std::string& command)
{
	std::vector<std::string> words = toWords(command);

	if (words.empty()) return;

	if (words[0] == "load" && words.size() == 1)
	{
		// load history from file
		system(__MY__CLS__);
		printWelcome();

		Historizer::getInstance().load();
	}
	else if (words[0] == "save" && words.size() == 1)
	{
		// save history to file
		Historizer::getInstance().save();
	}
	else if (words[0] == "cls" && words.size() == 1)
	{
		// clear screen
		system(__MY__CLS__);
		printWelcome();
	}
	else if (words[0] == "clv" && words.size() == 1)
	{
		// clear all variables
		Evaluator::getInstance().clearAllVars();
		Historizer::getInstance().addExpression("clv");
	}
	else if (words[0] == "clh" && words.size() == 1)
	{
		// clear current history
		Historizer::getInstance().clearCurrentHistory();
	}
	else if (words[0] == "test" && words.size() == 1)
	{
		Tester::getInstance().testProgram();
	}
	else
	{
		// evaluate
		bool error = false;
		std::string res = Evaluator::getInstance().Evaluate(command, error);

		if (!error)
		{
			// save the sentence to history
			Historizer::getInstance().addExpression(words, res);
		}

		if (res != "")
			std::cout << "eval:~# " << res << std::endl;
	}
}

// Components testing

void test()
{
#ifndef __LINUX_COMPILATION__
	BigInt_test();
	BigDouble_test();
	BigNumber_test();

	Evaluator_test();
#endif
}

#ifndef __LINUX_COMPILATION__
void BigInt_test()
{
	std::shared_ptr<IBigNum> a;
	std::shared_ptr<IBigNum> b;
	a.reset(new BigInt("100"));
	b.reset(new BigInt("200"));

	// Addition and subtraction

	assert(((*a) + b)->toString() == "300");
	assert(((*a) - b)->toString() == "-100");
	assert(((*b) - a)->toString() == "100");

	*a = "4294967295";
	*b = "1";

	assert(((*a) + b)->toString() == "4294967296");

	*a = "34536092734827374";
	*b = "17495534520689";

	assert(((*a) + b)->toString() == "34553588269348063");

	*a = "9346197394502902461";
	*b = "193756834507346";
	assert(((*a) - b)->toString() == "9346003637668395115");

	*b = "-193756834507346";
	assert(((*a) + b)->toString() == "9346003637668395115");
	assert(((*a) - b)->toString() == "9346391151337409807");

	*a = "9346391151337409807";
	*b = "9346391151337409808";
	assert(((*a) - b)->toString() == (std::string)"-1");

	
	*a = "10294579475639";
	*b = "920836657234957645";
	assert(((*a) - b)->toString() == "-920826362655482006");

	*a = "298234857623546849576937456";
	*b = "935875672478340856375346904";
	assert(((*a) + b)->toString() == "1234110530101887705952284360");

	*a = "5440042";
	*b = "1";
	assert(((*a) + b)->toString() == "5440043");
	assert(((*b) + a)->toString() == "5440043");

	// Multiplication

	*a = "23";
	*b = "10";
	assert(((*a) * b)->toString() == "230");

	*b = "4294967295";
	assert(((*a) * b)->toString() == "98784247785");

	*a = "9346197394502902461";
	*b = "193756834507346";
	assert(((*a) * b)->toString() == "1810889621839687247965638025978506");

	*a = "343568980234068356";
	*b = "8920475";
	assert(((*a) * b)->toString() == "3064798498953500917989100");

	*a = "4567998345";
	*b = "10000000000000000000000";
	assert(((*a) * b)->toString() == "45679983450000000000000000000000");

	*a = "0";
	*b = "394502384296587943508467539475";
	assert(((*a) * b)->toString() == "0");

	*a = "-8928347858356002834";
	*b = "79274910239957845";
	assert(((*a) * b)->toString() == "-707793975062291984126566622860532730");

	*a = "-2783498683645";
	*b = "-9008236478";
	assert(((*a) * b)->toString() == "25074414378475871002310");

	// Integer division

	*a = "25";
	*b = "6";
	assert(((*a) / b)->toString() == "4");
	assert(((*a) % b)->toString() == "1");

	*a = "34580734568356452";
	*b = "86486";
	assert(((*a) / b)->toString() == "399841992557");
	assert(((*a) % b)->toString() == "71750");

	*a = "983548552859375095684873";
	*b = "75493619375";
	assert(((*a) / b)->toString() == "13028234187233");
	assert(((*a) % b)->toString() == "44509245498");

	*a = "97253648625635836485";
	*b = "458374562654356";
	assert(((*a) / b)->toString() == "212170");
	assert(((*a) % b)->toString() == "317667261123965");

	*a = "235496930583765739563745";
	*b = "7355902745275";
	assert(((*a) / b)->toString() == "32014687896");
	assert(((*a) % b)->toString() == "457025872345");

	*a = "548304856923530545734";
	*b = "186459345865";
	assert(((*a) / b)->toString() == "2940613431");
	assert(((*a) % b)->toString() == "137437232919");

	*a = "9835485584873";
	*b = "7549361937528593750956";
	assert(((*a) / b)->toString() == "0");
	assert(((*a) % b)->toString() == "9835485584873");

	try
	{
		a = (*a) / std::shared_ptr<IBigNum>(new BigInt(0u));
		assert(false);
	}
	catch (std::exception& ex)
	{

	}

	*a = "-394592026748483583745";
	*b = "72534729375254";
	assert(((*a) / b)->toString() == "-5440043");
	assert(((*a) % b)->toString() == "20046261312177");

	*a = "394592026748483583745";
	*b = "-72534729375254";
	assert(((*a) / b)->toString() == "-5440043");
	assert(((*a) % b)->toString() == "-20046261312177");

	*a = "394592026748483583745";
	*b = "72534729375254";
	assert(((*a) / b)->toString() == "5440042");
	assert(((*a) % b)->toString() == "52488468063077");

	*a = "394592026748483583745";
	*b = "100000000000000000000";
	assert(((*a) % b)->toString() == "94592026748483583745");

	*a = "0";
	*b = "37485345";
	assert(((*a) - b)->toString() == "-37485345");

	*a = "-5";
	*b = "5";
	assert(((*a) + b)->toString() == "0");

	*a = "-5";
	*b = "5";
	assert((*((*a) + b) + b)->toString() == "5");

	BigInt c("00678");
	assert(c.toString() == "678");

	BigInt d("-0");

	std::cout << "BigInt test finished successful" << std::endl;
}

void BigDouble_test()
{
	std::shared_ptr<IBigNum> a;
	std::shared_ptr<IBigNum> b;
	a.reset(new BigDouble("100"));
	b.reset(new BigDouble("100.25"));

	assert(a->toString() == "100");
	assert(b->toString() == "100.25");

	assert(((*a) + b)->toString() == "200.25");

	*a = "0.9";
	*b = "0.1";
	assert(((*a) + b)->toString() == "1");

	*b = "0.025";
	assert(((*a) + b)->toString() == "0.925");

	*b = "0.1025";
	//std::cout << (a + b).toString() << std::endl;
	assert(((*a) + b)->toString() == "1.0025");

	*a = "0.00951";
	*b = "0.00050";
	assert(((*a) + b)->toString() == "0.01001");

	*a = "1";
	*b = "-0.5";
	assert(((*a) + b)->toString() == "0.5");

	*a = "0.2";
	*b = "-0.5";
	assert(((*a) + b)->toString(true) == "-0.3");

	*a = "3.5";
	*b = "-2.7";
	//std::cout << (a + b).toString() << std::endl;
	assert(((*a) + b)->toString(true) == "0.8");

	*a = "1.5";
	*b = "-2.7";
	assert(((*a) + b)->toString(true) == "-1.2");

	*a = "1.5";
	*b = "-2.3";
	assert(((*a) + b)->toString(true) == "-0.8");

	*a = "2.5";
	*b = "-2.7";
	assert(((*a) + b)->toString(true) == "-0.2");

	*a = "1.5";
	*b = "-0.007";
	assert(((*a) + b)->toString(true) == "1.493");

	*a = "1.015";
	*b = "-0.0067";
	assert(((*a) + b)->toString(true) == "1.0083");

	*a = "2.015";
	*b = "-3.006";
	assert(((*a) + b)->toString(true) == "-0.991");

	*a = "2.5";
	*b = "-3.5";
	assert(((*a) + b)->toString(true) == "-1");

	*a = "2.7";
	*b = "-3.5";
	assert(((*a) + b)->toString(true) == "-0.8");

	*a = "7.5";
	*b = "-3.5";
	assert(((*a) + b)->toString(true) == "4");

	*a = "7.8";
	*b = "-3.2";
	assert(((*a) + b)->toString(true) == "4.6");

	*a = "7.000154";
	*b = "-3.000061";
	assert(((*a) + b)->toString(true) == "4.000093");

	*a = "0.000154";
	*b = "-3.000061";
	assert(((*a) + b)->toString(true) == "-2.999907");

	*a = "0.000154";
	*b = "-3.000154";
	assert(((*a) + b)->toString(true) == "-3");

	*a = "-3.000865401";
	*b = "3.000865401";
	assert(((*a) + b)->toString(true) == "-0");

	*a = "2878348.00009374592340234";
	*b = "872354.007264544";
	assert(((*a) + b)->toString(true) == "3750702.00735828992340234");

	*a = "2364.0003945";
	*b = "-978542.0029345254";
	assert(((*a) + b)->toString(true) == "-976178.0025400254");

	*a = "1.045";
	*b = "-0.005";
	assert(((*a) + b)->toString(true) == "1.04");

	*a = "1.12";
	*b = "25.034";
	assert(((*a) * b)->toString(true) == "28.03808");

	*b = "1.12";
	assert(((*a) * b)->toString(true) == "1.2544");

	std::cout << "BigDouble test finished successful" << std::endl;
}

void BigNumber_test()
{
	std::shared_ptr<IBigNum> a;
	std::shared_ptr<IBigNum> b;

	a.reset(new BigInt("24"));
	b.reset(new BigDouble("0.5"));

	a = (*a) * b;
	std::cout << a->toString() << std::endl;
	assert(a->toString() == "12");

	IBigNum_ptr c(new BigInt("5"));

	/*try
	{
		a.reset(((*a) % c).get());
	}
	catch (std::exception& ex)
	{

	}*/

	std::cout << ((*c) * b)->toString() << std::endl;
	IBigNum_ptr d = (*c) * b;

	a = (*a) - /*(*c) * b*/d;
	assert(a->toString() == "9.5");

	a.reset(new BigInt("23089367583674934536092"));
	a = *((*a) % c) + (*a) * b;
	assert(a->toString() == "11544683791837467268048");

	b.reset(new BigDouble("0.00000000001"));
	a = (*a) * b;
	assert(a->toString() == "115446837918.37467268048");

	b.reset(new BigDouble("97364583534647.00902974774"));
	a = (*a) * b;
	assert(a->toString() == "11240433294314444638297514.8480305956698040221152");

	a.reset(new BigInt("3"));
	b.reset(new BigDouble("-0.01"));
	a = (*a) + b;
	//std::cout << a->toString() << std::endl;
	assert(a->toString() == "2.99");

	// TODO: wrong sign
	a.reset(new BigDouble("-23.0056"));
	b.reset(new BigInt("1"));
	a = (*a) + b;
	//std::cout << a->toString() << std::endl;
	assert(a->toString() == "-22.0056");

	std::cout << "BigNumber test finished successful" << std::endl;
}

void Evaluator_test()
{
	bool error = false;

	assert(Evaluator::getInstance().Evaluate("2+6", error) == "8");
	assert(Evaluator::getInstance().Evaluate("3+4*2/(1-5)", error) == "1");
	assert(Evaluator::getInstance().Evaluate("2+5*7-3*(89+0.5)", error) == "-231.5");
	assert(Evaluator::getInstance().Evaluate("2 + 5 * 7 - 3 * (89 + 0.5                 )", error) == "-231.5");
	assert(Evaluator::getInstance().Evaluate("2 + 5\n *\n 7\t\t - 3 * (89 + 0.5  \t\t)", error) == "-231.5");
	assert(Evaluator::getInstance().Evaluate("2 + 5 * 7 - 3 * (89 + 0.5", error) == "Missing a close bracket!!!");

	//std::cout << Evaluator::getInstance().Evaluate("3 - 0.01") << std::endl;
	assert(Evaluator::getInstance().Evaluate("23.0056 - 1", error) == "22.0056");
	//assert(Evaluator::getInstance().Evaluate("-23.0056 + 1") == "-22.0056");
	assert(Evaluator::getInstance().Evaluate("3 - 0.01", error) == "2.99");

	//std::cout << Evaluator::getInstance().Evaluate("pi * 7") << std::endl;

	assert(Evaluator::getInstance().Evaluate("2 + 5 * 7 - 3 * 89 + 0.5)", error) == "Missing an open bracket!!!");
	//std::cout << Evaluator::getInstance().Evaluate("2 + pi * 7 - 2 * (89 + 0.5)") << std::endl;
	assert(Evaluator::getInstance().Evaluate("2 + pi * 7 - 2 * (89 + 0.5)", error) == "-155.0088514248714476");
	assert(Evaluator::getInstance().Evaluate("2 + pi * 7 ^ x * (89 + 0.5)", error) == "Incorrect symbol!!!");
	assert(Evaluator::getInstance().Evaluate("2 + pi * 7 + x * (89 + 0.5)", error) == "Unknown variable!!!");
	assert(Evaluator::getInstance().Evaluate("10.5 / 3", error) == "Division is not defined for BigDouble!!!");

	assert(Evaluator::getInstance().Evaluate("x=23+34", error) == "");
	assert(Evaluator::getInstance().Evaluate("x % 5", error) == "2");
	assert(Evaluator::getInstance().Evaluate("x = 5", error) == "");
	assert(Evaluator::getInstance().Evaluate("x", error) == "5");

	assert(Evaluator::getInstance().Evaluate("y/=34 -54", error) == "Incorrect variable name!!!");
	assert(Evaluator::getInstance().Evaluate("2y=34 -54", error) == "Incorrect variable name!!!");
	assert(Evaluator::getInstance().Evaluate("y,,=34 -54", error) == "Incorrect variable name!!!");
	assert(Evaluator::getInstance().Evaluate("y=34 -54=x", error) == "Incorrect number!!!");
	assert(Evaluator::getInstance().Evaluate("y g=34 -54", error) == "Incorrect variable name!!!");

	Evaluator::getInstance().clearAllVars();
	assert(Evaluator::getInstance().Evaluate("2 + pi * 7 + x * (89 + 0.5)", error) == "Unknown variable!!!");

	Evaluator::getInstance().Evaluate("10 - - 2", error);

	std::cout << "Evaluator test finished successful" << std::endl;
}
#endif

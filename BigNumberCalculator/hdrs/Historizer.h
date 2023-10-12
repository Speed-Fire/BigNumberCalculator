/**
* @file Historizer.h
* File with description of class Historizer
*/

#pragma once
#include <string>
#include <vector>

/// 
/// \class Historizer
/// \brief Remembers all successful math expressions and their results,
///  can store this history to file and load it from him.
/// 
/// Only one object of this class can exist.
/// 
class Historizer
{
	std::vector<std::string> history;
	std::string filePath;

	/// Converts all variables in Evaluator to one string.
	/// @return string with variables.
	std::string saveVars();

	/// Converts all expressions in history to one string.
	/// @return string with expressions.
	std::string saveExprs();

	/// Standard constructor.
	Historizer();

public:
	/// Create or get already existing instance of Historizer
	/// @return historizer instance
	static Historizer& getInstance();

	Historizer(const Historizer&) = delete;
	Historizer& operator=(const Historizer&) = delete;

	/// Deletes all expressions in history.
	void clearCurrentHistory();

	/// Save current history and variables to file.
	void save();

	/// Load history and variables from file.
	void load();

	/// Adds expression to history.
	/// @param words of math expression.
	/// @param answer of math expression
	void addExpression(const std::vector<std::string>& words, const std::string& answer);

	/// Adds expression to history.
	/// @param expr expression to be add.
	void addExpression(const std::string& expr);
};
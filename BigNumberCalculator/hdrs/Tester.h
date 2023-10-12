/**
* @file Tester.h
* File with description of class Tester
*/

#include "Evaluator.h"

/// 
/// \class Tester
/// \brief Parses math expressions from file to calculator and prints dismatches 
///	of received result and expected result, and count of successful evaluations. 
/// 
/// Only one object of this class can exist.
/// 
class Tester
{
	/// \var filePath
	/// Path to file with examples.
	std::string filePath;

	/// \var pref
	/// Prefix, which is putted to begin of each tester's output.
	std::string pref = "test:~# ";

	/// The standard tester constructor.
	Tester();

	/// Reads data from file and puts it to exprs.
	/// @param[out] exprs readed data
	/// @return true if got some error
	bool readData(std::vector<std::pair<std::string, std::string>>& exprs);

	/// Tests readed from file data.
	/// @param[in] exprs data to test
	/// @return count of successful tests
	size_t testData(std::vector<std::pair<std::string, std::string>>& exprs);

	/// Checks if the given string starts with given prefix.
	/// @param[in] str string to check
	/// @param[in] pref prefix
	/// @return true if starts, otherwise false
	bool starts_with(const std::string& str, const std::string& pref) const;

public:
	Tester(const Tester&) = delete;

	/// Create or get already existing instance of Tester
	/// @return tester instance
	static Tester& getInstance();

	/// Start tests
	void testProgram();
};

/// Splits string to words by spaces.
/// @param[in] sentence string with words.
/// @return array of words.
std::vector<std::string> toWords(const std::string& sentence);

/// Check if the whole string is only space characters.
/// @param[in] str - string to check
/// @return true if yes, otherwise false.
bool isspace(std::string& str);

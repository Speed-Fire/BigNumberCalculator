/**
* @file BigDouble.h
* File with description of class BigDouble
*/

#pragma once

#include "BigInt.h"

/// 
/// \class BigDouble
/// \brief Class implementing for big double number.
/// 
/// Represents big double as big integer and comma point.
/// Comma point starts indexing from the right side of number. 
/// Derived from IBigNum.
///
class BigDouble : public IBigNum
{
	BigInt intPart; ///< sign of integer part is sign of the whole big double.
	size_t commaPoint;

	/// Converts string with number in 10 notation to big double and 
	/// assigns result to this big double.
	/// @param st string with number.
	void stringToNum(const std::string& st);

	/// Cut all last zeros from fractional part.
	void cutExcessZeros();

public:
	/// Standard constructor of big double.
	/// Assigns value to zero.
	BigDouble():commaPoint(0) {}

	/// Constructor of big double.
	/// Convert string to big double and then assigns it to this.
	/// @param num string with number.
	BigDouble(std::string num);

	/// Constructor of big double.
	/// Copies another big double to this.
	/// @param num another big double.
	BigDouble(const BigDouble& num);

	/// Converts this big double to string format in 10 notation
	/// @param zeroMinus show zero with its sign (without plus).
	/// @return string with number.
	std::string toString(bool zeroMinus = false) const;

	/// Checks if this big double has minus sign.
	/// @return true if number is negative.
	bool checkMinus() const { return intPart.checkMinus(); }

	/// Method for addition this big double and second big double.
	/// Doesn't change this big double.
	/// @param num2 second big double.
	/// @return result of operation.
	BigDouble addBigDouble(BigDouble num2) const;

	/// Method for subtraction this big double and second big double.
	/// Doesn't change this big double.
	/// @param num2 second big double.
	/// @return result of operation.
	BigDouble subBigDouble(const BigDouble& num2) const;

	/// Method for multiplication this big double and second big double.
	/// Doesn't change this big double.
	/// @param num2 second big double.
	/// @return result of operation.
	BigDouble multiplyBigDouble(const BigDouble& num2) const;

	/// Multiplies this big integer by -1.
	void timesMinusOne() { intPart.timesMinusOne(); }

	/// Makes copy of this big double.
	/// @return pointer to copy.
	IBigNum_ptr clone() const;

	/// Operator for addition of two big numbers.
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	IBigNum_ptr operator+(const IBigNum_ptr& num2) const;

	/// Operator for addition of two big numbers.
	/// @param num2 second number.
	/// @return pointer to result of operation (also big number).
	IBigNum_ptr operator+(long num2) const;

	/// Operator for subtracting of two big numbers.
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	IBigNum_ptr operator-(const IBigNum_ptr& num2) const;

	/// Operator for multiplying of two big numbers.
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	IBigNum_ptr operator*(const IBigNum_ptr& num2) const;

	/// Operator for division of two big numbers.
	/// This version only throws an exception of prohibited division with big doubles.
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	IBigNum_ptr operator/(const IBigNum_ptr& num2) const;

	/// Operator for modding of two big numbers.
	/// This version only throws an exception of prohibited modding with big doubles.
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	IBigNum_ptr operator%(const IBigNum_ptr& num2) const;

	/// Operator for assigning big number.
	/// @param num string with a number.
	/// @return reference to current big double.
	BigDouble& operator=(const std::string& num);
};
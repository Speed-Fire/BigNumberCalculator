/**
* @file IBigNum.h
* File with description of interface IBigNum.
*/

#pragma once

#include <string>
#include <memory>

class IBigNum;

/// Simple name for pointer to big number.
#define IBigNum_ptr std::shared_ptr<IBigNum>

/// 
/// \class IBigNum
/// \brief An abstract class for big numbers.
/// 
/// Contains some methods for outside use.
/// 
class IBigNum
{
public:
	/// A pure virtual method for checking a sign of a number.
	/// @return True if number is negative, otherwise - false.
	virtual bool checkMinus() const = 0;

	/// A pure virtual method for converting number to string.
	/// @param[in] zeroMinus If true displays zero with his sign (without plus).
	/// @return string with number
	virtual std::string toString(bool zeroMinus = false) const = 0;

	/// A pure virtual method for copying IBigNum
	/// @return pointer to copy
	virtual IBigNum_ptr clone() const = 0;

	/// A pure virtual method for multiplying big number by -1.
	virtual void timesMinusOne() = 0;

	/// A pure virtual operator for addition of two big numbers.
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	virtual IBigNum_ptr operator+(const IBigNum_ptr& num2) const = 0;

	/// A pure virtual operator for addition of two big numbers.
	/// @param num2 second number.
	/// @return pointer to result of operation (also big number).
	virtual IBigNum_ptr operator+(long num2) const = 0;

	/// A pure virtual operator for subtracting of two big numbers.
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	virtual IBigNum_ptr operator-(const IBigNum_ptr& num2) const = 0;

	/// A pure virtual operator for multiplying of two big numbers.
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	virtual IBigNum_ptr operator*(const IBigNum_ptr& num2) const = 0;

	/// A pure virtual operator for division of two big numbers.
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	virtual IBigNum_ptr operator/(const IBigNum_ptr& num2) const = 0;

	/// A pure virtual operator for modding of two big numbers.
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	virtual IBigNum_ptr operator%(const IBigNum_ptr& num2) const = 0;

	/// A pure virtual operator for assigning big number.
	/// @param num string with a number.
	/// @return reference to current big number.
	virtual IBigNum& operator=(const std::string& num) = 0;
};
/**
* @file BigInt.h
* File with description of class BigInt
*/

#pragma once

#include "IBigNum.h"

#include<vector>
#include<string>

/// 
/// \class BigInt
/// \brief Class implementing for big integer number.
/// 
/// Represents big integer as an array of uint32 in notation 2^32.
/// Derived from IBigNum.
/// 
class BigInt : public IBigNum
{
	/// \var bigNum
	/// Array, which contains the number in notation 2^32 without a sign.
	std::vector<uint32_t> bigNum;

	/// \var isMinus
	/// Sign of the number. True if number is negative.
	bool isMinus;

	/// Derives this big integer by simple uint32.
	/// Doesn't change this big number.
	/// @param num divider.
	/// @return pair of quotient and reminder.
	std::pair<BigInt, uint32_t> divInt(uint32_t num) const;

	/// Shifts all elements in array bigNum to right (n -> n + 1).
	/// Equals to multiplying big integer by (uint32_max + 1).
	/// Changes this big number.
	void shiftR();

	/// Converts string with number in 10 notation to big integer and 
	/// assigns result to this big integer.
	/// @param st string with number.
	void stringToNum(const std::string& st);
public:
	/// Compares if absolute value of this big integer is less than absolute value of another big integer.
	/// @param num2 another big integer.
	/// @return if this is less than second in absolute.
	bool absLessThan(const BigInt& num2) const;

	/// Compares if absolute value of this big integer is equal to absolute value of another big integer.
	/// @param num2 another big integer.
	/// @return if this is equal to second in absolute.
	bool isAbsEqual(const BigInt& num2) const;

	/// Standard constructor of big integer.
	/// Assigns value to zero.
	BigInt()
		:
		isMinus(false)
	{
		bigNum.push_back(0);
	}

	/// Constructor of big integer.
	/// Assigns value to value of parameter.
	/// @param num number to be assigned.
	BigInt(uint32_t num)
		:
		isMinus(false)
	{
		bigNum.push_back(num);
	}

	/// Constructor of big integer.
	/// Convert string to big integer and then assigns it to this.
	/// @param num string with number.
	BigInt(const std::string& num)
		:
		isMinus(false)
	{
		stringToNum(num);
	}

	/// Constructor of big integer.
	/// Convert string to big integer and then assigns it to this.
	/// @param num string with number.
	BigInt(const char* num)
		:
		isMinus(false)
	{
		std::string st(num);
		stringToNum(st);
	}

	/// Converts this big integer to string format in 10 notation
	/// @param zeroMinus show zero with its sign (without plus).
	/// @return string with number.
	std::string toString(bool zeroMinus = false) const;

	/// Checks if this big integer has minus sign.
	/// @return true if number is negative.
	bool checkMinus() const { return isMinus; }

	/// Checks if this number equals to zero.
	/// Sign nevermind.
	/// @return true if is zero.
	bool isZero() const;

	/// Method for addition this big integer and second big integer.
	/// Doesn't change this big integer.
	/// @param num2 second big integer.
	/// @return result of operation.
	BigInt addBigInt(BigInt num2) const;

	/// Method for subtraction this big integer and second big integer.
	/// Doesn't change this big integer.
	/// @param num2 second big integer.
	/// @return result of operation.
	BigInt subBigInt(const BigInt& num2) const;

	/// Method for multiplication this big integer and second big integer.
	/// Doesn't change this big integer.
	/// @param num2 second big integer.
	/// @return result of operation.
	BigInt multiplyBigInt(const BigInt& num2) const;

	/// Method for division this big integer by second big integer.
	/// Doesn't change this big integer.
	/// @param num second big integer.
	/// @return quotient and reminder.
	std::pair<BigInt, BigInt> divide(const BigInt& num) const;

	/// Multiplies this big integer by -1.
	void timesMinusOne() { isMinus = !isMinus; }

	/// Makes copy of this big integer.
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
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	IBigNum_ptr operator/(const IBigNum_ptr& num2) const;

	/// Operator for modding of two big numbers.
	/// @param num2 pointer so second big number.
	/// @return pointer to result of operation (also big number).
	IBigNum_ptr operator%(const IBigNum_ptr& num2) const;

	/// Operator for assigning big number.
	/// @param num string with a number.
	/// @return reference to current big number.
	IBigNum& operator=(const std::string& num);

	/// Operator for assigning big integer.
	/// @param num2 another big integer.
	/// @return reference to current big number.
	BigInt& operator=(const BigInt& num2);

	/// Compares if this big integer is less than another big integer.
	/// @param num2 another big integer.
	/// @return if this is less than second.
	bool operator<(const BigInt& num2) const;
};
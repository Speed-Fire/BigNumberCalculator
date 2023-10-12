#include "../hdrs/BigInt.h"
#include "../hdrs/BigDouble.h"

#include <limits.h>
#include <bitset>
#include <algorithm>
#include <stdexcept>

#include <iostream>

void BigInt::stringToNum(const std::string& st)
{
	size_t start = 0;
	bigNum.clear();

	if (st[0] == '-')
	{
		isMinus = true;
		start++;
	}
	else
		isMinus = false;

	/// Convert decimal number string to binary number string
	std::string tmp = st.substr(start);
	std::string binNum;

	while (tmp.compare("0") != 0 || binNum.size() % 32)
	{
		char rem = 0;

		for (auto& symb : tmp)
		{
			auto dig = symb + rem * 10 - '0';
			rem = dig % 2;
			symb = dig / 2 + '0';
		}

		// trimming
		while (tmp.size() > 1 && tmp.front() == '0')
			tmp.erase(0, 1);

		binNum.push_back(rem + '0');
	}

	std::reverse(binNum.begin(), binNum.end());

	// binary number string to vector of uint32_t
	for (size_t i = 0; i < binNum.size(); i += 32)
		bigNum.emplace_back(
			std::bitset<32>(binNum.substr(i, 32)).to_ulong());

	std::reverse(bigNum.begin(), bigNum.end());

	if (bigNum.empty())
		bigNum.push_back(0);

}

void BigInt::shiftR()
{
	bigNum.push_back(0);
	for (size_t i = bigNum.size() - 1; i > 0; i+=-1)
		bigNum[i] = bigNum[i - 1];

	bigNum[0] = 0;
}

std::pair<BigInt, uint32_t> BigInt::divInt(uint32_t num) const
{
	BigInt copy = *this;

	uint32_t rem = 0;

	for (int i = bigNum.size() - 1; i >= 0; i--)
	{
		uint64_t dig = bigNum[i] + rem * ((uint64_t)UINT32_MAX + 1);
		rem = (uint32_t)(dig % num);
		copy.bigNum[i] = dig / num;
	}

	while (copy.bigNum.size() > 1 && copy.bigNum.back() == 0)
		copy.bigNum.erase(copy.bigNum.end() - 1);

	return std::pair<BigInt, uint32_t>(copy, rem);
}

std::pair<BigInt, BigInt> BigInt::divide(const BigInt& num) const
{
	if (num.bigNum.size() == 1 && num.bigNum[0] == 0)
		throw std::runtime_error("Division by zero!!!");

	bool finalSign = isMinus != num.isMinus;

	if (num.bigNum.size() == 1)
	{
		auto res = divInt(num.bigNum[0]);
		res.first.isMinus = finalSign;
		BigInt rem(res.second);
		rem.isMinus = isMinus;

		if (finalSign && res.second)
		{
			bool tmp = res.first.isMinus;
			res.first.isMinus = false;
			res.first = res.first.addBigInt("1");
			res.first.isMinus = tmp;

			rem = subBigInt(num.multiplyBigInt(res.first));
		}

		return { res.first, rem };
	}

	if (absLessThan(num))
	{
		BigInt rem(*this);
		rem.isMinus = isMinus;

		if (!finalSign)
		{
			return { BigInt(0u), rem };
		}
		else
		{
			BigInt res = "-1";

			rem = subBigInt(num.multiplyBigInt(res));

			return { res, rem };
		}
	}

	BigInt divisor(num);
	divisor.isMinus = false;
	BigInt rem(0u);
	BigInt res;
	res.bigNum.clear();
	for (size_t i = bigNum.size() - 1;; i += -1)
	{
		rem.bigNum[0] = bigNum[i];
		if (rem.absLessThan(divisor) && i != 0)
		{
			rem.shiftR();
			continue;
		}

		uint32_t _divisor_small = 0, koef = 1;
		BigInt tmp(divisor);
		while (divisor.absLessThan(rem))
		{
			if (rem.absLessThan(tmp))
			{
				tmp = divisor;
				koef = 1;
			}

			_divisor_small += koef;
			rem = rem.subBigInt(tmp);
			tmp = tmp.multiplyBigInt(BigInt(2u));
			koef *= 2;
		}

		res.shiftR();
		res.bigNum[0] = _divisor_small;

		if (i == 0) break;
		rem.shiftR();
	}

	rem.isMinus = isMinus;

	if (finalSign && !rem.isZero())
	{
		res.isMinus = false;
		res = res.addBigInt("1");

		res.isMinus = finalSign;

		rem = subBigInt(num.multiplyBigInt(res));
	}

	res.isMinus = finalSign;

	return { res, rem };
}

bool BigInt::absLessThan(const BigInt& num2) const
{
	if (this->bigNum.size() < num2.bigNum.size())
		return true;
	else if (this->bigNum.size() > num2.bigNum.size())
		return false;

	for (auto i = this->bigNum.size() - 1; i >= 0; i--)
	{
		if (this->bigNum[i] < num2.bigNum[i])
			return true;
		else if (this->bigNum[i] > num2.bigNum[i])
			return false;

		if (i == 0) break;
	}

	return false;
}

bool BigInt::isAbsEqual(const BigInt& num2) const
{
	if (bigNum.size() != num2.bigNum.size())
		return false;

	for (size_t i = 0; i < bigNum.size(); i++)
		if (bigNum[i] != num2.bigNum[i])
			return false;

	return true;
}

bool BigInt::isZero() const
{
	return bigNum.size() == 1 && bigNum[0] == 0;
}

std::string BigInt::toString(bool zeroMinus) const
{
	std::vector<uint32_t> copy = bigNum;
	std::reverse(copy.begin(), copy.end());

	std::string bitStr;
	std::string res;

	/// convert vector of uint32_t to binary number string
	for (size_t i = 0; i < copy.size(); i++)
		bitStr += std::bitset<32>(copy[i]).to_string();

	/// convert bin num str to dec num str
	while (bitStr.compare("0") != 0)
	{
		// Euclidean division
		uint8_t rem = 0;
		for (auto& bit : bitStr)
		{
			rem = rem * 2 + bit - '0';
			if (rem >= 10)
			{
				bit = '1';
				rem -= 10;
			}
			else
				bit = '0';
		}

		res.push_back(rem + '0');

		// trimming
		while (bitStr.size() > 1 && bitStr.front() == '0')
			bitStr.erase(0, 1);
	}

	std::reverse(res.begin(), res.end());

	if(!zeroMinus)
		return (isMinus && !(bigNum.size() == 1 && bigNum[0] == 0) ? "-" : "") + res;
	else
		return (isMinus ? "-" : "") + res;
}

bool BigInt::operator<(const BigInt& num2) const
{
	if (this->isMinus && !num2.isMinus)
		return true;
	else if (!this->isMinus && num2.isMinus)
		return false;

	return absLessThan(num2);
}

BigInt BigInt::addBigInt(BigInt num2) const
{
	BigInt copy = *this;
	bool finalSign = false;
	bool subtraction = false;

	/// Final sign and operation computing
	if (this->isMinus == num2.isMinus) // numbers have same signs
	{
		finalSign = copy.isMinus;
	}
	else
	{
		bool leftIsModuloLess = absLessThan(num2);
		finalSign = (!leftIsModuloLess && copy.isMinus) ||
			(leftIsModuloLess && num2.isMinus);

		if (leftIsModuloLess) // |this| < |num2|
			std::swap(copy, num2);

		subtraction = true;
	}


	for (size_t i = 0; i < num2.bigNum.size(); i++)
	{
		if (i >= copy.bigNum.size())
		{
			copy.bigNum.push_back(num2.bigNum[i]);
		}
		else if (!subtraction) // addition
		{
			size_t k = i;
			uint32_t tmp = num2.bigNum[i];
			uint32_t old_tmp = 0;
			do
			{
				if (k == copy.bigNum.size())
					copy.bigNum.push_back(0);

				copy.bigNum[k] += tmp;

				old_tmp = tmp;
				tmp = 1;
			} while (copy.bigNum[k++] < old_tmp);
		}
		else // subtraction
		{
			size_t k = i;
			uint32_t tmp = num2.bigNum[i];
			uint32_t old_tmp = 0;
			do
			{
				old_tmp = copy.bigNum[k];

				copy.bigNum[k] -= tmp;
				tmp = 1;
			} while (copy.bigNum[k++] > old_tmp);
		}
	}

	while (copy.bigNum[copy.bigNum.size() - 1] == 0 && copy.bigNum.size() != 1)
		copy.bigNum.pop_back();

	copy.isMinus = finalSign;

	return copy;
}

BigInt BigInt::subBigInt(const BigInt& num2) const
{
	BigInt num = BigInt(num2);
	num.isMinus = !num.isMinus;
	return addBigInt(num);
}

BigInt BigInt::multiplyBigInt(const BigInt& num2) const
{
	BigInt res;

	if ((bigNum.size() == 1 && bigNum[0] == 0) ||
		(num2.bigNum.size() == 1 && num2.bigNum[0] == 0))
	{
		res.isMinus = this->isMinus != num2.isMinus;
		return res;
	}

	for (size_t i = 1; i < bigNum.size(); i++)res.bigNum.push_back(0);

	res.isMinus = this->isMinus != num2.isMinus;

	uint64_t tmp = 0;
	uint32_t carry = 0;
	size_t iter = 0;
	for (size_t i = 0; i < num2.bigNum.size(); i++, iter++)
	{
		for (size_t j = 0; j < bigNum.size(); j++)
		{
			tmp = (uint64_t)num2.bigNum[i] * bigNum[j];

			if (iter + j == res.bigNum.size())
				res.bigNum.push_back(0);

			size_t k = iter + j;
			carry = (uint32_t)tmp;
			uint32_t old_carry = 0;
			do
			{
				if (k == res.bigNum.size())
					res.bigNum.push_back(0);

				res.bigNum[k] += carry;
				old_carry = carry;

				carry = 1;
			} while (res.bigNum[k++] < old_carry);


			k = iter + j + 1;
			carry = tmp / ((uint64_t)UINT32_MAX + 1);
			old_carry = 0;
			do
			{
				if (k == res.bigNum.size())
					res.bigNum.push_back(0);

				res.bigNum[k] += carry;
				old_carry = carry;

				carry = 1;
			} while (res.bigNum[k++] < old_carry);
		}
	}

	while (res.bigNum.size() > 1 && res.bigNum.back() == 0)
		res.bigNum.erase(res.bigNum.end() - 1);

	return res;
}

IBigNum_ptr BigInt::clone() const
{
	return IBigNum_ptr(new BigInt(*this));
}

IBigNum_ptr BigInt::operator+(const IBigNum_ptr& num2) const
{
	if (BigInt* kk = dynamic_cast<BigInt*>(num2.get()))
	{
		IBigNum_ptr res(new BigInt(addBigInt(*kk)));
		return res;
	}
	else
	{
		IBigNum_ptr t(new BigInt(*this));
		
		return (*dynamic_cast<BigDouble*>(num2.get())) + t;
	}
}

IBigNum_ptr BigInt::operator+(long num2) const
{
	BigInt t(abs(num2));
	if (num2 < 0)
		t.isMinus = true;

	IBigNum_ptr res(new BigInt(addBigInt(t)));
	return res;
}

IBigNum_ptr BigInt::operator-(const IBigNum_ptr& num2) const
{
	if (BigInt* kk = dynamic_cast<BigInt*>(num2.get()))
	{
		IBigNum_ptr res(new BigInt(subBigInt(*kk)));
		return res;
	}
	else
	{
		IBigNum_ptr t(new BigInt(*this));
		IBigNum_ptr t2 = num2->clone();
		t2->timesMinusOne();

		return (*dynamic_cast<BigDouble*>(t2.get())) + t;
	}
}

IBigNum_ptr BigInt::operator*(const IBigNum_ptr& num2) const
{
	if (BigInt* kk = dynamic_cast<BigInt*>(num2.get()))
	{
		IBigNum_ptr res(new BigInt(multiplyBigInt(*kk)));
		return res;
	}
	else
	{
		IBigNum_ptr t(new BigInt(*this));

		return (*dynamic_cast<BigDouble*>(num2.get())) * t;
	}
}

IBigNum_ptr BigInt::operator/(const IBigNum_ptr& num2) const
{
	if (BigInt* kk = dynamic_cast<BigInt*>(num2.get()))
		return IBigNum_ptr(new BigInt(divide(*kk).first));
	else
		throw std::runtime_error("Division is not defined for BigDouble!!!");
}

IBigNum_ptr BigInt::operator%(const IBigNum_ptr& num2) const
{
	if (BigInt* kk = dynamic_cast<BigInt*>(num2.get()))
		return IBigNum_ptr(new BigInt(divide(*kk).second));
	else
		throw std::runtime_error("Mod is not defined for BigDouble!!!");
}

IBigNum& BigInt::operator=(const std::string& num)
{
	stringToNum(num);
	return *this;
}

BigInt& BigInt::operator=(const BigInt& num2)
{
	bigNum = num2.bigNum;
	isMinus = num2.isMinus;

	return *this;
}

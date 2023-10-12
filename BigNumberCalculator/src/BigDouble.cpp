#include "../hdrs/BigDouble.h"

#include <stdexcept>

void BigDouble::stringToNum(const std::string& st)
{
	size_t dot = st.find('.');
	if (dot != std::string::npos)
	{
		std::string num = st.substr(0, dot) + st.substr(dot + 1, st.length() - dot);
		commaPoint = st.substr(dot + 1, st.length() - dot).length();
		intPart = BigInt(num);
	}
	else
	{
		commaPoint = 0;
		intPart = BigInt(st);
	}
}

void BigDouble::cutExcessZeros()
{
	for(std::pair<BigInt, BigInt> tmp; (tmp = intPart.divide(10)).second.isZero() && commaPoint > 0;)
	{
		intPart = tmp.first;
		commaPoint--;
	}
}

BigDouble::BigDouble(std::string num)
	:
	commaPoint(0)
{
	stringToNum(num);
}

BigDouble::BigDouble(const BigDouble& num)
	:
	commaPoint(0)
{
	stringToNum(num.toString(true));
}

std::string BigDouble::toString(bool zeroMinus) const
{
	std::string res = intPart.toString();
	size_t p = intPart.checkMinus() ? 1 : 0;

	while (res.length() - p <= commaPoint)
		res.insert(p, "0");

	if (commaPoint)
		res.insert(res.length() - commaPoint, ".");

	return res;
}

BigDouble BigDouble::addBigDouble(BigDouble num2) const
{
	BigDouble res(*this);

	size_t cP = std::max(res.commaPoint, num2.commaPoint);

	for (size_t it = res.commaPoint; it < cP; it++)
	{
		res.intPart = res.intPart.multiplyBigInt(10);
	}
	
	for (size_t it = num2.commaPoint; it < cP; it++)
	{
		num2.intPart = num2.intPart.multiplyBigInt(10);
	}

	res.intPart = res.intPart.addBigInt(num2.intPart);
	res.commaPoint = cP;

	res.cutExcessZeros();

	return res;
}

BigDouble BigDouble::subBigDouble(const BigDouble& num2) const
{
	BigDouble t = num2;
	t.intPart.timesMinusOne();
	return addBigDouble(t);
}

BigDouble BigDouble::multiplyBigDouble(const BigDouble& num2) const
{
	BigDouble res(*this);
	res.intPart = res.intPart.multiplyBigInt(num2.intPart);
	res.commaPoint += num2.commaPoint;

	res.cutExcessZeros();

	return res;
}

IBigNum_ptr BigDouble::clone() const
{
	return IBigNum_ptr(new BigDouble(*this));
}


IBigNum_ptr BigDouble::operator+(const IBigNum_ptr& num2) const
{
	if (BigDouble* kk = dynamic_cast<BigDouble*>(num2.get()))
	{
		BigDouble k = addBigDouble(*kk);
		if (!k.commaPoint)
			return IBigNum_ptr(new BigInt(k.intPart));
		else
			return IBigNum_ptr(new BigDouble(k));
	}
	else
	{
		BigInt* kk2 = dynamic_cast<BigInt*>(num2.get());
		BigDouble t("0");
		t.intPart = *kk2;
		
		return IBigNum_ptr(new BigDouble(addBigDouble(t)));
	}
}

IBigNum_ptr BigDouble::operator+(long num2) const
{
	BigDouble t("0");
	t.intPart = num2;

	return IBigNum_ptr(new BigDouble(addBigDouble(t)));
}

IBigNum_ptr BigDouble::operator-(const IBigNum_ptr& num2) const
{
	if (BigDouble* kk = dynamic_cast<BigDouble*>(num2.get()))
	{
		BigDouble k = subBigDouble(*kk);
		if (!k.commaPoint)
			return IBigNum_ptr(new BigInt(k.intPart));
		else
			return IBigNum_ptr(new BigDouble(k));
	}
	else
	{
		BigInt* kk2 = dynamic_cast<BigInt*>(num2.get());
		BigDouble t("0");
		t.intPart = *kk2;
		t.intPart.timesMinusOne();

		return IBigNum_ptr(new BigDouble(addBigDouble(t)));
	}
}

IBigNum_ptr BigDouble::operator*(const IBigNum_ptr& num2) const
{
	if (BigDouble* kk = dynamic_cast<BigDouble*>(num2.get()))
	{
		return IBigNum_ptr(new BigDouble(multiplyBigDouble(*kk)));
	}
	else
	{
		BigInt* kk2 = dynamic_cast<BigInt*>(num2.get());
		BigDouble t("0");
		t.intPart = *kk2;

		BigDouble k = multiplyBigDouble(t);
		if (!k.commaPoint)
			return IBigNum_ptr(new BigInt(k.intPart));
		else
			return IBigNum_ptr(new BigDouble(k));
	}
}

IBigNum_ptr BigDouble::operator/(const IBigNum_ptr& num2) const
{
	throw std::runtime_error("Division is not defined for BigDouble!!!");
	return IBigNum_ptr();
}

IBigNum_ptr BigDouble::operator%(const IBigNum_ptr& num2) const
{
	throw std::runtime_error("Mod is not defined for BigDouble!!!");
	return IBigNum_ptr();
}

BigDouble& BigDouble::operator=(const std::string& num)
{
	commaPoint = 0;

	stringToNum(num);

	return *this;
}
#pragma once

template<typename _Ty>
class counter
{
public:
	counter()
	{
		++countNum;
	}
	counter(const counter&)
	{
		++countNum;
	}
	~counter()
	{
		--countNum;
	}
	static size_t howMany()
	{
		return countNum;
	}
private:
	static size_t countNum;
};
// countedObject.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<memory>
#include"Counted.h"
using namespace std;

class printer :private Counted<printer>
{
public:
	static printer* makePrinter()
	{
		return new printer();
	}
	static printer* makePrinter(const printer& rhs)
	{
		return new printer(rhs);
	}
	~printer(){}
	using Counted<printer>::objectCount;
	using Counted<printer>::TooManyObjects;
private:
	printer(){}
	printer(const printer& rhs){}
};
template<>
const size_t Counted<printer>::maxObjects = 2;

int main()
{
	shared_ptr<printer> sp(printer::makePrinter());
	cout << sp->objectCount();
	shared_ptr<printer> sp1(printer::makePrinter());
	system("pause");
    return 0;
}


// countedObject.cpp : �������̨Ӧ�ó������ڵ㡣
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

int main()
{
	shared_ptr<printer> sp(printer::makePrinter());
	cout << sp->objectCount();
	system("pause");
    return 0;
}

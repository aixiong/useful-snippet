// CountingObjects.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include"counter.h"
using namespace std;

class printer:private counter<printer>
{
public:
	using counter<printer>::howMany;
};

template<>
size_t counter<printer>::countNum = 0;

class specialPrinter :public printer,private counter<specialPrinter>
{
public:
	using counter<specialPrinter>::howMany;
};

template<>
size_t counter<specialPrinter>::countNum = 0;

class A1
{
public:
	void func() {
		cout << "A1" << endl;
	}
};
class A2
{
public:
	void func() {
		cout << "A2" << endl;
	}
};
class A:public A1,public A2
{
public:
	using A1::func;
	A2::func;
};

int main()
{
	A a;
	a.func();
	system("pause");
    return 0;
}


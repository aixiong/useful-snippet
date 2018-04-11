// heap class.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include"heapTracked.h"
using namespace std;


class A {
public:
	virtual ~A() = 0{}
};

class B:public A
{
public:
	void func(){}
};

int main()
{
	B b;
	system("pause");
    return 0;
}


// final class.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include"unInherited.h"
using namespace std;

class A :private virtual unInherited<A>
{

};

class B :public A
{

};

int main()
{
	A a;
	B b;
	system("pause");
    return 0;
}


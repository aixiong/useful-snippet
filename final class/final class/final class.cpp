// final class.cpp : �������̨Ӧ�ó������ڵ㡣
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


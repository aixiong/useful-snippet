// AVLTree-learn.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<vector>
#include"AVLTree.h"
using namespace std;

template<typename T,template<typename,typename> class container>
class A
{
private:
	container<T,allocator<T>> one;
};
int main()
{
	A<int,std::vector> a;
	system("pause");
    return 0;
}


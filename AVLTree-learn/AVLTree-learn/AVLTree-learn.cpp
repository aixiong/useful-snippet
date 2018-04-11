// AVLTree-learn.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<vector>
#include<memory>
#include"AVLTree.h"
using namespace std;




int main()
{
	xhy::simple_avlTree<int> one;
	while (true)
	{
		int t, n;
		cin >> t;
		if (t == 1)
		{
			cin >> n;
			one.insert(n);
		}
		else if (t == 2)
		{
			cin >> n;
			cout << one.remove(n) << endl;
		}
		else if(t==3)
		{
			cin >> n;
			cout << one.search(n)->value_field << endl;
		}
		else
		{
			one.print();
			cout << endl;
		}
	}
	system("pause");
    return 0;
}


#pragma once

template<typename T>
class unInherited
{
private:
	unInherited(){}
	~unInherited(){}
private:
	friend  T;
};


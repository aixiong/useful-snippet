// callback.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>

template<typename ReturnType,typename... Args>
class Callback
{
public:
	typedef ReturnType(*FuncType)(void*, Args...);
	Callback():func(nullptr),obj(nullptr){}
	Callback(FuncType f,void* o):func(f),obj(o){}
	ReturnType operator()(Args... args)
	{
		return (*func)(obj, args...);
	}
private:
	FuncType func;
	void* obj;
};

template<typename ReturnType,class T,typename... Args>
class MemberCallbackFatory
{
private:
	template<ReturnType (T::*Func)(Args...)>
	static ReturnType Wrapper(void* o, Args... args)
	{
		return (static_cast<T*>(o)->*Func)(args...);
	}
public:
	template<ReturnType(T::*Func)(Args...)>
	static Callback<ReturnType, Args...> Bind(T* o)
	{
		return Callback<ReturnType, Args...>(&MemberCallbackFatory::Wrapper<Func>, o);
	}
};

template<typename ReturnType, typename... Args>
class FreeCallbackFatory
{
private:
	template<ReturnType(*Func)(Args...)>
	static ReturnType Wrapper(void* o,Args... args)
	{
		return (*Func)(args...);
	}
public:
	template<ReturnType(*Func)(Args...)>
	static Callback<ReturnType, Args...> Bind(void*)
	{
		return Callback<ReturnType, Args...>(&FreeCallbackFatory::Wrapper<Func>, nullptr);
	}
};

template<typename ReturnType,class T,typename... Args>
MemberCallbackFatory<ReturnType, T, Args...> GetCallbackFatory(ReturnType(T::*Func)(Args...))
{
	return MemberCallbackFatory<ReturnType, T, Args...>();
}
template<typename ReturnType,  typename... Args>
FreeCallbackFatory<ReturnType, Args...> GetCallbackFatory(ReturnType(*Func)(Args...))
{
	return FreeCallbackFatory<ReturnType, Args...>();
}

class A
{
public:
	void sb(int a)
	{
		std::cout << "fafada";
	}
};
#define BIND_MEM_CB(memFuncPtr,instancePtr) \
(GetCallbackFatory(memFuncPtr).Bind<memFuncPtr>(instancePtr))

#define BIND_FREE_CB(memFuncPtr) \
(GetCallbackFatory(memFuncPtr).Bind<memFuncPtr>(nullptr))

void fda(int){}
int main()
{
	A a;
	Callback<void, int> one = BIND_FREE_CB(&fda);
	one(11);
	system("pause");
    return 0;
}


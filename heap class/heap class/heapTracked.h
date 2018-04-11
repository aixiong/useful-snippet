#pragma once

#include<unordered_set>

class heapTracked
{
public:
	class MissingAddress{};
	virtual ~heapTracked() = 0{}

	static void* operator new(size_t size);
	static void operator delete(void *ptr);

	bool isOnHeap()const;
private:
	typedef const void* RawAddress;
	static std::unordered_set<RawAddress> addresses;
};
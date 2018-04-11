#include "heapTracked.h"

void * heapTracked::operator new(size_t size)
{
	void *memPtr = ::operator new(size);
	addresses.insert(memPtr);
	return memPtr;
}

void heapTracked::operator delete(void * ptr)
{
	auto it = addresses.find(ptr);
	if (it != addresses.end())
	{
		addresses.erase(it);
		::operator delete(ptr);
	}
	else throw MissingAddress();
}

bool heapTracked::isOnHeap() const
{
	const void *rawAddress = dynamic_cast<const void*>(this);
	return addresses.find(rawAddress) != addresses.end();
}

std::unordered_set<heapTracked::RawAddress> heapTracked::addresses;

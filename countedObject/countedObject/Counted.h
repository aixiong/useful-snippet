#pragma once

/*
Counted.h used for private base class of some classes which need counting.
*/

template<typename BeingCounted>
class Counted
{
public:
	class TooManyObjects{};
	static int objectCount() { return numObjects; }
protected:
	Counted();
	Counted(const Counted& rhs);
	~Counted() { --numObjects; }
private:
	static size_t numObjects;
	static const size_t maxObjects = 1;
	void init();
};

template<typename BeingCounted>
size_t Counted<BeingCounted>::numObjects = 0;


template<typename BeingCounted>
Counted<BeingCounted>::Counted()
{
	init();
}
template<typename BeingCounted>
Counted<BeingCounted>::Counted(const Counted<BeingCounted>&)
{
	init();
}

template<typename BeingCounted>
void Counted<BeingCounted>::init()
{
	if (numObjects >= maxObjects)throw TooManyObjects();
	++numObjects;
}



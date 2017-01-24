#pragma once
#ifndef GCC_TOOL
#define GCC_TOOL

class GCCRandom
{
public:
	GCCRandom();
	~GCCRandom();
	unsigned int Random(unsigned int range);
	double Random();
	void SetRandomSeed(unsigned int seed);
	unsigned int GetRandomSeed();
private:

};
#endif // !GCC_TOOL

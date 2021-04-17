#ifndef MEMORY_H
#define MEMORY_H

#include <Windows.h>
#include <Psapi.h>

class Memory
{
private:
	SIZE_T maxPhysicalMem;
	SIZE_T maxVirtualMem;

	static Memory& getInstance();

	Memory();
public:
	~Memory();

	static void resetMemUsed();
	static void recordMemUsed();
	static void printMemUsed();
	static void getMemUsed(SIZE_T& virtualMem, SIZE_T& physicalMem);
};

#endif // !MEMORY_H

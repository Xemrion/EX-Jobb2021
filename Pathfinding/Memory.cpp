#include "Memory.h"
#include <iostream>

Memory& Memory::getInstance()
{
	static Memory memory;
	return memory;
}

Memory::Memory() : maxPhysicalMem(0), maxVirtualMem(0)
{
}

Memory::~Memory()
{
}

void Memory::resetMemUsed()
{
	Memory& memory = Memory::getInstance();
	memory.maxVirtualMem = 0;
	memory.maxPhysicalMem = 0;
}

void Memory::recordMemUsed()
{
	PROCESS_MEMORY_COUNTERS_EX pmc = {};
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	SIZE_T physicalMemUsedByMe = pmc.WorkingSetSize;

	Memory& memory = Memory::getInstance();

	if (virtualMemUsedByMe > memory.maxVirtualMem)
		memory.maxVirtualMem = virtualMemUsedByMe;

	if (physicalMemUsedByMe > memory.maxPhysicalMem)
		memory.maxPhysicalMem = physicalMemUsedByMe;
}

void Memory::printMemUsed()
{
	Memory& memory = Memory::getInstance();

	std::cout << ">>> Max Memory Used <<<\n";
	std::cout << "> Virtual: " << memory.maxVirtualMem << " Bytes\n";
	std::cout << "> Physical: " << memory.maxPhysicalMem << " Bytes\n";
}

void Memory::getMemUsed(SIZE_T& virtualMem, SIZE_T& physicalMem)
{
	Memory& memory = Memory::getInstance();
	virtualMem = memory.maxVirtualMem;
	physicalMem = memory.maxPhysicalMem;
}

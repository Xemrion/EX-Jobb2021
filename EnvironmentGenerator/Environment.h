#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include "Position.h"

class Environment
{
public:
	static Environment* readFromFile(const std::string& filename);

	Environment(int xSize, int ySize, int zSize);
	~Environment();

	bool canVisit(int x, int y, int z) const;
	bool canVisit(Position position) const;
	void setCanVisit(int x, int y, int z, bool canVisit);
	void setCanVisit(Position position, bool canVisit);
	float getAmountVisitable() const;
	int getSizeX() const;
	int getSizeY() const;
	int getSizeZ() const;

	void saveToFile(const std::string& filename) const;

private:
	int xSize;
	int ySize;
	int zSize;
	bool* nodes;
};

#endif // !ENVIRONMENT_H

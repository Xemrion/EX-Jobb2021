#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>

class Environment
{
public:
	static Environment* readFromFile(std::string filename);

	Environment(int xSize, int ySize, int zSize);
	~Environment();

	bool canVisit(int x, int y, int z) const;
	void setCanVisit(int x, int y, int z, bool canVisit);
	float getAmountVisitable() const;
	int getSizeX() const;
	int getSizeY() const;
	int getSizeZ() const;

	void saveToFile(std::string filename) const;

private:
	int xSize;
	int ySize;
	int zSize;
	bool* nodes;
};

#endif // !ENVIRONMENT_H

#include "Environment.h"
#include <algorithm>
#include <fstream>
#include <iostream>

Environment* Environment::readFromFile(const std::string& filename)
{
    std::ifstream outputFile(filename);

    if (outputFile.is_open())
    {
        std::cout << "Loading from file...\n";
        Environment* env = nullptr;

        try
        {
            std::string xSize = "0", ySize = "0", zSize = "0";
            std::getline(outputFile, xSize);
            std::getline(outputFile, ySize);
            std::getline(outputFile, zSize);

            env = new Environment(std::stoi(xSize), std::stoi(ySize), std::stoi(zSize));
            std::string line;

            for (int i = 0; i < env->ySize * env->zSize; i++)
            {
                std::getline(outputFile, line);

                for (int x = 0; x < env->xSize; x++)
                {
                    env->nodes[i * env->xSize + x] = line[x] == '1';
                }
            }

            std::cout << "Loaded environment from: " << filename << "\n";
        }
        catch (const std::exception& exception)
        {
            std::cout << exception.what() << "\n";
        }
        
        outputFile.close();
        return env;
    }

    return nullptr;
}

Environment::Environment(int xSize, int ySize, int zSize) :
    xSize(xSize), ySize(ySize), zSize(zSize)
{
    int length = xSize * ySize * zSize;
    this->nodes = new bool[length];

    std::fill(this->nodes, this->nodes + length, false);
}

Environment::~Environment()
{
    delete[] this->nodes;
}

bool Environment::canVisit(int x, int y, int z) const
{
    if (x >= 0 && x < this->xSize && y >= 0 && y < this->ySize && z >= 0 && z < this->zSize)
    {
        return this->nodes[this->xSize * (this->ySize * z + y) + x];
    }

    return false;
}

bool Environment::canVisit(Position position) const
{
    return this->canVisit(position.x, position.y, position.z);
}

void Environment::setCanVisit(int x, int y, int z, bool canVisit)
{
    if (x >= 0 && x < this->xSize && y >= 0 && y < this->ySize && z >= 0 && z < this->zSize)
    {
        this->nodes[this->xSize * (this->ySize * z + y) + x] = canVisit;
    }
}

void Environment::setCanVisit(Position position, bool canVisit)
{
    return this->setCanVisit(position.x, position.y, position.z, canVisit);
}

void Environment::setCanVisitAll(bool canVisit)
{
    int length = this->xSize * this->ySize * this->zSize;
    std::fill(this->nodes, this->nodes + length, canVisit);
}

float Environment::getAmountVisitable() const
{
    int length = this->xSize * this->ySize * this->zSize;
    int visitable = 0;

    for (int i = 0; i < length; i++)
    {
        if (this->nodes[i])
        {
            visitable++;
        }
    }

    return static_cast<float>(visitable) / length;
}

int Environment::getSizeX() const
{
    return this->xSize;
}

int Environment::getSizeY() const
{
    return this->ySize;
}

int Environment::getSizeZ() const
{
    return this->zSize;
}

void Environment::saveToFile(const std::string& filename) const
{
    std::ofstream outputFile(filename);

    if (outputFile.is_open())
    {
        std::cout << "Saving to file...\n";

        outputFile << std::to_string(this->xSize) << "\n";
        outputFile << std::to_string(this->ySize) << "\n";
        outputFile << std::to_string(this->zSize) << "\n";

        for (int i = 0; i < this->ySize * this->zSize; i++)
        {
            for (int x = 0; x < this->xSize; x++)
            {
                outputFile << std::to_string(this->nodes[i * this->xSize + x]);
            }

            outputFile << "\n";
        }

        outputFile.close();
        std::cout << "Saved environment to: " << filename << "\n";
    }
}

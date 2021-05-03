#include "AlgorithmBacktrack.h"
#include <iostream>
AlgorithmBacktrack::AlgorithmBacktrack()
{
}

AlgorithmBacktrack::~AlgorithmBacktrack()
{
}

bool AlgorithmBacktrack::solveMaze(Environment* env, const Position& current, const Position& end, std::vector<Position>& outpath, std::vector<Position> alreadyChecked) {

    Position neighbour;
    //returns a list of the paths taken
    if (current == end) {
        outpath.insert(outpath.begin(), current);
        std::cout << "End Found"<< "\n";
        return true;
    }
    if (alreadyChecked.size() == 4096) {
        std::cout << "Should be all of them" << "\n";
    }
    for (int i = 0; i < 6; i++) {
        neighbour = current.getNeighbour(static_cast<Position::Direction>(i));
        if (env->canVisit(neighbour)) {
            bool found = false;
            for (int nrInAlready = 0; nrInAlready < alreadyChecked.size(); nrInAlready++) {
                if (alreadyChecked[nrInAlready] == neighbour) {
                    found = true;
                }
            }
            if (found == false) {
                alreadyChecked.insert(alreadyChecked.begin(), neighbour);
                std::cout << "Trying node nr: " << alreadyChecked.size() << "\n";
                
                if (solveMaze(env, neighbour, end, outpath, alreadyChecked)) {
                    outpath.insert(outpath.begin(), current);
                    return true;
                }
            }
            if (found && i == 5) {
                //current = alreadyChecked.at(alreadyChecked.size()-5);
                //solveMaze(env, current, end, outpath, alreadyChecked);
                return false;
            }
        }
    }
    return false;
}

bool AlgorithmBacktrack::pathfind(Environment* env, const Position& start, const Position& end, std::vector<Position>& outPath)
{
    // returns a list of the paths taken
    std::vector<Position> alreadyChecked;
	return solveMaze(env, start, end, outPath, alreadyChecked);
}

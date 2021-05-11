#include "AlgorithmBacktrack.h"
//#include <iostream>

#ifdef MEMORY_TEST_ACTIVE
#include "Memory.h"
#endif // MEMORY_TEST_ACTIVE

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
       // std::cout << "End Found"<< "\n";
        return true;
    }
    /*if (alreadyChecked.size() == 4096) {
        std::cout << "Should be all of them" << "\n";
    }*/

#ifdef MEMORY_TEST_ACTIVE
    Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE

    for (int i = 0; i < 6; i++) {
        neighbour = current.getNeighbour(static_cast<Position::Direction>(i));
        if (env->canVisit(neighbour)) {
            bool found = false;
            for (int nrInAlready = 0; nrInAlready < static_cast<int>(alreadyChecked.size()); nrInAlready++) {
                if (alreadyChecked[nrInAlready] == neighbour) {
                    found = true;
                }
            }
            if (found == false) {
                alreadyChecked.insert(alreadyChecked.begin(), neighbour);
                //std::cout << "Trying node nr: " << alreadyChecked.size() << "\n";

#ifdef MEMORY_TEST_ACTIVE
                Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE
                
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

#ifdef MEMORY_TEST_ACTIVE
        Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE
    }
    return false;
}

bool AlgorithmBacktrack::pathfind(Environment* env, const Position& start, const Position& end, std::vector<Position>& outPath)
{
    bool endFound = false;
    Position current = start;
    std::vector<Position> chain;
    // returns a list of the paths taken
    std::vector<Position> alreadyChecked;
    Position neighbour;
    chain.insert(chain.end(), current);

    int nrOffounds = 0;
    while (endFound != true) {
        // Emergency exit
        if (alreadyChecked.size() > pow(env->getSizeX(), 3)) {
            return false;
        }
        //returns a list of the paths taken
        if (current == end) {
            //std::cout << "Finished" << "\n";
            //outPath.insert(outPath.begin(), current);
            outPath = chain;
            // std::cout << "End Found"<< "\n";
            return true;
            
        }
        /*if (alreadyChecked.size() == 4096) {
            std::cout << "Should be all of them" << "\n";
        }*/

#ifdef MEMORY_TEST_ACTIVE
        Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE
        int testPossible = 0;
        Position testNeighbur;
        for (int j = 0; j < 6; j++) {
            testNeighbur = current.getNeighbour(static_cast<Position::Direction>(j));
            if (env->canVisit(testNeighbur)) {
                testPossible++;
            }
        }
        //std::cout << "Nr of possible: " << testPossible<< "\n";
        for (int i = 0; i < 6; i++) {
            neighbour = current.getNeighbour(static_cast<Position::Direction>(i));
            if (env->canVisit(neighbour)) {
                bool found = false;
                for (int nrInAlready = 0; nrInAlready < static_cast<int>(alreadyChecked.size()); nrInAlready++) {
                    if (alreadyChecked[nrInAlready] == neighbour) {
                        found = true;
                    }
                }
                if (found == false) {
                    //alreadyChecked.insert(alreadyChecked.begin(), neighbour);
                    //std::cout << "Trying node nr: " << alreadyChecked.size() << "\n";

#ifdef MEMORY_TEST_ACTIVE
                    Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE
                    alreadyChecked.insert(alreadyChecked.begin(), neighbour);
                    chain.insert(chain.end(), neighbour);
                    current = neighbour;
                    i = 5;
                    nrOffounds = 0;
                }
                else if (found) 
                {
                    nrOffounds++;
                    if (nrOffounds >= testPossible) {
                        //std::cout << "Reject" << "\n";
                        chain.erase(chain.begin() + chain.size() - 1);
                        current = chain.at(chain.size() - 1);
                        //std::cout << "Going to pos: " << chain.size() - 1 << "\n";
                        nrOffounds = 0;
                    }
                    else {
                        //std::cout << "Wait: "<< nrOffounds << " Possible: " << testPossible << "\n";
                    }
                }
            }

#ifdef MEMORY_TEST_ACTIVE
            Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE
        }
        //std::cout << "new Loop" << "\n";
    }
    return false;

	//return solveMaze(env, start, end, outPath, alreadyChecked);
}

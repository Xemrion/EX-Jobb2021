#include "AlgorithmDijkstras.h"
#include <set>
#include <unordered_set>
//#include <iostream>

#ifdef MEMORY_TEST_ACTIVE
#include "Memory.h"
#endif // MEMORY_TEST_ACTIVE

AlgorithmDijkstras::AlgorithmDijkstras()
{
}

AlgorithmDijkstras::~AlgorithmDijkstras()
{
}

bool AlgorithmDijkstras::pathfind(Environment* env, const Position& start, const Position& end, std::vector<Position>& outPath)
{
	if (!env || !env->canVisit(start) || !env->canVisit(end)) {
		//std::cout << "Impossible" << "\n";
		return false;
	}

	std::vector<Node*> totalSet;
	Node* currentNode = nullptr;
	Position neighbourPos;
	int visitedCount = 0;
	int nrOfAvailableNodes = 1;

	bool allmarked = false;
	bool endFound = false;
	int endPosInList = -1;

	//Add start node to the open set.
	Node* startNode = new Node(start);
	totalSet.push_back(startNode);
	currentNode = startNode;
	currentNode->hCost = 1000;

#ifdef MEMORY_TEST_ACTIVE
	Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE

	while (visitedCount < nrOfAvailableNodes/* && endFound == false*/)
	{
		for (int i = 0; i < 6; i++)
		{
			neighbourPos = currentNode->pos.getNeighbour(static_cast<Position::Direction>(i));

			if (env->canVisit(neighbourPos))
			{
				bool found = false;
				int posOfFound = 0;
				for (int j = 0; j < static_cast<int>(totalSet.size()) && !found; j++) {
					if (totalSet.at(j)->pos == neighbourPos) {
						found = true;
						posOfFound = j;
						break;
					}
				}
				float gCost = currentNode->gCost + currentNode->pos.distanceTo(neighbourPos);
				if (found){
					if (totalSet.at(posOfFound)->hCost > (neighbourPos.distanceTo(end) + currentNode->hCost)) {
					}
					totalSet.at(posOfFound)->gCost = gCost;
				}
				else if(!found){
					//lägg till
					float hCost = neighbourPos.distanceTo(end);// +currentNode->hCost;
					nrOfAvailableNodes++;
					totalSet.push_back(new Node(neighbourPos, gCost + hCost, gCost, hCost, currentNode));
					
				}
			}
			if (i == 5) {
				currentNode->visited = true;
				visitedCount++;
				float minCost = 1000.0f;
				int posOfMin = -1;
				for (int next = 0; next < static_cast<int>(totalSet.size()); next++) {
					if (totalSet.at(next) != currentNode && totalSet.at(next)->gCost != 0 && !totalSet.at(next)->visited){
						if (totalSet.at(next)->gCost < minCost) {
							minCost = totalSet.at(next)->gCost;
							posOfMin = next;
						}
						if (totalSet.at(next)->pos == end) {
							endFound = true;
							//std::cout << "End Node is: " << next << " Position: " << totalSet.at(next)->pos.x << "," << totalSet.at(next)->pos.y << "," << totalSet.at(next)->pos.z << "\n";
							endPosInList = next;
						}
					}

#ifdef MEMORY_TEST_ACTIVE
					Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE
				}
				if (posOfMin == -1) {
					//std::cout << "No unvisited node found " << "\n";
				}
				else {
					currentNode = totalSet.at(posOfMin);
					//std::cout << "nr of nodes visited: " << totalSet.size() << " nr of nodes visitable :" << nrOfAvailableNodes << "\n";
				}
			}


#ifdef MEMORY_TEST_ACTIVE
			Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE
		}
	}

	//std::cout << "################ Completed. ################\n";
	//std::cout << "nr of nodes visited: " << visitedCount <<"\n";

	if (endPosInList > 0) {
		currentNode = totalSet.at(0);
		currentNode->included = true;
		outPath.insert(outPath.begin(), currentNode->pos);
		float minGCost = 1000;
		float LastGCost = minGCost;
		int posOfMinCost = -1;
		int nrInOutPath = 0;
		int currentPosInList = 0;
		while (totalSet.at(currentPosInList)->hCost != 0) {
			for (int i = 0; i < 6; i++) {
				neighbourPos = totalSet.at(currentPosInList)->pos.getNeighbour(static_cast<Position::Direction>(i));
				if (env->canVisit(neighbourPos)) {
					for (int j = 0; j < static_cast<int>(totalSet.size()); j++) {
						if (totalSet.at(j)->pos == neighbourPos && totalSet.at(j)->hCost < minGCost && !totalSet.at(j)->included) {
							minGCost = totalSet.at(j)->hCost;
							posOfMinCost = j;
						}
						else if (totalSet.at(j)->pos == neighbourPos && totalSet.at(j)->hCost == minGCost && !totalSet.at(j)->included) {
							minGCost = totalSet.at(j)->hCost;
							posOfMinCost = j;
						}
					}
				}

#ifdef MEMORY_TEST_ACTIVE
				Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE
			}
			if (LastGCost != minGCost) {
				LastGCost = minGCost;
				currentPosInList = posOfMinCost;
				outPath.insert(outPath.begin(), totalSet.at(currentPosInList)->pos);
				totalSet.at(currentPosInList)->included = true;
				nrInOutPath++;
				//std::cout << "nr of nodes added to path: " << nrInOutPath << " Cost of Node: " << currentNode->hCost << "\n";
			}
			else {
				//if (currentPosInList == 0) {
				//	std::cout << "Stopped" << "\n";
				//	return false;
				//}
				totalSet.at(currentPosInList)->hCost += 10;
				currentPosInList = 0;
				minGCost = 1000;
				LastGCost = 1000;
				std::cout << "Reset" << "\n";
				for (int i = 0; i < totalSet.size(); i++) {
					totalSet.at(i)->included = false;
				}
				outPath.clear();
				outPath.insert(outPath.begin(), totalSet.at(currentPosInList)->pos);
			}

#ifdef MEMORY_TEST_ACTIVE
			Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE
		}
		

		//Delete the nodes.
		for (Node* node : totalSet)
			delete node;

		return true;
	}

#ifdef MEMORY_TEST_ACTIVE
	Memory::recordMemUsed();
#endif // MEMORY_TEST_ACTIVE

	//Delete the nodes.
	for (Node* node : totalSet)
		delete node;

	return false;
}

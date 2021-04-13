#include "AlgorithmDijkstras.h"
#include <set>
#include <unordered_set>
#include <iostream>
AlgorithmDijkstras::AlgorithmDijkstras()
{
}

AlgorithmDijkstras::~AlgorithmDijkstras()
{
}

bool AlgorithmDijkstras::pathfind(Environment* env, const Position& start, const Position& end, std::vector<Position>& outPath)
{
	if (!env || !env->canVisit(start) || !env->canVisit(end))
		return false;

	std::multiset<Node*, NodeComparer> openSet;
	std::vector<Node*> closedSet;
	std::vector<Node*> totalSet;
	Node* currentNode = nullptr;
	Position neighbourPos;
	int visitedCount = 0;
	int nrOfAvailableNodes = 1;

	bool allmarked = false;

	//Add start node to the open set.
	//openSet.insert();
	Node* startNode = new Node(start);
	totalSet.push_back(startNode);
	currentNode = startNode;
	while (visitedCount < nrOfAvailableNodes)
	{
		//Move node to the closed set.
		//currentNode = *openSet.begin();
		//closedSet.push_back(currentNode);
		//totalSet.push_back(currentNode);
		//openSet.erase(openSet.begin());
		
		for (int i = 0; i < 6; i++)
		{
			neighbourPos = currentNode->pos.getNeighbour(static_cast<Position::Direction>(i));

			if (env->canVisit(neighbourPos))
			{
				bool found = false;
				int posOfFound = 0;
				for (int j = 0; j < totalSet.size() && !found; j++) {
					if (totalSet.at(j)->pos == neighbourPos) {
						found;
						posOfFound = j;
						break;
					}
				}
				float gCost = currentNode->gCost + currentNode->pos.distanceTo(neighbourPos);
				if (found && gCost < totalSet.at(posOfFound)->gCost) {
					totalSet.at(posOfFound)->gCost = gCost;
				}
				else if(!found){
					//lägg till
					float hCost = neighbourPos.distanceTo(end);
					nrOfAvailableNodes++;
					totalSet.push_back(new Node(neighbourPos, gCost + hCost, gCost, hCost, currentNode));
					
				}
			}
			if (i == 5) {
				currentNode->visited = true;
				visitedCount++;
				int minCost = 1000;
				int posOfMin = 0;
				for (int next = 0; next < totalSet.size(); next++) {
					if (totalSet.at(next)->gCost < minCost && totalSet.at(next) != currentNode && totalSet.at(next)->gCost != 0 && !totalSet.at(0)->visited){
						minCost = totalSet.at(next)->gCost;
						posOfMin = next;
					}
				}
				currentNode = totalSet.at(posOfMin);
				std::cout << "nr of nodes visited: " << totalSet.size() << "nr of nodes visitable :" << nrOfAvailableNodes << "\n";
			}
		}
	}

	std::cout << "################ Completed. ################\n";
	std::cout << "nr of nodes visited: " << visitedCount <<"\n";

	//Delete the nodes.
	for (Node* node : openSet)
		delete node;
	for (Node* node : closedSet)
		delete node;

	return false;
}

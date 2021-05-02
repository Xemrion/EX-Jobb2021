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
	bool endFound = false;
	int endPosInList = -1;

	//Add start node to the open set.
	//openSet.insert();
	Node* startNode = new Node(start);
	totalSet.push_back(startNode);
	currentNode = startNode;
	currentNode->hCost = 1000;
	while (visitedCount < nrOfAvailableNodes/* && endFound == false*/)
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
						found = true;
						posOfFound = j;
						break;
					}
				}
				float gCost = currentNode->gCost + currentNode->pos.distanceTo(neighbourPos);
				if (found){
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
				int posOfMin = -1;
				for (int next = 0; next < totalSet.size(); next++) {
					if (totalSet.at(next) != currentNode && totalSet.at(next)->gCost != 0 && !totalSet.at(next)->visited){
						if (totalSet.at(next)->gCost < minCost) {
							minCost = totalSet.at(next)->gCost;
							posOfMin = next;
						}
						if (totalSet.at(next)->pos == end) {
							endFound = true;
							std::cout << "End Node is: " << next << " Position: " << totalSet.at(next)->pos.x << "," << totalSet.at(next)->pos.y << "," << totalSet.at(next)->pos.z << "\n";
							endPosInList = next;
						}
					}
				}
				if (posOfMin == -1) {
					std::cout << "No unvisited node found " << "\n";
				}
				else {
					currentNode = totalSet.at(posOfMin);
					std::cout << "nr of nodes visited: " << totalSet.size() << " nr of nodes visitable :" << nrOfAvailableNodes << "\n";
				}
			}
		}
	}

	std::cout << "################ Completed. ################\n";
	std::cout << "nr of nodes visited: " << visitedCount <<"\n";

	if (endPosInList > 0) {
		/*do
		{
			outPath.insert(outPath.begin(), currentNode->pos);
			currentNode = currentNode->parent;
		} while (currentNode);*/
		currentNode = totalSet.at(0);
		currentNode->included = true;
		outPath.insert(outPath.begin(), currentNode->pos);
		float minGCost = 1000;
		float LastGCost = minGCost;
		int posOfMinCost = -1;
		int nrInOutPath = 0;
		while (currentNode->hCost != 0) {
			for (int i = 0; i < 6; i++) {
				neighbourPos = currentNode->pos.getNeighbour(static_cast<Position::Direction>(i));
				if (env->canVisit(neighbourPos)) {
					for (int j = 0; j < totalSet.size(); j++) {
						if (totalSet.at(j)->pos == neighbourPos && totalSet.at(j)->hCost < minGCost && !totalSet.at(j)->included) {
							minGCost = totalSet.at(j)->hCost;
							posOfMinCost = j;
						}
					}
				}
			}
			if (LastGCost != minGCost) {
				LastGCost = minGCost;
				currentNode = totalSet.at(posOfMinCost);
				outPath.insert(outPath.begin(), currentNode->pos);
				currentNode->included = true;
				nrInOutPath++;
				std::cout << "nr of nodes added to path: " << nrInOutPath << " Cost of Node: " << currentNode->hCost << "\n";
			}
			else {
				std::cout << "No new nodes: " << "\n";
			}
			
		}
		

		//Delete the nodes.
		for (Node* node : openSet)
			delete node;
		for (Node* node : closedSet)
			delete node;
		for (Node* node : totalSet)
			delete node;

		return true;
	}

	//Delete the nodes.
	for (Node* node : openSet)
		delete node;
	for (Node* node : closedSet)
		delete node;
	for (Node* node : totalSet)
		delete node;


	return false;
}

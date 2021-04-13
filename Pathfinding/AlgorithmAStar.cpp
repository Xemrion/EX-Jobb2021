#include "AlgorithmAStar.h"
#include <set>
#include <map>

AlgorithmAStar::AlgorithmAStar()
{
}

AlgorithmAStar::~AlgorithmAStar()
{
}

bool AlgorithmAStar::pathfind(Environment* env, const Position& start, const Position& end, std::vector<Position>& outPath)
{
	if (!env || !env->canVisit(start) || !env->canVisit(end))
		return false;

	std::multiset<Node*, NodeComparer> openSetNodes; //Sorted by lowest cost.
	std::map<Position, Node*> openSet; //Sorted by position.
	std::map<Position, Node*> closedSet; //Sorted by position.
	Node* currentNode = nullptr;
	Node* neighbour = nullptr;
	Position neighbourPos;
	std::map<Position, Node*>::iterator iterator;

	//Add start node to the open set.
	openSetNodes.insert(new Node(start));
	openSet[start] = *openSetNodes.begin();

	while (!openSetNodes.empty())
	{
		//Move node with lowest cost to the closed set.
		currentNode = *openSetNodes.begin();

		closedSet[currentNode->pos] = currentNode;
		openSet.erase(currentNode->pos);
		openSetNodes.erase(openSetNodes.begin());

		//Is the node the end pos? Retrace to get the full path.
		if (currentNode->pos == end)
		{
			outPath.clear();

			do
			{
				outPath.insert(outPath.begin(), currentNode->pos);
				currentNode = currentNode->parent;
			}
			while (currentNode);

			//Delete the nodes.
			for (auto& pair : openSet)
				delete pair.second;
			for (auto& pair : closedSet)
				delete pair.second;

			return true;
		}

		//Add neighbours the the open set.
		for (int i = 0; i < 6; i++)
		{
			neighbourPos = currentNode->pos.getNeighbour(static_cast<Position::Direction>(i));

			if (env->canVisit(neighbourPos))
			{
				//Find the node if it already exists in the closed set.
				iterator = closedSet.find(neighbourPos);

				if (iterator == closedSet.end())
				{
					float gCost = currentNode->gCost + currentNode->pos.distanceTo(neighbourPos);

					//Find the node if it already exists in the open set.
					iterator = openSet.find(neighbourPos);

					if (iterator == openSet.end())
					{
						//If the node doesn't exist:
						float hCost = neighbourPos.distanceTo(end);
						neighbour = new Node(neighbourPos, gCost + hCost, gCost, hCost, currentNode);

						openSet[neighbourPos] = neighbour;
						openSetNodes.insert(neighbour);
					}
					else
					{
						//If the node exists:
						neighbour = iterator->second;

						//Only update the node if it would get a lesser gCost.
						if (gCost < neighbour->gCost)
						{
							neighbour->gCost = gCost;
							neighbour->fCost = neighbour->gCost + neighbour->hCost;
							neighbour->parent = currentNode;
						}
					}
				}
			}
		}
	}

	//Delete the nodes.
	for (auto& pair : openSet)
		delete pair.second;
	for (auto& pair : closedSet)
		delete pair.second;

	return false;
}

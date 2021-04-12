#include "AlgorithmAStar.h"
#include <set>
#include <unordered_set>

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

	std::multiset<Node*, NodeComparer> openSet;
	std::vector<Node*> closedSet;
	Node* currentNode = nullptr;
	Position neighbourPos;

	//Add start node to the open set.
	openSet.insert(new Node(start));

	while (!openSet.empty())
	{
		//Move node to the closed set.
		currentNode = *openSet.begin();
		closedSet.push_back(currentNode);
		openSet.erase(openSet.begin());

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
			for (Node* node : openSet)
				delete node;
			for (Node* node : closedSet)
				delete node;

			return true;
		}

		//Add neighbours the the open set.
		for (int i = 0; i < 6; i++)
		{
			neighbourPos = currentNode->pos.getNeighbour(static_cast<Position::Direction>(i));

			if (env->canVisit(neighbourPos))
			{
				//Find the node if it already exists in the closed set.
				auto neighbourIter = std::find_if(closedSet.begin(), closedSet.end(),
					[&neighbourPos](Node* node) { return node->pos == neighbourPos; }
				);

				if (neighbourIter == closedSet.end())
				{
					//Find the node if it already exists in the open set.
					auto neighbourIter2 = std::find_if(openSet.begin(), openSet.end(),
						[&neighbourPos](Node* node) { return node->pos == neighbourPos; }
					);

					float gCost = currentNode->gCost + currentNode->pos.distanceTo(neighbourPos);

					if (neighbourIter2 == openSet.end())
					{
						//If the node doesn't exist: add it!
						float hCost = neighbourPos.distanceTo(end);
						openSet.insert(new Node(neighbourPos, gCost + hCost, gCost, hCost, currentNode));
					}
					else
					{
						Node* neighbour = *neighbourIter2;

						if (gCost < neighbour->gCost)
						{
							//If the node exists: only change it if it would get a lesser gCost.
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
	for (Node* node : openSet)
		delete node;
	for (Node* node : closedSet)
		delete node;

	return false;
}

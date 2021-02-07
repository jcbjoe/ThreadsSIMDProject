#include "GameManager.h"
#include "Dino.h"
#include <iostream>

Dino::Dino(int x, int y, sf::Texture& tex) : position(x, y), _currentPathPos(0) {

	_sprite.setTexture(tex);

	_sprite.setScale(DINOSCALE, DINOSCALE);

	_animationNum = 4 + (rand() % 5);

	int startingVelocity = 1;

	float velocityX = -startingVelocity;
	float velocityY = -startingVelocity;

	int velocityXRand = (rand() % 2);
	int velocityYRand = (rand() % 2);

	if (velocityXRand == 1) velocityX = startingVelocity;
	if (velocityYRand == 1) velocityY = startingVelocity;

	velocity = sf::Vector2i(velocityX, velocityY);

}

//https://en.sfml-dev.org/forums/index.php?topic=6180.0
sf::Vector2f Interpolate(const sf::Vector2f& pointA, const sf::Vector2f& pointB, float factor) {
	if (factor > 1.f)
		factor = 1.f;

	else if (factor < 0.f)
		factor = 0.f;

	return pointA + (pointB - pointA) * factor;
}

void Dino::Update(float dTime) {

	auto dest = GameManager::GetInstance()->GetDestination();
	if (dest != nullptr) {
		if (_isFollowingPath) {

			auto currentWaypointPos = sf::Vector2f(_currentPath.at(_currentPathPos).x, _currentPath.at(_currentPathPos).y);

			auto distance = ManhattanFinder(position, currentWaypointPos);
			auto movespeed = 100;

			position = Interpolate(position, currentWaypointPos, (movespeed * dTime) / distance);

			if (distance == 0) {
				if (_currentPathPos == (_currentPath.size() - 1)) {
					//Reached end
				}
				else {
					_currentPathPos++;
				}
			}
		}
		else {
			auto path = findPath(*dest);
			if (path.size() != 0) {
				_currentPath = path;
				_isFollowingPath = true;
				_currentPathPos = 0;
				for (auto vec : path) {
					auto tile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(vec.x, vec.y);
					if (tile != nullptr && tile->GetColour() != sf::Color::Black) {
						tile->SetColour(sf::Color::Red);
					}
				}
			}
		}
	}
	else {
		if (_isFollowingPath) {
			_isFollowingPath = false;

			for (auto vec : _currentPath) {
				auto tile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(vec.x, vec.y);
				if (tile != nullptr && tile->GetColour() != sf::Color::Black) {

					tile->SetColour(sf::Color::White);
				}
			}
		}

		const float minX = 0;
		const float minY = 0;
		const float maxX = (SCREENX - (DINOSIZEX / 2));
		const float maxY = (SCREENY - (DINOSIZEY / 2));
		const float speed = 100;
		if (position.x <= minX) {
			velocity.x = 1.0f;
		}
		else if (position.x >= maxX) {
			velocity.x = -1.0f;
		}

		if (position.y <= minY) {
			velocity.y = 1.0f;
		}
		else if (position.y >= maxY) {
			velocity.y = -1.0f;
		}
		auto speedTime = (speed * dTime);
		auto xVelocity = speedTime * velocity.x;
		auto yVelocity = speedTime * velocity.y;

		position.x += xVelocity;
		position.y += yVelocity;

	}

	_sprite.setPosition(position.x, position.y);

	_sprite.setScale(_scale, _scale);

	if (_clock.getElapsedTime().asSeconds() > 0.1f) {

		if (_animationNum == 9) {
			_animationNum = 4;
		}
		else {
			_animationNum++;
		}
		_clock.restart();
	}

	if (velocity.x > 0) _sprite.setTextureRect(sf::IntRect(24 * _animationNum, 0, 24, 24));
	else _sprite.setTextureRect(sf::IntRect(24 * _animationNum, 0, -24, 24));
}

void Dino::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite);
}


std::vector<sf::Vector2i> Dino::findPath(sf::Vector2f dest) {

	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	std::vector<Node*> completeList;

	auto clampedStartX = std::clamp((int)position.x, 0, SCREENX - 1);
	auto clampedStartY = std::clamp((int)position.y, 0, SCREENY - 1);
	auto clampedPos = sf::Vector2f(clampedStartX, clampedStartY);

	auto startTile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(clampedStartX, clampedStartY);
	auto startTilePos = startTile->GetPosition();
	auto endTile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(dest.x, dest.y);
	sf::Vector2i endTilePos = endTile->GetPosition();

	Node* startNode = new Node(startTilePos.x, startTilePos.y, 0, ManhattanFinder(clampedPos, dest), nullptr);

	Node* destNode = new Node(endTilePos.x, endTilePos.y, 0, 0, nullptr);

	_allNodes.push_back(startNode);
	_allNodes.push_back(destNode);
	openList.push_back(startNode);

	do {

		Node* currentNode = GetLowestFScore(openList);

		closedList.push_back(currentNode);
		RemoveFromVector(currentNode, openList);

		if (VectorContains(destNode, closedList)) {
			Node* tmp = currentNode;

			do {

				completeList.push_back(tmp);
				tmp = tmp->parent;

			} while (tmp != nullptr);

			break;
		}

		std::vector<Node*> adjacentNodes = GetAdjacentNodes(currentNode, destNode);

		for (int i = 0; i < adjacentNodes.size(); ++i) {
			if (VectorContains(adjacentNodes.at(i), closedList)) {
				continue;
			}

			if (!(VectorContains(adjacentNodes.at(i), openList))) {
				openList.push_back(adjacentNodes.at(i));
			}
			else {
				if ((currentNode->gScore + 1) < adjacentNodes.at(i)->gScore) {
					Node* newNode = adjacentNodes.at(i);
					newNode->gScore = currentNode->gScore + 1;

					RemoveFromVector(adjacentNodes.at(i), openList);

					openList.push_back(newNode);
				}
			}
		}

	} while (!openList.empty());

	std::vector<sf::Vector2i> pathToReturn;

	for (int i = completeList.size(); i != 0; --i) {
		int index = i - 1;
		pathToReturn.push_back(sf::Vector2i(completeList.at(index)->x, completeList.at(index)->y));
	}

	// Clean up vectors and memory
	openList.clear();
	closedList.clear();
	completeList.clear();

	for (Node* node : _allNodes) {
		delete node;
	}

	_allNodes.clear();

	return pathToReturn;
}

Node* Dino::GetLowestFScore(std::vector<Node*> openList) {
	Node* nodeToReturn = nullptr;

	int fScoreLowest = NULL;

	for (Node* node : openList) {
		if (fScoreLowest == NULL) {
			fScoreLowest = node->gScore + node->hScore;
			nodeToReturn = node;
		}
		else {
			int fScore = node->gScore + node->hScore;
			if (fScore < fScoreLowest) {
				nodeToReturn = node;
				fScoreLowest = fScore;
			}
		}
	}

	return nodeToReturn;
}


void Dino::RemoveFromVector(Node* nodeToRemove, std::vector<Node*>& vector) {
	for (int i = 0; i < vector.size(); i++) {
		if (vector.at(i)->x == nodeToRemove->x && vector.at(i)->y == nodeToRemove->y) {
			vector.erase(vector.begin() + i);
		}
	}
}

bool Dino::VectorContains(Node* containsNode, std::vector<Node*> vector) {
	for (Node* node : vector) {
		if (node->x == containsNode->x && node->y == containsNode->y) {
			return true;
		}
	}

	return false;
}

std::vector<Node*> Dino::GetAdjacentNodes(Node* node, Node* dest) {
	std::vector<Node*> adjacentNodes;

	bool aboveBool = false;
	bool belowBool = false;
	bool leftBool = false;
	bool rightBool = false;

	sf::Vector2f pos;

	pos = sf::Vector2f(node->x, node->y + GRID_WIDTH);
	Tile* aboveTile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(pos.x, pos.y);
	if (aboveTile != nullptr && aboveTile->GetColour() != sf::Color::Black) {
		Node* above = new Node(pos.x, pos.y, node->gScore + 1, ManhattanFinder(pos, sf::Vector2f(dest->x, dest->y)), node);
		adjacentNodes.push_back(above);
		_allNodes.push_back(above);
		aboveBool = true;
	}

	pos = sf::Vector2f(node->x, node->y - GRID_WIDTH);
	Tile* belowTile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(pos.x, pos.y);
	if (belowTile != nullptr && belowTile->GetColour() != sf::Color::Black) {
		Node* below = new Node(pos.x, pos.y, node->gScore + 1, ManhattanFinder(pos, sf::Vector2f(dest->x, dest->y)), node);
		adjacentNodes.push_back(below);
		_allNodes.push_back(below);
		belowBool = true;
	}

	pos = sf::Vector2f(node->x - GRID_WIDTH, node->y);
	Tile* leftTile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(pos.x, pos.y);
	if (leftTile != nullptr && leftTile->GetColour() != sf::Color::Black) {
		Node* left = new Node(pos.x, pos.y, node->gScore + 1, ManhattanFinder(pos, sf::Vector2f(dest->x, dest->y)), node);
		adjacentNodes.push_back(left);
		_allNodes.push_back(left);
		leftBool = true;
	}

	pos = sf::Vector2f(node->x + GRID_WIDTH, node->y);
	Tile* rightTile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(pos.x, pos.y);
	if (rightTile != nullptr && rightTile->GetColour() != sf::Color::Black) {
		Node* right = new Node(pos.x, pos.y, node->gScore + 1, ManhattanFinder(pos, sf::Vector2f(dest->x, dest->y)), node);
		adjacentNodes.push_back(right);
		_allNodes.push_back(right);
		rightBool = true;
	}

	pos = sf::Vector2f(node->x - GRID_WIDTH, node->y - GRID_WIDTH);
	Tile* topLeftTile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(pos.x, pos.y);
	if (topLeftTile != nullptr && aboveBool && leftBool && topLeftTile->GetColour() != sf::Color::Black) {
		Node* topLeft = new Node(pos.x, pos.y, node->gScore + 1, ManhattanFinder(pos, sf::Vector2f(dest->x, dest->y)), node);
		adjacentNodes.push_back(topLeft);
		_allNodes.push_back(topLeft);
	}

	pos = sf::Vector2f(node->x + GRID_WIDTH, node->y - GRID_WIDTH);
	Tile* topRightTile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(pos.x, pos.y);
	if (topRightTile != nullptr && aboveBool && rightBool && topRightTile->GetColour() != sf::Color::Black) {
		Node* topRight = new Node(pos.x, pos.y, node->gScore + 1, ManhattanFinder(pos, sf::Vector2f(dest->x, dest->y)), node);
		adjacentNodes.push_back(topRight);
		_allNodes.push_back(topRight);
	}

	pos = sf::Vector2f(node->x - GRID_WIDTH, node->y + GRID_WIDTH);
	Tile* bottomLeftTile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(pos.x, pos.y);
	if (bottomLeftTile != nullptr && belowBool && leftBool && bottomLeftTile->GetColour() != sf::Color::Black) {
		Node* bottomLeft = new Node(pos.x, pos.y, node->gScore + 1, ManhattanFinder(pos, sf::Vector2f(dest->x, dest->y)), node);
		adjacentNodes.push_back(bottomLeft);
		_allNodes.push_back(bottomLeft);
	}

	pos = sf::Vector2f(node->x + GRID_WIDTH, node->y + GRID_WIDTH);
	Tile* bottomRightTile = GameManager::GetInstance()->GetGrid()->GetTileAtPos(pos.x, pos.y);
	if (bottomRightTile != nullptr && belowBool && rightBool && bottomRightTile->GetColour() != sf::Color::Black) {
		Node* bottomRight = new Node(pos.x, pos.y, node->gScore + 1, ManhattanFinder(pos, sf::Vector2f(dest->x, dest->y)), node);
		adjacentNodes.push_back(bottomRight);
		_allNodes.push_back(bottomRight);
	}

	return adjacentNodes;

}
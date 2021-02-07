#pragma once
#include <SFML/Graphics.hpp>

struct Node {
	int x;
	int y;
	int gScore;
	int hScore;
	Node* parent;

	Node(int _x, int _y, int _gScore, int _hScore, Node* _parent) : x(_x), y(_y), gScore(_gScore), hScore(_hScore), parent(_parent) {

	}
};

class Dino : public sf::Drawable {
public:

	Dino(int x, int y, sf::Texture& tex);

	void Update(float dTime);

	sf::Sprite GetSprite() { return _sprite; };

	sf::Vector2f position;
	sf::Vector2i velocity = sf::Vector2i(0, 0);

	bool isFollowingPath() { return _isFollowingPath; };

private:

	sf::Clock _clock;

	int _scale = 2;

	sf::Sprite _sprite;

	int _animationNum;

	bool _isFollowingPath = false;

	std::vector<sf::Vector2i> _currentPath;
	int _currentPathPos;

	std::vector<Node*> _allNodes;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::vector<sf::Vector2i> findPath(sf::Vector2f dest);

	float ManhattanFinder(sf::Vector2f a, sf::Vector2f b) { return abs(a.x - b.x) + abs(a.y - b.y); };

	Node* GetLowestFScore(std::vector<Node*> openList);

	void RemoveFromVector(Node* nodeToRemove, std::vector<Node*>& vector);

	bool VectorContains(Node* containsNode, std::vector<Node*> vector);

	std::vector<Node*> GetAdjacentNodes(Node* node, Node* destination);

};




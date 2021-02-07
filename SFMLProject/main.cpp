#include <SFML/Graphics.hpp>
#include <iostream>
#include <numeric>
#include "Config.h"
#include "GameManager.h"


int main()
{
	// Disabling the resizing for now - Will mess up the grid and not needed for this proof of concept
	sf::RenderWindow window(sf::VideoMode(SCREENX, SCREENY), "DINOS!!", sf::Style::Titlebar | sf::Style::Close);

	srand(time(NULL));

	sf::Font font;
	if (!font.loadFromFile("Roboto-Regular.ttf"))
	{
		throw new std::exception("Failed to load font");
	}

	sf::Text text;

	text.setFont(font);

	text.setFillColor(sf::Color::Blue);

	sf::Clock clock;

	std::vector<float> fpsVec;

	while (window.isOpen())
	{

		float dTime = clock.restart().asSeconds();

		float fps = 1.0 / dTime;

		fpsVec.push_back(fps);

		text.setString("FPS: " + std::to_string(fps));

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2f loc = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
					GameManager::GetInstance()->SetDestination(&loc);
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					GameManager::GetInstance()->SetDestination(nullptr);
				}
				if (event.mouseButton.button == sf::Mouse::Middle)
				{
					float avg = std::accumulate(fpsVec.begin(), fpsVec.end(), 0.0) / fpsVec.size();
					auto lowest = *std::min_element(fpsVec.begin(), fpsVec.end());
					auto highest = *std::max_element(fpsVec.begin(), fpsVec.end());
					std::cout << "Avg FPS: " << avg << std::endl << "Min FPS: " << lowest << std::endl << "Max FPS: " << highest << std::endl;
				}
			}
		}

		window.clear(sf::Color::White);

		GameManager::GetInstance()->Update(dTime);
		GameManager::GetInstance()->Draw(dTime, window);

		window.draw(text);


		window.display();

	}

	return 0;
}

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Board.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gomoku");

	Board board(19, &window);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        		window.setView(sf::View(visibleArea));
				board.resize();
			}
        }
        window.clear();
		board.draw();
        window.display();
    }

    return 0;
}
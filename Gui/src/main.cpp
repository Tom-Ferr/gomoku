#include <iostream>
#include <SFML/Graphics.hpp>

#include <Board.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Gomoku");

	Board board(19, &window);
    while (window.isOpen())
    {
        sf::Event event;
        if (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        		window.setView(sf::View(visibleArea));
				board.resize();
			}
			if (event.type == sf::Event::MouseMoved && board.enabled())
				board.hover();
			if (event.type == sf::Event::MouseButtonPressed && board.enabled())
				board.click();
        }
        window.clear();
		board.draw();
        window.display();
    }

    return 0;
}
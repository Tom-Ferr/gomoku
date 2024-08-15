#include <iostream>
#include <SFML/Graphics.hpp>

#include <Board.hpp>
#include <Heuristics.hpp>
#include <Free_Three_Checker.hpp>

int main()
{
	std::srand(42);
	Free_Three_Checker::set_masks(6, 19);
	Heuristics::set_masks(5, 19);
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Gomoku");
	window.setVerticalSyncEnabled(true);
	Gui gui(&window);

	Board board(19, &window);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
                window.close();
			else if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        		window.setView(sf::View(visibleArea));
				board.resize();
			}
			else if (event.type == sf::Event::MouseMoved && board.enabled())
				board.hover();
			else if (event.type == sf::Event::MouseButtonPressed && board.enabled())
				board.click();
        }
        window.clear();
		gui.draw();
		board.draw();
        window.display();
    }

    return 0;
}
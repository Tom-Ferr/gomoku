#ifndef BOARD_HPP
# define BOARD_HPP

# include <SFML/Graphics.hpp>
# include "Tile.hpp"

class Tile;

class Board
{
	public:
		Board(size_t sqrt, sf::RenderWindow* window);
		Board();
		Board(Board const &other);
		~Board();
		Board &operator=(Board const &other);
		void resize();
		void draw();
		size_t sqrt();
		sf::Vector2f const &get_dimensions() const;
		sf::Vector2f const &get_position() const;

	private:
		size_t				_sqrt;
		size_t				_size;
		sf::RectangleShape	_shape;
		sf::RenderWindow*	_window;
		sf::Vector2f		_dimensions;
		sf::Vector2f		_position;
		std::vector<Tile>	_tiles;
};

#endif

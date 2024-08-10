#ifndef TILE_HPP
# define TILE_HPP

# include <iostream>
# include <SFML/Graphics.hpp>
# include <Board.hpp>

class Board;

class Tile
{
    public:
		Tile();
		Tile(Board *board, size_t pos = 0);
		Tile(Tile const &other);
		~Tile();
		Tile &operator=(Tile const &other);
		void resize();
		void draw(sf::RenderWindow& window);


    private:
		Board				*_board;
		size_t				_pos;
		sf::Vector2f		_dimensions;
		sf::Vector2f		_position;
		sf::RectangleShape	_shape;
};


#endif

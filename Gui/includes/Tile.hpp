#ifndef TILE_HPP
# define TILE_HPP

# include <iostream>
# include <SFML/Graphics.hpp>
# include <Board.hpp>
# include <map>

class Board;

class Tile
{
	static sf::Color p1_color;
	static sf::Color p2_color;

    private:
		Board				*_board;
		size_t				_pos;
		sf::Vector2f		_dimensions;
		sf::Vector2f		_position;
		sf::RectangleShape	_square;
		sf::CircleShape		_piece;
		bool				_enabled;
		bool				_set;
		bool				_hover;
		bool				_hint;

    public:
		Tile();
		Tile(Board *board, size_t pos = 0);
		Tile(Tile const &other);
		~Tile();
		Tile &operator=(Tile const &other);
		void resize();
		void draw();
		bool hover();
		bool click();
		bool enabled();
		void enable();
		void disable();
};

#endif

#ifndef BOARD_HPP
# define BOARD_HPP

# include <SFML/Graphics.hpp>
# include <Tile.hpp>
# include <Game.hpp>
# include <Gui.hpp>

# define TILE_SIZE 500
# define BACKGROUND_SIZE 1024
# define BUTTON_SIZE 200

class Tile;

class Board
{
	private:

		size_t				_sqrt;
		size_t				_size;
		sf::RectangleShape	_shape;
		sf::RenderWindow*	_window;
		sf::Vector2f		_dimensions;
		sf::Vector2f		_position;
		std::vector<Tile>	_tiles;
		sf::Vector2f		_tiles_dimensions;
		Tile				*_hovered_tile;
		bool				_enabled;
		Game				_game;

	public:
		Board(size_t sqrt, sf::RenderWindow* window);
		Board();
		Board(Board const &other);
		~Board();
		Board &operator=(Board const &other);
		void resize();
		void draw();
		size_t sqrt();
		sf::RenderWindow &window() const;
		sf::Vector2f const &get_dimensions() const;
		sf::Vector2f const &get_position() const;
		void init();
		bool hover();
		bool click();
		bool enabled();
		void enable();
		void disable();
		sf::Vector2f get_tile_position(int i);
		sf::Vector2f& get_tile_dimensions();
		size_t get_hovered_tile(sf::Vector2f &mouse);
};

#endif

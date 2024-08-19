#ifndef BOARD_HPP
# define BOARD_HPP

//# include <SFML/Graphics.hpp>
//# include <Tile.hpp>
//# include <Game.hpp>
//# include <Gui.hpp>
#include <MLX42/MLX42.h>
#include <vector>
#include <Rect.hpp>
#include <Tile.hpp>


# define TILE_SIZE 500
# define BACKGROUND_SIZE 1024
# define BUTTON_SIZE 200

typedef enum e_piecetype
{
	PT_BLACK,
	PT_WHITE,
	PT_HINT,
	PT_BLACKHOVER,
	PT_WHITEHOVER
} e_piecetype;

class Tile;

class Board
{
	private:

		mlx_image_t			*_background;
		static Rect			_dimensions;
		std::vector<Tile>	_tiles;


		bool _init();
		void _reset();
	public:
		static size_t				_sqrt;
		static size_t				_size;
		static mlx_image_t			*_tile_images[9];
		static mlx_image_t			*_piece_images[5];
		//sf::RenderWindow*	_window;
		//sf::Vector2f		_dimensions;
		//sf::Vector2f		_position;
		//sf::Vector2f		_tiles_dimensions;
		//Tile				*_hovered_tile;
		//bool				_enabled;
		//Game				_game;

	public:
		Board();
		Board(Board const &other);
		~Board();
		bool reset(size_t sqrt);
		bool init();
		Board &operator=(Board const &other) ;
		void resize();
		static size_t size();
		static size_t sqrt();
		static Rect const &dimensions();
		//void draw();
		//sf::RenderWindow &window() const;
		//sf::Vector2f const &get_dimensions() const;
		//sf::Vector2f const &get_position() const;
		//bool hover();
		//bool click();
		//bool enabled();
		//void enable();
		//void disable();
		//sf::Vector2f get_tile_position(int i);
		//sf::Vector2f& get_tile_dimensions();
		//size_t get_hovered_tile(sf::Vector2f &mouse);
};

#endif

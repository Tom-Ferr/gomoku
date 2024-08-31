#ifndef BOARD_HPP
# define BOARD_HPP

//# include <SFML/Graphics.hpp>
//# include <Tile.hpp>
# include <Game.hpp>
//# include <Gui.hpp>
#include <MLX42/MLX42.h>
#include <common.hpp>
#include <vector>
#include <Rect.hpp>
#include <Tile.hpp>
#include <Info.hpp>
#include <BoardStatusBar.hpp>
# include <BoardEndGame.hpp>
# include <BoardMode.hpp>

# define TILE_SIZE 500
# define BACKGROUND_SIZE 1024
# define BUTTON_SIZE 200
# define DEFAULT_BOARD 19

class Tile;
class Info;

class Board
{
	private:
		mlx_image_t			*_background;
		static Rect			_dimensions;
		std::vector<Tile>	_tiles;
		Tile				*_hovered_tile;
		Rect				_tile_dimensions;
		bool				_enabled;
		bool				_visible;

		bool _init();
		void _reset();

	public:
		static size_t		_sqrt;
		static size_t		_size;
		static mlx_image_t	*_tile_images[9];
		static mlx_image_t	*_piece_images[5];
		Game				_game;
		BoardStatusBar		_statusbar;
		BoardEndGame		_endgame;
		BoardMode			_mode;

	public:
		Board();
		Board(Board const &other);
		~Board();
		bool init();
		Board &operator=(Board const &other) ;
		void resize();
		static size_t size();
		static size_t sqrt();
		static Rect const &dimensions();
		static mlx_image_t *piece_image(t_piecetype piecetype);
		bool hover();
		bool click();
		bool enabled();
		void enable();
		void disable();
		bool visible();
		bool show();
		void hide();
		Rect get_tile_position(int pos);
		void _remove_captures();
		Rect& get_tile_dimensions();
		int get_hovered_tile();
};

#endif

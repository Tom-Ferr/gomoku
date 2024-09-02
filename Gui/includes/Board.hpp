#ifndef BOARD_HPP
# define BOARD_HPP

//# include <SFML/Graphics.hpp>
//# include <Tile.hpp>
# include <Game.hpp>
# include <MLX42/MLX42.h>
# include <common.hpp>
# include <vector>
# include <Rect.hpp>
# include <Tile.hpp>
# include <Info.hpp>
# include <BoardStatusBar.hpp>
# include <BoardEndGame.hpp>
# include <BoardMode.hpp>

# define TILE_SIZE 500
# define BACKGROUND_SIZE 1024
# define BUTTON_SIZE 200
# define DEFAULT_BOARD 19

class Tile;
class Info;
class Gui;

class Board
{
	private:
		static Rect			_dimensions;
		mlx_image_t			*_background;
		Gui					*_gui;
		std::vector<Tile>	_tiles;
		Tile				*_hovered_tile;
		Rect				_tile_dimensions;
		bool				_enabled;
		bool				_visible;
		Game				_game;
		BoardStatusBar		_statusbar;
		BoardEndGame		_endgame;
		BoardMode			_mode;
		Tile				*_hinted_tile;
		size_t				_loop_count;

		bool				_init();
		void				_remove_captures();

	public:
		static size_t		_sqrt;
		static size_t		_size;
		static mlx_image_t	*_tile_images[9];
		static mlx_image_t	*_piece_images[5];

	public:
		Board(Gui *gui=nullptr);
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
		bool show(t_vs vs, t_startingplayer starting, t_gamemode mode);
		void hide();
		void loop();
		Rect& get_tile_dimensions();
		int get_hovered_tile();
		void clear_hints();
		void update_statusbar();
};

#endif

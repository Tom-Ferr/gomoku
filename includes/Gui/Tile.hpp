#ifndef TILE_HPP
# define TILE_HPP

# include <MLX42/MLX42.h>
# include <iostream>
# include <Board.hpp>
# include <map>

class Board;

class Tile
{
    private:
		size_t				_pos;
		bool				_enabled;
		int32_t				_tile_tex;
		int32_t				_tile_idx;
		int32_t				_pieces[5];
		bool				_hover;

    public:
		Tile();
		Tile(size_t pos = 0);
		Tile(Tile const &other);
		~Tile();
		Tile &operator=(Tile const &other);
		void resize(Rect dimensions);
		bool init();
		bool hover(bool on, bool turn);
		void hint(bool on);
		bool click(bool turn);
		bool enabled();
		void enable();
		void disable();
		void clear();
		void show();
		void hide();
		size_t &pos();
		mlx_instance_t &piece(int type);
};

#endif

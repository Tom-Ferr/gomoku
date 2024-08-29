#ifndef BOARDSTATUSBAR_HPP
# define BOARDSTATUSBAR_HPP

# include <string>
# include <Rect.hpp>
# include <Color.hpp>
# include <Text.hpp>
# include <Info.hpp>
# include <MLX42/MLX42.h>
# include <common.hpp>

class BoardStatusBar
{
	private:
		mlx_image_t						*_background;
		Rect							_dimensions;
		Info							_vs;
		Info							_player1;
		Info							_player2;
		Info							_turn;
		Info							_last_time;
		Info							_avg_time;

		void _resize_vertical(size_t text_height);
		void _resize_horizontal(size_t text_height);

	public:
		BoardStatusBar();
		BoardStatusBar(BoardStatusBar const &other);
		~BoardStatusBar();
		void init();
		void resize();
		BoardStatusBar &operator=(BoardStatusBar const &other);
		Rect const &dimensions();
		void hide();
		void show();
};

#endif

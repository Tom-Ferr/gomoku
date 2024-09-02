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
		Text							_captures;
		Info							_player1_captures;
		Info							_player2_captures;

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

		/*
		** setters
		*/
		void set_vs(bool ai);
		void set_player1(bool black);
		void set_player2(bool black, bool ai);
		void set_turn(bool p1_turn);
		void set_last_time(std::string const &last_time);
		void set_avg_time(std::string const &avg_time);
		void set_player1_captures(size_t captures);
		void set_player2_captures(size_t captures);

};

#endif

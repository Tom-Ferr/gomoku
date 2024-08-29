#ifndef BOARDENDGAME_HPP
# define BOARDENDGAME_HPP

# include <string>
# include <Rect.hpp>
# include <Color.hpp>
# include <Text.hpp>
# include <Info.hpp>
# include <MLX42/MLX42.h>
# include <common.hpp>

class BoardEndGame
{
	private:
		mlx_image_t						*_background;
		Rect							_dimensions;
		Text							_player;
		Text							_wins;
		Text							_esc;

	public:
		BoardEndGame();
		BoardEndGame(BoardEndGame const &other);
		~BoardEndGame();
		void init();
		void resize();
		BoardEndGame &operator=(BoardEndGame const &other);
		Rect const &dimensions();
		void hide();
		void show(std::string winner);
};

#endif

#ifndef BOARDENDGAME_HPP
# define BOARDENDGAME_HPP

# include <string>
# include <Rect.hpp>
# include <Color.hpp>
# include <Text.hpp>
# include <Info.hpp>
# include <ABoardPopup.hpp>
# include <MLX42/MLX42.h>
# include <common.hpp>

/*
** Popup displayed when the game ends.
** Derived from ABoardPopup.
*/
class BoardEndGame : public ABoardPopup
{
	private:
		Text							_player;
		Text							_wins;

	public:
		BoardEndGame();
		BoardEndGame(BoardEndGame const &other);
		~BoardEndGame();
		BoardEndGame &operator=(BoardEndGame const &other);
		void init();
		void resize();
		void hide();
		bool click();
		void hover();
		void show(std::string winner);
};

#endif

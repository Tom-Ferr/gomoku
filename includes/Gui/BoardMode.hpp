#ifndef BOARDMODE_HPP
# define BOARDMODE_HPP

# include <string>
# include <Rect.hpp>
# include <Color.hpp>
# include <Text.hpp>
# include <Info.hpp>
# include <ABoardPopup.hpp>
# include <ButtonGroup.hpp>

class GameMessage;

/*
** Popup displayed when the game ends.
** Derived from ABoardPopup.
** Utilizes GameMessage class as an interface of the message to
** be displayed.
*/
class BoardMode: public ABoardPopup
{
	private:
		Text							_description;
		Text							_info;
		ButtonGroup						_buttons;

		void _resize_info();
		void _resize_buttons();

	public:
		BoardMode();
		BoardMode(BoardMode const &other);
		~BoardMode();
		BoardMode &operator=(BoardMode const &other);
		bool init();
		void resize();
		void hide();
		bool click();
		void show(GameMessage const &message);
		void hover();
		ButtonGroup const &buttons() const;
};

#endif

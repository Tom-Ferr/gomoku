#ifndef BUTTONGROUP_HPP
# define BUTTONGROUP_HPP

#include <Button.hpp>
#include <vector>

class ButtonGroup
{
	private:
		std::vector<Button>	_buttons;
		Rect				_dimensions;
		Text				_title;

	public:
		ButtonGroup();
		ButtonGroup(std::string const &title, Rect const &dimensions);
		ButtonGroup(ButtonGroup const &other);
		~ButtonGroup();
		ButtonGroup &operator=(ButtonGroup const &other);
		void resize(Rect const &dimensions);
		void add(std::string const &str);
		Rect const &dimensions();
		bool hover();
		bool click();
		void show();
		void hide();
		void clear();
};

#endif

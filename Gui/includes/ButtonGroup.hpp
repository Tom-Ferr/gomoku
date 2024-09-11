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
		void depth(size_t depth);
		bool hover();
		bool click();
		void show();
		void hide();
		void clear();
		size_t selected() const;
		size_t size() const;
};

#endif

#include <BoardMode.hpp>
#include <Gui.hpp>
# include <Board.hpp>


BoardMode::BoardMode()
{ }

BoardMode::BoardMode(BoardMode const &other)
{
	*this = other;
}

BoardMode::~BoardMode() {}

BoardMode &BoardMode::operator=(BoardMode const &other)
{
	(void)other;
	return (*this);
}

void BoardMode::init()
{
	_init();
	_buttons = ButtonGroup("Player 1 must choose", Rect(0, 0, 0, 0));
	hide();
}

void BoardMode::resize()
{
	_resize();
	_resize_info();
	_resize_buttons();
}

void BoardMode::_resize_info()
{
	Rect dimensions = Rect::subrect(_dimensions, 1, .07, 1);
	_info.resize(dimensions);
	_info.center(_dimensions.x + (_dimensions.width / 2));
}


void BoardMode::_resize_buttons()
{
	if (!_centerbox)
		return ;
	_buttons.resize(Rect::subrect(_dimensions, .5));
	_buttons.depth(_centerbox->instances[0].z + 1);
}

void BoardMode::hide()
{
	_hide();
	_buttons.hide();
	_info.hide();
}

void BoardMode::show(std::string mode, bool selecting)
{
	_set_header_text(mode);
	_show();
	_buttons.clear();
	if (selecting)
	{
		_buttons.add("Play as White");
		_buttons.add("Play as Black");
		_buttons.show();
	}
	else
	{
		_info = "Player is playing as Black";
		_info.show();
	}
	resize();
}

void BoardMode::show(GameMessage const &message)
{
	_set_header_text(message.mode());
	_show();
	_buttons.clear();
	if (message.selection())
	{
		_buttons.add("Play as White");
		_buttons.add("Play as Black");
		_buttons.show();
	}
	else
	{
		_info = message.text();
		_info.show();
	}
	resize();
}

void BoardMode::hover()
{
	_hover();
	if (_buttons.dimensions().contains(Gui::mouse().x, Gui::mouse().y))
		_buttons.hover();
}

/*
** Returns true if OK button has been clicked.
*/
bool BoardMode::click()
{
	if (_click())
		return true;
	if (_buttons.dimensions().contains(Gui::mouse().x, Gui::mouse().y))
		_buttons.click();
	return false;
}

ButtonGroup const &BoardMode::buttons() const
{
    return _buttons;
}

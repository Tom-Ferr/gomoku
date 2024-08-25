#include <ButtonGroup.hpp>
#include <Gui.hpp>

ButtonGroup::ButtonGroup()
{ }

ButtonGroup::ButtonGroup(std::string const &title, Rect const &dimensions)
{
	_title = Text(title, true);
	_dimensions = dimensions;
}

ButtonGroup::ButtonGroup(ButtonGroup const &other)
{
	*this = other;
}

ButtonGroup::~ButtonGroup()
{ }

ButtonGroup &ButtonGroup::operator=(ButtonGroup const &other)
{
	_buttons = other._buttons;
	_dimensions = other._dimensions;
	_title = other._title;
	return *this;
}

/*
** since all buttons will have the same height,
** dimensions is expected to contain x, y as usual
** but height should be the buttons height, not the button group.
** button group will adjust its own dimensions accordingly.
*/
void ButtonGroup::resize(Rect const &dimensions)
{
	_title.resize(dimensions);
	_dimensions = Rect(0, 0, 0, 0);
	for (size_t i = 0; i < _buttons.size(); i++)
	{
		_buttons[i].resize(Rect(dimensions.x, dimensions.y + dimensions.height * (i + 1) * 1.2, dimensions.width, dimensions.height));
		_dimensions.width = std::max(_dimensions.width, _buttons[i].dimensions().width);
		_dimensions.height =  std::max(_dimensions.height, _buttons[i].dimensions().y);
	}
	_dimensions.x = dimensions.x;
	_dimensions.y = dimensions.y;
	_dimensions.height += dimensions.height;
	_dimensions.height -= _dimensions.y;
}

void ButtonGroup::add(std::string const &str)
{
	_buttons.push_back(Button(str));
	if (_buttons.size() == 1)
		_buttons[0].select(true);
}

Rect const &ButtonGroup::dimensions()
{
	return _dimensions;
}

bool ButtonGroup::hover()
{
	for (size_t i = 0; i < _buttons.size(); i++)
		_buttons[i].hover();
	return true;
}

bool ButtonGroup::click()
{
	for (size_t i = 0; i < _buttons.size(); i++)
	{
		if (_buttons[i].dimensions().contains(Gui::mouse().x, Gui::mouse().y))
			_buttons[i].select();
		else
			_buttons[i].select(false);
	}
	return false;
}

void ButtonGroup::show()
{
	_title.show();
	for (size_t i = 0; i < _buttons.size(); i++)
		_buttons[i].show();
}

void ButtonGroup::hide()
{
	_title.hide();
	for (size_t i = 0; i < _buttons.size(); i++)
		_buttons[i].hide();
}

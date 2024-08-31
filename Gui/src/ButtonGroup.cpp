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
	size_t prev_y;
	float ratio;
	//Rect(756, 362, 194, 324);
	ratio = static_cast<float>(Gui::texture("selectbuttons")->height) / Gui::texture("selectbuttons")->width;
	_title.resize(Rect(dimensions.x, dimensions.y, dimensions.width, dimensions.width * ratio * .8));
	_title.center(dimensions.x + (dimensions.width / 2));
	prev_y = dimensions.y + (_title.dimensions().height * 1.2);
	for (size_t i = 0; i < _buttons.size(); i++)
	{
		_buttons[i].resize(Rect(dimensions.x, prev_y, dimensions.width, dimensions.height));
		prev_y += (_buttons[i].dimensions().height * 1.2);
	}
	_dimensions = dimensions;
	_dimensions.height = _title.dimensions().height;
	if (_buttons.size())
		_dimensions.height = (_buttons[_buttons.size() - 1].dimensions().y + _buttons[_buttons.size() - 1].dimensions().height) - _dimensions.y;
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
		{
			for (size_t j = 0; j < _buttons.size(); j++)
				_buttons[j].select(false);
			_buttons[i].select();
			return true;
		}
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

void ButtonGroup::clear()
{
	for (size_t i = 0; i < _buttons.size(); i++)
		_buttons[i].hide();
	_buttons.clear();
}

void ButtonGroup::depth(size_t depth)
{
	_title.depth(depth + 1);
	for (size_t i = 0; i < _buttons.size(); i++)
		_buttons[i].depth(depth + 1);
}

size_t ButtonGroup::selected()
{
	for (size_t i = 0; i < _buttons.size(); i++)
	{
		if (_buttons[i].selected())
			return i;
	}
	return 0;
}

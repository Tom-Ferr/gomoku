#include <Button.hpp>
#include <Gui.hpp>

mlx_image_t			*Button::_button = nullptr;
mlx_image_t			*Button::_button_hover = nullptr;
mlx_image_t			*Button::_button_selected = nullptr;

Button::Button()
{}

Button::Button(std::string const &str)
: _selected(false), _hovered(false), _initialized(false)
{
	_text = str;
	if (!Button::_button || !Button::_button_hover || !Button::_button_selected)
		_init_static();
	_init();
}

Button::Button(Button const &other)
{
	*this = other;
}

Button::~Button() {}

Button &Button::operator=(Button const &other)
{
	_button = other._button;
	_button_hover = other._button_hover;
	_button_selected = other._button_selected;
	_dimensions = other._dimensions;
	_text = other._text;
	_selected = other._selected;
	_hovered = other._hovered;
	_initialized = other._initialized;
	_buttons[BTN_NORMAL] = other._buttons[BTN_NORMAL];
	_buttons[BTN_HOVER] = other._buttons[BTN_HOVER];
	_buttons[BTN_SELECTED] = other._buttons[BTN_SELECTED];
	return (*this);
}

Rect const &Button::dimensions()
{
	return (_dimensions);
}



void Button::_init_static()
{
	Rect dimensions;

	dimensions = Rect(0, 0, Gui::texture("selectbuttons")->width / 3,
						Gui::texture("selectbuttons")->height / 3);
	if (!_button)
		_button =  mlx_new_image(Gui::mlx(),
								dimensions.width, dimensions.height);
	if (!_button_hover)
		_button_hover = mlx_new_image(Gui::mlx(),
								dimensions.width, dimensions.height);
	if (!_button_selected)
		_button_selected = mlx_new_image(Gui::mlx(),
								dimensions.width, dimensions.height);
}

void Button::_init()
{
	if (!_button || !_button_hover || !_button_selected)
		return ;
	_buttons[BTN_NORMAL]
			= mlx_image_to_window(Gui::mlx(), _button, 0, 0);
	_buttons[BTN_HOVER]
			= mlx_image_to_window(Gui::mlx(), _button_hover, 0, 0);
	_buttons[BTN_SELECTED]
			= mlx_image_to_window(Gui::mlx(), _button_selected, 0, 0);
	if (_buttons[BTN_NORMAL] == -1 || _buttons[BTN_HOVER] == -1
			|| _buttons[BTN_SELECTED] == -1)
		return ;
	mlx_set_instance_depth(
			&_button->instances[_buttons[BTN_NORMAL]], 20);
	mlx_set_instance_depth(
			&_button_hover->instances[_buttons[BTN_HOVER]], 20);
	mlx_set_instance_depth(
			&_button_selected->instances[_buttons[BTN_SELECTED]], 20);
	_button->instances[_buttons[BTN_NORMAL]].enabled = true;
	_button_hover->instances[_buttons[BTN_HOVER]].enabled = false;
	_button_selected->instances[_buttons[BTN_SELECTED]].enabled = false;
	_initialized = true;
	resize(Rect(300, 350, 0, 30));
}

/*
** Button height is defined by its width.
*/
void Button::resize(Rect dimensions)
{
	Rect text;
	float ratio;
	size_t center;

	center = dimensions.x + (dimensions.width / 2);
	ratio = static_cast<float>(Gui::texture("selectbuttons")->height)
				/ Gui::texture("selectbuttons")->width;
	dimensions.height = dimensions.width * ratio;
	if (initialized())
	{
		mlx_resize_image(_button, dimensions.width, dimensions.height);
		mlx_resize_image(_button_hover, dimensions.width, dimensions.height);
		mlx_resize_image(_button_selected, dimensions.width, dimensions.height);
		Gui::apply_texture(_button,
							Gui::texture("selectbuttons"), Color::white, 2, 3);
		Gui::apply_texture(_button_hover,
							Gui::texture("selectbuttons"), Color::white, 1, 3);
		Gui::apply_texture(_button_selected,
							Gui::texture("selectbuttons"), Color::white, 0, 3);
		_button->instances[_buttons[BTN_NORMAL]].x = center - dimensions.width / 2;
		_button->instances[_buttons[BTN_NORMAL]].y = dimensions.y;
		_button_hover->instances[_buttons[BTN_HOVER]].x
										= center - dimensions.width / 2;
		_button_hover->instances[_buttons[BTN_HOVER]].y = dimensions.y;
		_button_selected->instances[_buttons[BTN_SELECTED]].x
										= center - dimensions.width / 2;
		_button_selected->instances[_buttons[BTN_SELECTED]].y
										= dimensions.y;
	}
	text = Rect(dimensions.x,
				dimensions.y + (dimensions.height * .2),
				0, dimensions.height * .8);
	_text.resize(text);
	_text.center(center);
	text.width =_text.dimensions().width;
	_dimensions = dimensions;
}

void Button::depth(size_t depth)
{
	if (!initialized())
		return ;
	mlx_set_instance_depth(
				&_button->instances[_buttons[BTN_NORMAL]], depth);
	mlx_set_instance_depth(
				&_button_hover->instances[_buttons[BTN_HOVER]], depth);
	mlx_set_instance_depth(
				&_button_selected->instances[_buttons[BTN_SELECTED]], depth);
	_text.depth(depth + 1);
}

bool Button::selected() const
{
	return _selected;
}

void Button::select(bool selected)
{
	_hovered = false;
	if (!initialized())
		return ;
	if (selected)
	{
		_selected = true;
		_button->instances[_buttons[BTN_NORMAL]].enabled = false;
		_button_hover->instances[_buttons[BTN_HOVER]].enabled = false;
		_button_selected->instances[_buttons[BTN_SELECTED]].enabled = true;
	}
	else
	{
		_selected = false;
		_button->instances[_buttons[BTN_NORMAL]].enabled = true;
		_button_hover->instances[_buttons[BTN_HOVER]].enabled = false;
		_button_selected->instances[_buttons[BTN_SELECTED]].enabled = false;
	}
}
void Button::hover()
{
	bool on = false;

	if (!initialized())
		return ;
	if (_selected)
		return ;
	on = _dimensions.contains(Gui::mouse().x, Gui::mouse().y);
	if (on && !_hovered)
	{
		_button->instances[_buttons[BTN_NORMAL]].enabled = false;
		_button_hover->instances[_buttons[BTN_HOVER]].enabled = true;
		_hovered = true;
	}
	else if (!on && _hovered)
	{
		_button->instances[_buttons[BTN_NORMAL]].enabled = true;
		_button_hover->instances[_buttons[BTN_HOVER]].enabled = false;
		_hovered = false;
	}
}

void Button::hide()
{
	_text.hide();
	if (!initialized())
		return ;
	_button->instances[_buttons[BTN_NORMAL]].enabled = false;
	_button_hover->instances[_buttons[BTN_HOVER]].enabled = false;
	_button_selected->instances[_buttons[BTN_SELECTED]].enabled = false;
}

void Button::show()
{
	_text.show();
	if (!initialized())
		return ;
	if (_selected)
		_button_selected->instances[_buttons[BTN_SELECTED]].enabled = true;
	else
		_button->instances[_buttons[BTN_NORMAL]].enabled = true;
}

bool Button::initialized() const
{
	return _button && _button_hover && _button_selected && _initialized;
}
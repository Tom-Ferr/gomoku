#include <Button.hpp>
#include <Gui.hpp>

mlx_image_t			*Button::_button = nullptr;
mlx_image_t			*Button::_button_hover = nullptr;
mlx_image_t			*Button::_button_selected = nullptr;

Button::Button()
{}

Button::Button(std::string const &str)
: _selected(false), _hovered(false)
{
	_text = str;
	if (!Button::_button)
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
	_button =  mlx_new_image(Gui::mlx(), Gui::_piece_texture->width, Gui::_piece_texture->height);
	_button_hover = mlx_new_image(Gui::mlx(), Gui::_piece_texture->width, Gui::_piece_texture->height);
	_button_selected = mlx_new_image(Gui::mlx(), Gui::_piece_texture->width, Gui::_piece_texture->height);
}

void Button::_init()
{
	_buttons[BTN_NORMAL] = mlx_image_to_window(Gui::mlx(), _button, 0, 0);
	_buttons[BTN_HOVER] = mlx_image_to_window(Gui::mlx(), _button_hover, 0, 0);
	_buttons[BTN_SELECTED] = mlx_image_to_window(Gui::mlx(), _button_selected, 0, 0);
	_button->instances[_buttons[BTN_NORMAL]].enabled = true;
	_button_hover->instances[_buttons[BTN_HOVER]].enabled = false;
	_button_selected->instances[_buttons[BTN_SELECTED]].enabled = false;
	resize(Rect(300, 350, 0, 30));
}

void Button::resize(Rect const &dimensions)
{
	Rect text;

	mlx_resize_image(_button, dimensions.height, dimensions.height);
	mlx_resize_image(_button_hover, dimensions.height, dimensions.height);
	mlx_resize_image(_button_selected, dimensions.height, dimensions.height);
	Gui::apply_texture(_button, Gui::_piece_texture, Color(255, 255, 255, 255));
	Gui::apply_texture(_button_hover, Gui::_piece_texture, Color(100, 100, 255, 200));
	Gui::apply_texture(_button_selected, Gui::_piece_texture, Color(255, 210, 0, 255));
	_button->instances[_buttons[BTN_NORMAL]].x = dimensions.x;
	_button->instances[_buttons[BTN_NORMAL]].y = dimensions.y;
	_button_hover->instances[_buttons[BTN_HOVER]].x = dimensions.x;
	_button_hover->instances[_buttons[BTN_HOVER]].y = dimensions.y;
	_button_selected->instances[_buttons[BTN_SELECTED]].x = dimensions.x;
	_button_selected->instances[_buttons[BTN_SELECTED]].y = dimensions.y;
	std::cout << "Button Instance" << _buttons[BTN_SELECTED] << std::endl;
	text = Rect(dimensions.x + (dimensions.height * 1.5), dimensions.y + 4, 0, dimensions.height);
	std::cout << "Text rect" << text << std::endl;
	_text.resize(text);
	text.width =_text.dimensions().width;
	_dimensions = Rect(dimensions.x, dimensions.y, text.width + (dimensions.height) * 1.5, dimensions.height);
}

void Button::select(bool selected)
{
	_hovered = false;
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
	_button->instances[_buttons[BTN_NORMAL]].enabled = false;
	_button_hover->instances[_buttons[BTN_HOVER]].enabled = false;
	_button_selected->instances[_buttons[BTN_SELECTED]].enabled = false;
}

void Button::show()
{
	std::cout << "Button" << _text.text() << " " <<  _selected << std::endl;
	_text.show();
	if (_selected)
		_button_selected->instances[_buttons[BTN_SELECTED]].enabled = true;
	else
		_button->instances[_buttons[BTN_NORMAL]].enabled = true;
}
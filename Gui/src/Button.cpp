#include <Button.hpp>
#include <Gui.hpp>

mlx_image_t			*Button::_button = nullptr;
mlx_image_t			*Button::_button_hover = nullptr;
mlx_image_t			*Button::_button_selected = nullptr;

Button::Button()
{}

Button::Button(std::string const &str)
{
	_text = str;
	if (!Button::_button)
		_init_static();
	_init();
}

Button::Button(Button const &other)
:_dimensions(other._dimensions), _text(other._text)
{
	*this = other;
}

Button::~Button() {}

Button &Button::operator=(Button const &other)
{
	_button_hover = other._button_hover;
	_button_selected = other._button_selected;
	_dimensions = other._dimensions;
	_text = other._text;
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
	resize(Rect(300, 350, 0, 30));
}

void Button::resize(Rect const &dimensions)
{
	Rect text;

	mlx_resize_image(_button, dimensions.height, dimensions.height);
	mlx_resize_image(_button_hover, dimensions.height, dimensions.height);
	mlx_resize_image(_button_selected, dimensions.height, dimensions.height);
	Gui::apply_texture(_button, Gui::_piece_texture, Color(255, 255, 255, 100));
	Gui::apply_texture(_button_hover, Gui::_piece_texture, Color(100, 100, 255, 200));
	Gui::apply_texture(_button_selected, Gui::_piece_texture, Color(255, 210, 0, 255));
	_button->instances[_buttons[BTN_NORMAL]].x = dimensions.x;
	_button->instances[_buttons[BTN_NORMAL]].y = dimensions.y;
	_button_hover->instances[_buttons[BTN_HOVER]].x = dimensions.x;
	_button_hover->instances[_buttons[BTN_HOVER]].y = dimensions.y;
	_button_selected->instances[_buttons[BTN_SELECTED]].x = dimensions.x;
	_button_selected->instances[_buttons[BTN_SELECTED]].y = dimensions.y;
	text = Rect(dimensions.x + (dimensions.height * 1.5), dimensions.y + 4, 0, dimensions.height);
	_text.resize(text);
	text.width =_text.dimensions().width;
	_dimensions = Rect(dimensions.x, dimensions.y, text.width + dimensions.height * 1.1, dimensions.height);
}

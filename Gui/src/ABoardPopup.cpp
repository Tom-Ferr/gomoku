#include <ABoardPopup.hpp>
#include <Gui.hpp>
# include <Board.hpp>


ABoardPopup::ABoardPopup()
: _background(nullptr), _centerbox(nullptr), _header(nullptr), _ok_button(nullptr), _ok_button_hover(nullptr)
{ }

ABoardPopup::ABoardPopup(ABoardPopup const &other)
{
	*this = other;
}

ABoardPopup::~ABoardPopup() {}

ABoardPopup &ABoardPopup::operator=(ABoardPopup const &other)
{
	_dimensions = other._dimensions;
	_background = other._background;
	_centerbox = other._centerbox;
	_header = other._header;
	_ok_button = other._ok_button;
	_ok_button_hover = other._ok_button_hover;
	_header_text = other._header_text;
	_enabled = other._enabled;
	return (*this);
}

Rect const &ABoardPopup::dimensions()
{
	return (_dimensions);
}

void ABoardPopup::_init()
{
	_background = mlx_new_image(Gui::mlx(), 1, 1);
	_centerbox = mlx_new_image(Gui::mlx(), 300, 300);
	_header = mlx_new_image(Gui::mlx(), 300, 300);
	_ok_button = mlx_new_image(Gui::mlx(), 300, 300);
	_ok_button_hover = mlx_new_image(Gui::mlx(), 300, 300);
	mlx_put_pixel(_background, 0, 0, 0x00000fbb);
	mlx_image_to_window(Gui::mlx(), _background, _dimensions.x, _dimensions.y);
	mlx_image_to_window(Gui::mlx(), _centerbox, _dimensions.x, _dimensions.y);
	mlx_image_to_window(Gui::mlx(), _header, _dimensions.x, _dimensions.y);
	mlx_image_to_window(Gui::mlx(), _ok_button_hover, _dimensions.x, _dimensions.y);
	mlx_image_to_window(Gui::mlx(), _ok_button, _dimensions.x, _dimensions.y);
	_header_text = Text("Header", true);
	hide();
}

void ABoardPopup::_resize()
{
	_resize_background();
	_resize_box();
	_resize_ok();
	_resize_header();
}

void ABoardPopup::_resize_ok()
{
	if (!_ok_button)
		return ;
	_ok_dimensions = Rect::subrect(_dimensions, .3, .1, 2);
	mlx_resize_image(_ok_button, _ok_dimensions.width, _ok_dimensions.height);
	mlx_resize_image(_ok_button_hover, _ok_dimensions.width, _ok_dimensions.height);
	Gui::apply_texture(_ok_button, Gui::texture("playbuttons"), Color::white, 3, 2);
	Gui::apply_texture(_ok_button_hover, Gui::texture("playbuttons"), Color::white, 1, 2);
	_ok_dimensions.y = _ok_dimensions.y - _ok_dimensions.height * .3;
	_ok_button->instances[0].x = _ok_dimensions.x;
	_ok_button->instances[0].y = _ok_dimensions.y;
	_ok_button_hover->instances[0].x = _ok_dimensions.x;
	_ok_button_hover->instances[0].y = _ok_dimensions.y;
}
void ABoardPopup::_resize_box()
{
	if (!_centerbox)
		return ;
	_dimensions = Rect::subrect(Gui::dimensions(), .6);
	mlx_resize_image(_centerbox, _dimensions.width, _dimensions.height);
	_centerbox->instances[0].x = _dimensions.x;
	_centerbox->instances[0].y = _dimensions.y;
	_box_texture();
}

void ABoardPopup::_resize_background()
{
	if (!_background)
		return ;
	mlx_resize_image(_background,  Gui::dimensions().width,  Gui::dimensions().height);
	_background->instances[0].x = 0;
	_background->instances[0].y = 0;
}

void ABoardPopup::_resize_header()
{
	float ratio;
	Rect header_dimensions;

	if (!_header)
		return ;
	header_dimensions = Rect::subrect(_dimensions, 1.04);;
	ratio = static_cast<float>(Gui::texture("selectbuttons")->height) / Gui::texture("selectbuttons")->width;
	header_dimensions.height = header_dimensions.width * ratio;
	mlx_resize_image(_header, header_dimensions.width, header_dimensions.height);
	_header->instances[0].x = header_dimensions.x;
	_header->instances[0].y = header_dimensions.y;
	Gui::apply_texture(_header, Gui::texture("selectbuttons"), Color(255, 255, 255, 255), 4, 3);
	_header_text.resize(Rect::subrect(header_dimensions, .8));
	_header_text.center(header_dimensions.x + (header_dimensions.width / 2));
}

void ABoardPopup::_box_texture()
{
	std::map<std::string, Rect> subrects;

	subrects["top"] = Rect(0, 0, _centerbox->width, 10);
	subrects["left"] = Rect(0, 0, 10, _centerbox->height);
	subrects["right"] = Rect(_centerbox->width - 10, 0, 10, _centerbox->height);
	subrects["bottom"] = Rect(0, _centerbox->height - 10, _centerbox->width, 10);
	subrects["topleft"] = Rect(0, 0, 10, 10);
	subrects["topright"] = Rect(_centerbox->width - 10, 0, 10, 10);
	subrects["bottomright"] = Rect(_centerbox->width - 10, _centerbox->height - 10, 10, 10);
	subrects["bottomleft"] = Rect(0, _centerbox->height - 10, 10, 10);
	Gui::apply_texture(_centerbox, Gui::texture("box"), Color::white, 4, 3);
	Gui::apply_texture(_centerbox, Gui::texture("box"), Color::white, 1, 3, subrects["top"]);
	Gui::apply_texture(_centerbox, Gui::texture("box"), Color::white, 3, 3, subrects["left"]);
	Gui::apply_texture(_centerbox, Gui::texture("box"), Color::white, 5, 3, subrects["right"]);
	Gui::apply_texture(_centerbox, Gui::texture("box"), Color::white, 7, 3, subrects["bottom"]);
	Gui::apply_texture(_centerbox, Gui::texture("box"), Color::white, 0, 3, subrects["topleft"]);
	Gui::apply_texture(_centerbox, Gui::texture("box"), Color::white, 2, 3, subrects["topright"]);
	Gui::apply_texture(_centerbox, Gui::texture("box"), Color::white, 6, 3, subrects["bottomleft"]);
	Gui::apply_texture(_centerbox, Gui::texture("box"), Color::white, 8, 3, subrects["bottomright"]);
}

void ABoardPopup::_hide()
{
	_background->enabled = false;
	_centerbox->enabled = false;
	_header->enabled = false;
	_header_text.hide();
	_ok_button_hover->instances[0].enabled = false;
	_ok_button->instances[0].enabled = false;
	_enabled = false;
}

void ABoardPopup::_show()
{
	_background->enabled = true;
	_centerbox->enabled = true;
	_header->enabled = true;
	_header_text.show();
	_ok_button_hover->instances[0].enabled = false;
	_ok_button->instances[0].enabled = true;
	_resize();
	_enabled = true;
}

bool ABoardPopup::enabled()
{
	return _enabled;
}

void ABoardPopup::_hover()
{
	if (_ok_dimensions.contains(Gui::mouse().x, Gui::mouse().y))
	{
		_ok_button_hover->instances[0].enabled = true;
		_ok_button->instances[0].enabled = false;
	}
	else
	{
		_ok_button_hover->instances[0].enabled = false;
		_ok_button->instances[0].enabled = true;
	}
}

/*
** Returns true if OK button has been clicked.
*/
bool ABoardPopup::_click()
{
	if (_ok_dimensions.contains(Gui::mouse().x, Gui::mouse().y))
		return true;
	return false;
}

void ABoardPopup::_set_header_text(std::string const &text)
{
    	_header_text = text;
}
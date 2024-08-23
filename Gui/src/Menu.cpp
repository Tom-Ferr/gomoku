#include <Menu.hpp>
# include <Gui.hpp>

Menu::Menu()
: _logo(nullptr)
{

}

bool Menu::init()
{
	std::cout << "LOGO TEXTURE: " << Gui::_logo_texture << std::endl;
	_logo = mlx_new_image(Gui::mlx(), Gui::_logo_texture->height, Gui::_logo_texture->width);
	_play_button = mlx_new_image(Gui::mlx(), Gui::_playbutton_texture->height, Gui::_playbutton_texture->width);
	_play_button_hover = mlx_new_image(Gui::mlx(), Gui::_playbutton_texture->height, Gui::_playbutton_texture->width);
	_dimensions = Rect::subrect(Gui::dimensions(), .8);
	Gui::apply_texture(_logo, Gui::_logo_texture);
	Gui::apply_texture(_play_button, Gui::_playbutton_texture);
	Gui::apply_texture(_play_button_hover, Gui::_playbutton_texture, Color(100, 100, 255, 150));

	mlx_image_to_window(Gui::mlx(), _logo, 0, 0);
	mlx_image_to_window(Gui::mlx(), _play_button, 0, 0);
	mlx_image_to_window(Gui::mlx(), _play_button_hover, 0, 0);

	_button_vs_ai = Button("AI");
	_button_vs_player = Button("Player 2");
	resize();
	return (true);
}

Menu::Menu(Menu const &other)
{
	*this = other;
}

Menu::~Menu()
{ }

Menu &Menu::operator=(Menu const &other)
{
	(void)other;
	return *this;
}

void Menu::resize()
{
	_dimensions = Rect::subrect(Gui::dimensions(), .8);
	float ratio = float(Gui::_logo_texture->height) / Gui::_logo_texture->width;
	Rect _logo_dimensions = Rect::subrect(_dimensions, 1., ratio, 0);
	Rect _pbutton_dimensions = Rect::subrect(_dimensions, .4, .15, 2);
	Rect _button_dimensions = Rect::subrect(_dimensions, .4, .08, 1);
	std::cout << "Logo Dimensions: " << _logo_dimensions << std::endl;
	mlx_resize_image(_logo, _logo_dimensions.width, _logo_dimensions.height);
	mlx_resize_image(_play_button, _pbutton_dimensions.width, _pbutton_dimensions.height);
	mlx_resize_image(_play_button_hover, _pbutton_dimensions.width, _pbutton_dimensions.height);
	Gui::apply_texture(_logo, Gui::_logo_texture);
	_logo->instances[0].x = _logo_dimensions.x;
	_logo->instances[0].y = _logo_dimensions.y;
	_play_button->instances[0].x = _pbutton_dimensions.x;
	_play_button->instances[0].y = _pbutton_dimensions.y;
	_play_button_hover->instances[0].x = _pbutton_dimensions.x;
	_play_button_hover->instances[0].y = _pbutton_dimensions.y+100;
	_resize_buttons();
}

void Menu::_resize_buttons()
{
	std::cout << "Resize buttons" << std::endl;
	Rect button_box = Rect::subrect(Rect::subrect(Gui::dimensions(), .6), 1, .55, 2);
	button_box.height = button_box.height * .7;
	Rect left_button_box = Rect::subrect(button_box, 1, .5, 1);
	left_button_box.y = button_box.y;
	left_button_box.x = button_box.x;
	Rect right_button_box = left_button_box;
	right_button_box.x = button_box.x + button_box.width / 2 + (left_button_box.width * .1);
	_button_vs_ai.resize(Rect::subrect(left_button_box, 1., .3, 0));
	_button_vs_player.resize(Rect::subrect(left_button_box, 1., .3, 0));

}

Rect const &Menu::dimensions()
{
	return _dimensions;
}

/*
bool Menu::hover()
{
	Tile				*hovered;
	int					tile;
	_enabled = true;

	if (enabled() && Menu::dimensions().contains(Gui::_mouse.x, Gui::_mouse.y))
	{
		tile = get_hovered_tile();
		if (tile == -1)
		{
			if (_hovered_tile)
			{
				_hovered_tile->hover(false, true);
				_hovered_tile = nullptr;
			}
			return false;
		}
		std::cout << "Menu Hovered. Tile: " << get_hovered_tile() << "  mouse: " << Gui::_mouse << std::endl;
		hovered = &_tiles[get_hovered_tile()];
		if (hovered == _hovered_tile)
			return true;
		if (_hovered_tile)
			_hovered_tile->hover(false, true);
		_hovered_tile = hovered;
		_hovered_tile->hover(true, true);
		return true;
	}
	else if (_hovered_tile)
	{
		_hovered_tile->hover(false, true);
		_hovered_tile = NULL;
	}
	return false;
}

bool Menu::click()
{
	if (enabled() && _hovered_tile)
	{
		if (_hovered_tile->click(true))
		{
			disable();
			_game.Menu().applymove(_hovered_tile->pos());
			_hovered_tile = nullptr;
			if (_game.step(false))
			{
				_tiles[_game.move()].hover(true, false);
				_tiles[_game.move()].click(false);
				enable();
			}
			std::cout << _game.move() << std::endl;
		}
	}
	return true;
}
*/
bool Menu::enabled()
{
	return _enabled;
}

void Menu::enable()
{
	_enabled = true;
}

void Menu::disable()
{
	_enabled = false;
}

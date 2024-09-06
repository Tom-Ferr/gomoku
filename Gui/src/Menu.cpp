#include <Menu.hpp>
# include <Gui.hpp>

Menu::Menu()
: _logo(nullptr)
{

}

bool Menu::init()
{
	_logo = mlx_new_image(Gui::mlx(), Gui::texture("logo")->height, Gui::texture("logo")->width);
	_play_button = mlx_new_image(Gui::mlx(), Gui::texture("playbutton")->height, Gui::texture("playbutton")->width);
	_play_button_hover = mlx_new_image(Gui::mlx(), Gui::texture("playbutton")->height, Gui::texture("playbutton")->width);
	_dimensions = Rect::subrect(Gui::dimensions(), .8);
	Gui::apply_texture(_logo, Gui::texture("logo"));
	Gui::apply_texture(_play_button, Gui::texture("playbutton"));
	Gui::apply_texture(_play_button_hover, Gui::texture("playbutton"), Color(100, 100, 255, 150));

	mlx_image_to_window(Gui::mlx(), _logo, 0, 0);
	mlx_image_to_window(Gui::mlx(), _play_button, 0, 0);
	mlx_image_to_window(Gui::mlx(), _play_button_hover, 0, 0);

	_bgroup_vs = ButtonGroup("Versus", Rect(0, 0, 0, 0));
	_bgroup_vs.add("AI");
	_bgroup_vs.add("Player 2");
	_bgroup_starting = ButtonGroup("Starting Player", Rect(0, 0, 0, 0));
	_bgroup_starting.add("Player 1");
	_bgroup_starting.add("AI / Player2");
	_bgroup_starting.add("Random");
	_bgroup_mode = ButtonGroup("Game Mode", Rect(0, 0, 0, 0));
	_bgroup_mode.add("Standard");
	_bgroup_mode.add("Pro");
	_bgroup_mode.add("Long Pro");
	_bgroup_mode.add("Swap");
	_bgroup_mode.add("Swap2");
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
	float ratio;

	_dimensions = Rect::subrect(Gui::dimensions(), .8);
	_play_dimensions = Rect::subrect(_dimensions, .4, .15, 2);
	ratio = float(Gui::texture("logo")->height) / Gui::texture("logo")->width;
	Rect logo_dimensions = Rect::subrect(_dimensions, 1., ratio, 0);
	mlx_resize_image(_logo, logo_dimensions.width, logo_dimensions.height);
	mlx_resize_image(_play_button, _play_dimensions.width, _play_dimensions.height);
	mlx_resize_image(_play_button_hover, _play_dimensions.width, _play_dimensions.height);
	Gui::apply_texture(_logo, Gui::texture("logo"));
	Gui::apply_texture(_play_button, Gui::texture("playbuttons"), Color::white, 2, 2);
	Gui::apply_texture(_play_button_hover, Gui::texture("playbuttons"), Color::white, 0, 2);
	_logo->instances[0].x = logo_dimensions.x;
	_logo->instances[0].y = logo_dimensions.y;
	_play_button->instances[0].x = _play_dimensions.x;
	_play_button->instances[0].y = _play_dimensions.y;
	_play_button_hover->instances[0].x = _play_dimensions.x;
	_play_button_hover->instances[0].y = _play_dimensions.y;
	_play_button_hover->instances[0].enabled = false;
	_resize_buttons();
}

void Menu::_resize_buttons()
{
	Rect button_box = Rect::subrect(Gui::dimensions(), .9);
	button_box = Rect::subrect(button_box, .3, .5, 1);
	button_box.y = _logo->instances[0].y + (_logo->height * 1.1);
	_bgroup_vs.resize(Rect(button_box.x - (button_box.width * 1.1) , button_box.y, button_box.width, button_box.height));
	_bgroup_starting.resize(Rect(button_box.x, button_box.y, button_box.width, button_box.height));
	_bgroup_mode.resize(Rect(button_box.x + (button_box.width * 1.1), button_box.y, button_box.width, button_box.height));
	_bgroup_vs.depth(_logo->instances[0].z + 1);
	_bgroup_starting.depth(_logo->instances[0].z + 1);
	_bgroup_mode.depth(_logo->instances[0].z + 1);
}

Rect const &Menu::dimensions()
{
	return _dimensions;
}

void Menu::hover()
{
	_bgroup_vs.hover();
	_bgroup_starting.hover();
	_bgroup_mode.hover();
	if (_play_dimensions.contains(Gui::mouse().x, Gui::mouse().y))
	{
		_play_button_hover->instances[0].enabled = true;
		_play_button->instances[0].enabled = false;
	}
	else
	{
		_play_button_hover->instances[0].enabled = false;
		_play_button->instances[0].enabled = true;
	}
}

/*
** returns true if the game should start
*/
bool Menu::click()
{
	if (_bgroup_vs.dimensions().contains(Gui::mouse().x, Gui::mouse().y))
		_bgroup_vs.click();
	else if (_bgroup_starting.dimensions().contains(Gui::mouse().x, Gui::mouse().y))
		_bgroup_starting.click();
	else if (_bgroup_mode.dimensions().contains(Gui::mouse().x, Gui::mouse().y))
		_bgroup_mode.click();
	else if (_play_dimensions.contains(Gui::mouse().x, Gui::mouse().y))
	{
		std::cout << "Play button clicked" << std::endl;
		return true;
	}
	return false;
}

void Menu::hide()
{
	_logo->instances[0].enabled = false;
	_bgroup_vs.hide();
	_bgroup_starting.hide();
	_bgroup_mode.hide();
	_play_button_hover->instances[0].enabled = false;
	_play_button->instances[0].enabled = false;
}


void Menu::show()
{
	_logo->instances[0].enabled = true;
	_bgroup_vs.show();
	_bgroup_starting.show();
	_bgroup_mode.show();
	_play_button_hover->instances[0].enabled = false;
	_play_button->instances[0].enabled = true;
	resize();
}

t_gamemode Menu::opt_mode()
{
	return static_cast<t_gamemode>(_bgroup_mode.selected());
}

t_startingplayer Menu::opt_starting()
{
	return static_cast<t_startingplayer>(_bgroup_starting.selected());
}

t_vs Menu::opt_vs()
{
	return static_cast<t_vs>(_bgroup_vs.selected());
}

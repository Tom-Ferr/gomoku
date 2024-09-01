#include <BoardEndGame.hpp>
#include <Gui.hpp>
# include <Board.hpp>


BoardEndGame::BoardEndGame()
: _background(nullptr)
{

}

BoardEndGame::BoardEndGame(BoardEndGame const &other)
{
	*this = other;
}

BoardEndGame::~BoardEndGame() {}

BoardEndGame &BoardEndGame::operator=(BoardEndGame const &other)
{
	_dimensions = other._dimensions;
	_background = other._background;
	return (*this);
}

Rect const &BoardEndGame::dimensions()
{
	return (_dimensions);
}

void BoardEndGame::init()
{
	_background = mlx_new_image(Gui::mlx(), 1, 1);
	mlx_put_pixel(_background, 0, 0, 0x04447377);
	mlx_image_to_window(Gui::mlx(), _background, _dimensions.x, _dimensions.y);
	_player = Text("Player", true);
	_wins = Text("Wins", true);
	_esc = Text("Press ESC to exit");
	hide();
	resize();
}

void BoardEndGame::resize()
{
	Rect subrect;
	size_t center;

	_dimensions = Gui::dimensions();
	center = _dimensions.width / 2;
	subrect = Rect::subrect(_dimensions, .6);
	if (_background)
	{
		mlx_resize_image(_background, _dimensions.width, _dimensions.height);
		_background->instances[0].x = _dimensions.x;
		_background->instances[0].y = _dimensions.y;
	}
	_player.resize(Rect::subrect(subrect, 1, .4, 0));
	_wins.resize(Rect::subrect(subrect, 1, .3, 1));
	_esc.resize(Rect::subrect(subrect, 1, .1, 2));
	_player.resize(Rect(center - (_player.dimensions().width / 2), _player.dimensions().y, _player.dimensions().width, _player.dimensions().height));
	_wins.resize(Rect(center - (_wins.dimensions().width / 2), _wins.dimensions().y, _wins.dimensions().width, _wins.dimensions().height));
	_esc.resize(Rect(center - (_esc.dimensions().width / 2), _esc.dimensions().y, _esc.dimensions().width, _esc.dimensions().height));
}

void BoardEndGame::hide()
{
	_background->enabled = false;
	_player.hide();
	_wins.hide();
	_esc.hide();
}

void BoardEndGame::show(std::string winner)
{
	_background->enabled = true;
	_player = winner;
	_player.show();
	_wins.show();
	_esc.show();
	resize();
}

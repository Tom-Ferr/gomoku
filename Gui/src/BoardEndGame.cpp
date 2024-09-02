#include <BoardEndGame.hpp>
#include <Gui.hpp>
# include <Board.hpp>


BoardEndGame::BoardEndGame()
{}

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

void BoardEndGame::init()
{
	_init();
	_set_header_text("Game Over");
	_player = Text("Player", true);
	_wins = Text("Wins", true);
	hide();
}

void BoardEndGame::resize()
{
	size_t center;
	Rect subrect;

	_resize();
	center = _dimensions.x + _dimensions.width / 2;
	subrect = Rect::subrect(_dimensions, .6);
	_player.resize(Rect::subrect(subrect, 1, .4, 0));
	_wins.resize(Rect::subrect(subrect, 1, .3, 1));
	_player.center(center);
	_wins.center(center);
}

void BoardEndGame::hide()
{
	_hide();
	_player.hide();
	_wins.hide();
}

void BoardEndGame::show(std::string winner)
{
	_show();
	_player = winner;
	_player.show();
	_wins.show();
	resize();
}

bool BoardEndGame::click()
{
	if (_click())
		return true;
	return false;
}

void BoardEndGame::hover()
{
	_hover();
}

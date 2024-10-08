#include <Tile.hpp>
#include <Gui.hpp>

Tile::Tile() {}

Tile::Tile(size_t pos)
: _pos(pos), _enabled(true)
{ }

bool Tile::init()
{
	size_t x = _pos % Board::sqrt();
	size_t y = _pos / Board::sqrt();
	size_t tex = 4;

	if (x == 0 && y == 0)
		tex = 8;
	else if (x == 0 && y == Board::sqrt() - 1)
		tex = 2;
	else if (x == 0)
		tex = 5;
	else if (x == Board::sqrt() - 1 && y == 0)
		tex = 6;
	else if (x == Board::sqrt() - 1 && y == Board::sqrt() - 1)
		tex = 0;
	else if (x == Board::sqrt() - 1)
		tex = 3;
	else if (y == 0)
		tex = 7;
	else if (y == Board::sqrt() - 1)
		tex = 1;
	_tile_idx = mlx_image_to_window(
					Gui::mlx(), Board::_tile_images[tex], 0, 0);
	if (_tile_idx == -1)
		return false;
	mlx_set_instance_depth(
					&Board::_tile_images[tex]->instances[_tile_idx], 2);
	_tile_tex = tex;
	for (size_t i = 0; i < 5; i++)
	{
		_pieces[i] = mlx_image_to_window(
						Gui::mlx(), Board::_piece_images[i], 0, 0);
		if (_pieces[i] == -1)
			return false;
		piece(i).enabled = false;
	}
	return true;
}

Tile::Tile(Tile const &other)
: _pos(other._pos), _enabled(other._enabled),
_tile_tex(other._tile_tex), _tile_idx(other._tile_idx)
{
	for (size_t i = 0; i < 5; i++)
		_pieces[i] = other._pieces[i];
}


Tile::~Tile() {}

Tile &Tile::operator=(Tile const &other)
{
	_pos = other._pos;
	_enabled = other._enabled;
	_tile_tex = other._tile_tex;
	_tile_idx = other._tile_idx;
	for (size_t i = 0; i < 5; i++)
		_pieces[i] = other._pieces[i];
	return *this;
}


/*
** since image sizes is defined globally at Board, this is really
** just repositioning
*/
void Tile::resize(Rect dimensions)
{
	dimensions.x = (Board::dimensions().x + Board::dimensions().width)
						- (dimensions.width * ((_pos % Board::sqrt()) + 1));
	dimensions.y = (Board::dimensions().y + Board::dimensions().height)
						- (dimensions.height * ((_pos / Board::sqrt()) + 1));
	Board::_tile_images[_tile_tex]->instances[_tile_idx].x = dimensions.x;
	Board::_tile_images[_tile_tex]->instances[_tile_idx].y = dimensions.y;

	for (int i = 0; i < 5; i++)
	{
		piece(i).x = dimensions.x;
		piece(i).y = dimensions.y;
	}
}

bool Tile::hover(bool on, bool turn)
{
	if (!enabled())
		return false;
	piece(PT_BLACKHOVER).enabled = false;
	piece(PT_WHITEHOVER).enabled = false;
	if (on)
	{

		_hover = true;
		if (turn)
			piece(PT_BLACKHOVER).enabled = true;
		else
			piece(PT_WHITEHOVER).enabled = true;
		return true;
	}
	else
		_hover = false;
	return false;
}


void Tile::hint(bool on)
{
	piece(PT_BLACKHOVER).enabled = false;
	piece(PT_WHITEHOVER).enabled = false;
	if (on)
		piece(PT_HINT).enabled = true;
	else
		piece(PT_HINT).enabled = false;
}

bool Tile::click(bool turn)
{
	if (enabled() && _hover)
	{
		if (turn)
			piece(PT_BLACK).enabled = true;
		else
			piece(PT_WHITE).enabled = true;
		disable();
		return true;
	}
	return false;
}
void Tile::clear()
{
	for (size_t i = 0; i < 5; i++)
		piece(i).enabled = false;
	enable();
}

void Tile::show()
{
	Board::_tile_images[_tile_tex]->instances[_tile_idx].enabled = true;
}

void Tile::hide()
{
	Board::_tile_images[_tile_tex]->instances[_tile_idx].enabled = false;
}

bool Tile::enabled()
{
	return _enabled;
}

void Tile::enable()
{
	_enabled = true;
}

void Tile::disable()
{
	_enabled = false;
}

size_t &Tile::pos()
{
	return _pos;
}

mlx_instance_t &Tile::piece(int type)
{
	(void)type;
	return Board::_piece_images[type]->instances[_pieces[type]];
}
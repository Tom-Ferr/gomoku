#include <Tile.hpp>

Tile::Tile() {}

sf::Color Tile::p1_color = sf::Color(100, 100, 100, 255);
sf::Color Tile::p2_color = sf::Color::White;

Tile::Tile(Board *board, size_t pos)
: _board(board), _pos(pos), _enabled(true)
{
	(void)_set;(void)_hover;(void)_hint;

	size_t x = pos % _board->sqrt();
	size_t y = pos / _board->sqrt();
	size_t tex = 4;
	if (x == 0 && y == 0)
		tex = 8;
	else if (x == 0 && y == _board->sqrt() - 1)
		tex = 2;
	else if (x == 0)
		tex = 5;
	else if (x == _board->sqrt() - 1 && y == 0)
		tex = 6;
	else if (x == _board->sqrt() - 1 && y == _board->sqrt() - 1)
		tex = 0;
	else if (x == _board->sqrt() - 1)
		tex = 3;
	else if (y == 0)
		tex = 7;
	else if (y == _board->sqrt() - 1)
		tex = 1;


	_square = sf::RectangleShape(sf::Vector2f(417, 417));
	_piece = sf::CircleShape(20, 500);
	_piece.setFillColor(sf::Color::Transparent);
	_piece.setTexture(&_board->get_button_texture());
	_square.setTexture(&_board->get_tile_texture(tex), true);

}

Tile::Tile(Tile const &other)
: _board(other._board), _pos(other._pos), _square(other._square), _piece (other._piece), _enabled(other._enabled) {}


Tile::~Tile() {}

Tile &Tile::operator=(Tile const &other)
{
	_board = other._board;
	_pos = other._pos;
	_square = other._square;
	_piece = other._piece;
	_enabled = other._enabled;
	return *this;
}

void Tile::resize(sf::Vector2f &dimensions, sf::Vector2f position)
{
	float diameter;
	_square.setSize(dimensions);
	_square.setOrigin(dimensions.x, dimensions.y);
	_square.setPosition(position);

	diameter = (dimensions.x * .9);
	_piece.setRadius(diameter / 2);
	_piece.setOrigin(dimensions.x - (dimensions.x - diameter) / 2,
						dimensions.y - (dimensions.y - diameter) / 2);
	_piece.setPosition(position);


//	_piece.setRadius(diameter / 2);
//	_piece.setOrigin(dimensions.x / 2, dimensions.y / 2);
//	_piece.setPosition(position);
}
/*
void Tile::resize()
{
	sf::Vector2f bsize = _board->get_dimensions();
	_dimensions = sf::Vector2f(
		(bsize.x / _board->sqrt()), (bsize.y / _board->sqrt()));
	_square.setSize(_dimensions);
	_position = sf::Vector2f(
		(_board->get_position().x + bsize.x / 2) - _dimensions.x * (_pos % _board->sqrt()),
		(_board->get_position().y + bsize.y / 2) - _dimensions.y * (_pos / _board->sqrt()));
	_square.setOrigin(_dimensions.x, _dimensions.y);
	_square.setPosition(_position);
	float diameter = (_dimensions.x * .9);
	_piece.setRadius(diameter / 2);
	_piece.setOrigin(_dimensions.x - (_dimensions.x - diameter) / 2,
						_dimensions.y - (_dimensions.y - diameter) / 2);
	_piece.setPosition(_position);
}
*/

bool Tile::hover(bool on)
{
	if (!enabled())
		return false;
	if (on)
	{
		_hover = true;
		std::cout << "hover" << _pos << std::endl;
		sf::Color color;
		if (_board->TURN)
			color = Tile::p1_color;
		else
			color = Tile::p2_color;
		color.a = 100;
		_piece.setFillColor(color);
		return true;
	}
	else
	{
		_hover = false;
		_piece.setFillColor(sf::Color::Transparent);
		return false;
	}
}

bool Tile::click()
{
	if (enabled() && _hover)
	{
		std::cout << "click" << _pos << std::endl;
		sf::Color color;
		if (_board->TURN)
			color = Tile::p1_color;
		else
			color = Tile::p2_color;
		_piece.setFillColor(color);
		_board->TURN = !_board->TURN;
		disable();
		return true;
	}
	return false;
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

void Tile::draw()
{
	_board->window().draw(_square);
	_board->window().draw(_piece);
}

size_t &Tile::pos()
{
	return _pos;
}
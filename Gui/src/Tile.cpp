#include <Tile.hpp>

Tile::Tile() {}

Tile::Tile(Board *board, size_t pos)
: _board(board), _pos(pos)
{
	_shape = sf::RectangleShape(sf::Vector2f(50, 50));
	if (pos % 2)
		_shape.setFillColor(sf::Color(100, 100, 100, 255));
	else
		_shape.setFillColor(sf::Color(200, 200, 200, 255));
}

Tile::Tile(Tile const &other)
: _board(other._board), _pos(other._pos), _dimensions(other._dimensions),
_position(other._position), _shape(other._shape) {}


Tile::~Tile() {}

Tile &Tile::operator=(Tile const &other)
{
	_board = other._board;
	_pos = other._pos;
	_dimensions = other._dimensions;
	_position = other._position;
	_shape = other._shape;
	return *this;
}

void Tile::resize()
{
	sf::Vector2f bsize = _board->get_dimensions();
	_dimensions = sf::Vector2f(
		(bsize.x / _board->sqrt()), (bsize.y / _board->sqrt()));
	_shape.setSize(_dimensions);
	_position = sf::Vector2f(
		(_board->get_position().x + bsize.x / 2) - _dimensions.x * (_pos % _board->sqrt()),
		(_board->get_position().y + bsize.y / 2) - _dimensions.y * (_pos / _board->sqrt()));
	_shape.setOrigin(_dimensions.x, _dimensions.y);
	_shape.setPosition(_position);
}

void Tile::draw(sf::RenderWindow& window)
{
	window.draw(_shape);
}


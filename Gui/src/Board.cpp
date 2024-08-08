#include "Board.hpp"

Board::Board() {}

Board::Board(size_t sqrt, sf::RenderWindow* window)
: _sqrt(sqrt), _size(sqrt * sqrt), _window(window)
{
	_shape = sf::RectangleShape();
	_shape.setFillColor(sf::Color::White);
	for (size_t i = 0; i < _size; i++)
		_tiles.push_back(Tile(this, i));
	resize();
}

Board::Board(Board const &other)
: _sqrt(other._sqrt), _size(other._size), _shape(other._shape), _window(other._window),
_dimensions(other._dimensions), _position(other._position), _tiles(other._tiles) {}

Board::~Board() {}

Board &Board::operator=(Board const &other)
{
	_sqrt = other._sqrt;
	_size = other._size;
	_shape = other._shape;
	_window = other._window;
	_dimensions = other._dimensions;
	_position = other._position;
	_tiles = other._tiles;
	return *this;
}

void Board::resize()
{
	sf::Vector2u wsize = _window->getSize();
	_dimensions = sf::Vector2f (
		(std::min(wsize.x * .8, wsize.y * .8)),
		(std::min(wsize.x * .8, wsize.y * .8))
	);
	_position = sf::Vector2f(wsize.x / 2, wsize.y / 2);
	_shape.setSize(_dimensions);
	_shape.setOrigin(_dimensions.x / 2, _dimensions.y / 2);
	_shape.setPosition(_position);
	for (std::vector<Tile>::iterator it = _tiles.begin(); it != _tiles.end(); ++it)
		it->resize();
}

void Board::draw()
{
	_window->draw(_shape);
	for (std::vector<Tile>::iterator it = _tiles.begin(); it != _tiles.end(); ++it)
		it->draw(*_window);
}

size_t Board::sqrt()
{
	return _sqrt;
}

sf::Vector2f const &Board::get_dimensions() const
{
	return _dimensions;
}

sf::Vector2f const &Board::get_position() const
{
	return _position;
}
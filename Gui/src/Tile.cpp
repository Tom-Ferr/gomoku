#include <Tile.hpp>
#include <Gui.hpp>
Tile::Tile() {}

Tile::Tile(size_t pos)
: _pos(pos), _enabled(true)
{
	std::cout << "INITIAL BOARD POSITION: " << Board::dimensions() << std::endl;
	//(void)_set;(void)_hover;(void)_hint;

	size_t x = pos % Board::sqrt();
	size_t y = pos / Board::sqrt();
	size_t tex = 4;
	if (x == 0 && y == 0)
		tex = 0;
	else if (x == 0 && y == Board::sqrt() - 1)
		tex = 6;
	else if (x == 0)
		tex = 3;
	else if (x == Board::sqrt() - 1 && y == 0)
		tex = 2;
	else if (x == Board::sqrt() - 1 && y == Board::sqrt() - 1)
		tex = 8;
	else if (x == Board::sqrt() - 1)
		tex = 5;
	else if (y == 0)
		tex = 1;
	else if (y == Board::sqrt() - 1)
		tex = 7;
	_tile_idx = mlx_image_to_window(Gui::mlx(), Board::_tile_images[tex], 0, 0);
	_tile_tex = tex;
	for (size_t i = 0; i < 5; i++)
		_pieces[i] = mlx_image_to_window(Gui::mlx(), Board::_piece_images[i], 0, 0);
	//_square = sf::RectangleShape(sf::Vector2f(417, 417));
	//_piece = sf::CircleShape(20, 500);
	//_piece.setFillColor(sf::Color::Transparent);
	//_piece.setTexture(Gui::texture(TX_PIECE, tex));
	//_square.setTexture(Gui::texture(TX_TILE, tex), true);

}

Tile::Tile(Tile const &other)
: _pos(other._pos), _enabled(other._enabled),
_tile_tex(other._tile_tex), _tile_idx(other._tile_idx)
{
	for (size_t i = 0; i < 5; i++)
		_pieces[i] = other._pieces[i];
}//, _square(other._square), _piece (other._piece), _enabled(other._enabled) {}


Tile::~Tile() {}

Tile &Tile::operator=(Tile const &other)
{
	_pos = other._pos;
	_enabled = other._enabled;
	_tile_tex = other._tile_tex;
	_tile_idx = other._tile_idx;
	for (size_t i = 0; i < 5; i++)
		_pieces[i] = other._pieces[i];
	//_square = other._square;
	//_piece = other._piece;
	return *this;
}

/*
** since image sizes is defined globally at Board, this is really
** just repositioning
*/
void Tile::resize(Rect dimensions)
{
	(void)dimensions;
	//std::cout << "Old Tile resize:" << dimensions << std::endl;
	dimensions.x = Board::dimensions().x + dimensions.width * (_pos % Board::sqrt());
	dimensions.y = Board::dimensions().y + dimensions.height * (_pos / Board::sqrt());
	//std::cout << "Board dimensions:" << _board->dimensions() << std::endl;
	//std::cout << "New Tile resize:" << dimensions << std::endl;
	Board::_tile_images[_tile_tex]->instances[_tile_idx].x = dimensions.x;
	Board::_tile_images[_tile_tex]->instances[_tile_idx].y = dimensions.y;

	for (int i = 4; i >= 0; i--)
	{
		Board::_piece_images[i]->instances[_pieces[i]].x = dimensions.x;
		Board::_piece_images[i]->instances[_pieces[i]].y = dimensions.y;
		Board::_piece_images[i]->instances[_pieces[i]].enabled = false;
	}
	Board::_piece_images[_pos % 5]->instances[_pieces[_pos % 5]].enabled = true;
	//float diameter;
	//_square.setSize(dimensions);
	//_square.setOrigin(dimensions.x, dimensions.y);
	//_square.setPosition(position);
//
	//diameter = (dimensions.x * .9);
	//_piece.setRadius(diameter / 2);
	//_piece.setOrigin(dimensions.x - (dimensions.x - diameter) / 2,
	//					dimensions.y - (dimensions.y - diameter) / 2);
	//_piece.setPosition(position);


//	_piece.setRadius(diameter / 2);
//	_piece.setOrigin(dimensions.x / 2, dimensions.y / 2);
//	_piece.setPosition(position);
}

/*
bool Tile::hover(bool on, bool turn)
{
	if (!enabled())
		return false;
	if (on)
	{
		_hover = true;
		std::cout << "hover" << _pos << std::endl;
		sf::Color color;
		if (turn)
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

bool Tile::click(bool turn)
{
	if (enabled() && _hover)
	{
		std::cout << "click" << _pos << std::endl;
		sf::Color color;
		if (turn)
			color = Tile::p1_color;
		else
			color = Tile::p2_color;
		_piece.setFillColor(color);
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
*/
size_t &Tile::pos()
{
	return _pos;
}
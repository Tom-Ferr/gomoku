#include <Board.hpp>
# include <Gui.hpp>

mlx_image_t			*Board::_tile_images[9] = {nullptr};
mlx_image_t			*Board::_piece_images[5] = {nullptr};
Rect				Board::_dimensions;
size_t				Board::_sqrt = 0;
size_t				Board::_size = 0;

Board::Board()
: _background(nullptr)
{ }

bool Board::init()
{
	_background = mlx_new_image(Gui::mlx(), BACKGROUND_SIZE, BACKGROUND_SIZE);
	Board::_dimensions = Rect::subrect(Gui::dimensions(), .9);
	Gui::apply_texture(_background, Gui::_board_texture);
	mlx_image_to_window(Gui::mlx(), _background, 0, 0);
	return (true);
}

bool Board::reset(size_t sqrt)
{
 	_sqrt = sqrt;
	_size = sqrt * sqrt;
	_reset();
	_init();
	return (true);
}

Board::Board(Board const &other)
{
	*this = other;
}

Board::~Board()
{
	if (_background)
		mlx_delete_image(Gui::mlx(), _background);
	_background = nullptr;
}

Board &Board::operator=(Board const &other)
{
	(void)other;
//	std::cout << "Board copy assignment" << std::endl;
//	_sqrt = other._sqrt;
//	_size = other._size;
//	_background = other._background;
//	_dimensions = other._dimensions;
//	_tiles = other._tiles;
	//_tiles = other._tiles;
	//_window = other._window;
	//_dimensions = other._dimensions;
	//_position = other._position;
	//_tiles = other._tiles;
	//_tiles_dimensions = other._tiles_dimensions;
	//_hovered_tile = other._hovered_tile;
	//_enabled = other._enabled;
	//_game = other._game;
	return *this;
}

/*
** clears the tile_backgrounds (and all piece images)
** to ensure that the drawing queue is empty.
*/
void Board::_reset()
{
	_tiles.clear();
	for (size_t i = 0; i < 9; i++)
	{
		if (Board::_tile_images[i])
			mlx_delete_image(Gui::mlx(), Board::_tile_images[i]);
		Board::_tile_images[i] = nullptr;
	}
	for (size_t i = 0; i < 5; i++)
	{
		if (Board::_piece_images[i])
			mlx_delete_image(Gui::mlx(), Board::_piece_images[i]);
		Board::_piece_images[i] = nullptr;
	}
}

bool Board::_init()
{
	for (size_t i = 0; i < 9; i++)
	{
		Board::_tile_images[i] = mlx_new_image(Gui::mlx(), TILE_SIZE, TILE_SIZE);
		Gui::apply_texture(Board::_tile_images[i], Gui::_tile_texture, i);
	}
	for (size_t i = 0; i < 5; i++)
		Board::_piece_images[i] = mlx_new_image(Gui::mlx(), BUTTON_SIZE, BUTTON_SIZE);
	Gui::apply_texture(Board::_piece_images[0], Gui::_button_texture, Color::white);
	Gui::apply_texture(Board::_piece_images[1], Gui::_button_texture, Color::black);
	Gui::apply_texture(Board::_piece_images[2], Gui::_button_texture, Color::white_alpha);
	Gui::apply_texture(Board::_piece_images[3], Gui::_button_texture, Color::black_alpha);
	Gui::apply_texture(Board::_piece_images[4], Gui::_button_texture, Color::yellow);
	for (size_t i = 0; i < Board::_size; i++)
		_tiles.push_back(Tile(i));
	resize();

	//_game = Game(_sqrt);
	//_shape = sf::RectangleShape();
	//_shape.setTexture(Gui::texture(TX_BG));
	//for (size_t i = 0; i < _size; i++)
	//	_tiles.push_back(Tile(this, i));
	//resize();
	/*
	** depending on the turn, we'll make a game step here.
	*/
	return true;
}

void Board::resize()
{
	Rect tile_dimensions;

	_dimensions = Rect::subrect(Gui::dimensions(), .9);
	std::cout << "New Board resize:" << _dimensions;
	mlx_resize_image(_background, _dimensions.width, _dimensions.height);
	Gui::apply_texture(_background, Gui::_board_texture);
	_background->instances[0].x = _dimensions.x;
	_background->instances[0].y = _dimensions.y;
	std::cout << "Board dimensions:" << _dimensions << std::endl;
	tile_dimensions = Rect::subrect(_dimensions, 1.0 / sqrt());
	for (size_t i = 0; i < 9; i++)
	{
		mlx_resize_image(Board::_tile_images[i], tile_dimensions.width, tile_dimensions.height);
		Gui::apply_texture(Board::_tile_images[i], Gui::_tile_texture, i);
	}
	for (size_t i = 0; i < 5; i++)
		mlx_resize_image(Board::_piece_images[i], tile_dimensions.width, tile_dimensions.height);
	Gui::apply_texture(Board::_piece_images[0], Gui::_button_texture, Color::white);
	Gui::apply_texture(Board::_piece_images[1], Gui::_button_texture, Color::black);
	Gui::apply_texture(Board::_piece_images[2], Gui::_button_texture, Color::white_alpha);
	Gui::apply_texture(Board::_piece_images[3], Gui::_button_texture, Color::black_alpha);
	Gui::apply_texture(Board::_piece_images[4], Gui::_button_texture, Color::yellow);
	for (std::vector<Tile>::iterator it = _tiles.begin(); it != _tiles.end(); ++it)
	{
		std::cout << "Resizing Tile" << std::endl;
		std::cout << "dim: " <<  dimensions() << std::endl;
		std::cout << "This addr: " << this << std::endl;
		it->resize(tile_dimensions);
	}

//	sf::Vector2u wsize = _window->getSize();
//	_dimensions = sf::Vector2f (
//		(std::min(wsize.x * .8, wsize.y * .8)),
//		(std::min(wsize.x * .8, wsize.y * .8))
//	);
//	_position = sf::Vector2f(wsize.x / 2, wsize.y / 2);
//	_shape.setSize(_dimensions);
//	_shape.setOrigin(_dimensions.x / 2, _dimensions.y / 2);
//	_shape.setPosition(_position);
//	_tiles_dimensions = sf::Vector2f(
//		(_dimensions.x / sqrt()), (_dimensions.y / sqrt()));
//	for (std::vector<Tile>::iterator it = _tiles.begin();
//			it != _tiles.end(); ++it)
//		it->resize(_tiles_dimensions, get_tile_position(it->pos()));
}

size_t Board::size()
{
	return Board::_size;
}

size_t Board::sqrt()
{
	return Board::_sqrt;
}

Rect const &Board::dimensions()
{
	return Board::_dimensions;
}

/*
void Board::draw()
{
	_window->draw(_shape);
	for (std::vector<Tile>::iterator it = _tiles.begin(); it != _tiles.end(); ++it)
		it->draw();
}


sf::RenderWindow &Board::window() const
{
	return *_window;
}

sf::Vector2f const &Board::get_dimensions() const
{
	return _dimensions;
}

sf::Vector2f const &Board::get_position() const
{
	return _position;
}

bool Board::hover()
{
	Tile				*hovered;
	sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(window()));
	if (enabled() && _shape.getGlobalBounds().contains(mouse))
	{
		hovered = &_tiles[get_hovered_tile(mouse)];
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

bool Board::click()
{
	if (enabled() && _hovered_tile && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (_hovered_tile->click(true))
		{
			disable();
			draw();
			_window->display();
			_game.board().applymove(_hovered_tile->pos());
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

bool Board::enabled()
{
	return _enabled;
}

void Board::enable()
{
	_enabled = true;
}

void Board::disable()
{
	_enabled = false;
}

sf::Vector2f Board::get_tile_position(int pos)
{
	return sf::Vector2f(
	(get_position().x + get_dimensions().x / 2) - get_tile_dimensions().x * (pos % sqrt()),
	(get_position().y + get_dimensions().y / 2) - get_tile_dimensions().y * (pos / sqrt()));
}

sf::Vector2f& Board::get_tile_dimensions()
{
	return _tiles_dimensions;
}

size_t Board::get_hovered_tile(sf::Vector2f &mouse)
{
    sf::Vector2f board_center(
        get_position().x + get_dimensions().x / 2,
        get_position().y + get_dimensions().y / 2
    );
	sf::Vector2i tile(
		(board_center.x - mouse.x) / get_tile_dimensions().x,
		(board_center.y - mouse.y) / get_tile_dimensions().y
	);
    return (tile.y * sqrt() + tile.x);
}
*/

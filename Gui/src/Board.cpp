#include <Board.hpp>

Board::Board() {}

Board::Board(size_t sqrt, sf::RenderWindow* window)
: _sqrt(sqrt), _size(sqrt * sqrt), _window(window), _hovered_tile(nullptr), _enabled(true)
{
	TURN = true;
	_game = Game(sqrt);
	_background = sf::RectangleShape();
	_shape = sf::RectangleShape();
	_load_textures();
	_shape.setTexture(&_texture);
	_background.setTexture(&_background_texture);

	for (size_t i = 0; i < _size; i++)
		_tiles.push_back(Tile(this, i));
	resize();
}

Board::Board(Board const &other)
: _sqrt(other._sqrt), _size(other._size), _shape(other._shape), _window(other._window),
_dimensions(other._dimensions), _position(other._position), _tiles(other._tiles), _hovered_tile(nullptr), _enabled(other._enabled) {}

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
	_enabled = other._enabled;
	return *this;
}

void Board::_load_texture(sf::Texture &texture, std::string path, sf::IntRect rect, int type)
{
	uint8_t pixels[4] = {255, 255, 255, 255};

	if ((rect == sf::IntRect() && !texture.loadFromFile(path))
		|| (rect != sf::IntRect() && !texture.loadFromFile(path, rect)))
	{
		std::cerr << "Warning: Error loading texture: " << path << std::endl;
		if (texture.create(1, 1))
		{
			if (type == TX_BG)
				pixels[0] = 200;
			else if (type == TX_TILE || type == TX_BACKGROUND)
				pixels[3] = 0;
			else if (type == TX_BLACK)
			{
				pixels[0] = 0;
				pixels[1] = 0;
				pixels[2] = 0;
			}
			texture.update(pixels);
			texture.setRepeated(true);
		}
	}
	else
	{
		texture.setRepeated(false);
		texture.setSmooth(true);
	}
}

void Board::_load_textures()
{
	_load_texture(_texture, "assets/marble_bg.jpg");
	for (size_t i = 0; i < 9; i++)
	{
		_load_texture(_tile_texture[i], "assets/marble_tiles.png",
			sf::IntRect(
				(i % 3) * TILE_SIZE,
				(i / 3) * TILE_SIZE,
				TILE_SIZE, TILE_SIZE
			), TX_TILE);
	}
	_load_texture(_button_texture, "assets/white_piece.png", sf::IntRect(), TX_WHITE);
	_load_texture(_background_texture, "assets/background.jpg", sf::IntRect(), TX_BACKGROUND);
}

void Board::resize()
{
	sf::Vector2u wsize = _window->getSize();
	_background.setSize(sf::Vector2f(wsize.x, wsize.y));
	_dimensions = sf::Vector2f (
		(std::min(wsize.x * .8, wsize.y * .8)),
		(std::min(wsize.x * .8, wsize.y * .8))
	);
	_position = sf::Vector2f(wsize.x / 2, wsize.y / 2);
	_shape.setSize(_dimensions);
	_shape.setOrigin(_dimensions.x / 2, _dimensions.y / 2);
	_shape.setPosition(_position);
	_tiles_dimensions = sf::Vector2f(
		(_dimensions.x / sqrt()), (_dimensions.y / sqrt()));
	for (std::vector<Tile>::iterator it = _tiles.begin();
			it != _tiles.end(); ++it)
		it->resize(_tiles_dimensions, get_tile_position(it->pos()));
}

void Board::draw()
{
	_window->draw(_background);
	_window->draw(_shape);
	for (std::vector<Tile>::iterator it = _tiles.begin(); it != _tiles.end(); ++it)
		it->draw();
}

size_t Board::sqrt()
{
	return _sqrt;
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
			_hovered_tile->hover(false);
		_hovered_tile = hovered;
		_hovered_tile->hover(true);
		return true;
	}
	else if (_hovered_tile)
	{
		_hovered_tile->hover(false);
		_hovered_tile = NULL;
	}
	return false;
}

bool Board::click()
{
	if (enabled() && _hovered_tile && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (_hovered_tile->click())
		{
			disable();
			draw();
			_window->display();
			_game.board().applymove(_hovered_tile->pos(), false);
			_hovered_tile = nullptr;
			if (_game.step())
			{
				_tiles[_game.move()].hover(true);
				_tiles[_game.move()].click();
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

sf::Texture& Board::get_tile_texture(int i)
{
	return _tile_texture[i];
}

sf::Texture& Board::get_button_texture()
{
	return _button_texture;
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
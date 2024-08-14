#include <Board.hpp>

Board::Board() {}

Board::Board(size_t sqrt, sf::RenderWindow* window)
: _sqrt(sqrt), _size(sqrt * sqrt), _window(window), _enabled(true)
{
	TURN = true;
	_shape = sf::RectangleShape();
	_load_textures();
	_shape.setTexture(&_texture);
	for (size_t i = 0; i < _size; i++)
		_tiles.push_back(Tile(this, i));
	resize();
}

Board::Board(Board const &other)
: _sqrt(other._sqrt), _size(other._size), _shape(other._shape), _window(other._window),
_dimensions(other._dimensions), _position(other._position), _tiles(other._tiles), _enabled(other._enabled) {}

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
			else if (type == TX_TILE)
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
	_load_texture(_button_texture[0], "assets/white_piece.png", sf::IntRect(), TX_WHITE);
	_load_texture(_button_texture[1], "assets/black_piece.png", sf::IntRect(), TX_BLACK);
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
	for (std::vector<Tile>::iterator it = _tiles.begin();
			it != _tiles.end(); ++it)
		it->resize();
}

void Board::draw()
{
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
	sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(window()));
	if (enabled() && _shape.getGlobalBounds().contains(mouse))
	{
		for (std::vector<Tile>::iterator it = _tiles.begin();
				it != _tiles.end(); ++it)
			it->hover(mouse);
		return true;
	}
	return false;
}

bool Board::click()
{
	if (enabled() && hover() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(window()));
		for (std::vector<Tile>::iterator it = _tiles.begin();
				it != _tiles.end(); ++it)
			it->click(mouse);
	}
	return false;
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

sf::Texture& Board::get_button_texture(int i)
{
	return _button_texture[i];
}

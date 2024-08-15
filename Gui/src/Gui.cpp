#include  <Gui.hpp>

sf::Texture 		Gui::_background_texture;
sf::Texture 		Gui::_button_texture;
sf::Texture 		Gui::_tile_texture[9];
sf::Texture 		Gui::_texture;
sf::RenderWindow*	Gui::_window;

Gui::Gui() {}

Gui::Gui(sf::RenderWindow* window)
{
	Gui::_window = window;
	_load_textures();
	_background = sf::RectangleShape();
	_background.setSize(sf::Vector2f(_window->getSize().x, _window->getSize().y));
	_background.setTexture(&_background_texture);
}

void Gui::_load_texture(sf::Texture &texture, std::string path, sf::IntRect rect, int type)
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
			else if (type == TX_PIECE)
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

void Gui::_load_textures()
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
	_load_texture(_button_texture, "assets/white_piece.png", sf::IntRect(), TX_PIECE);
	_load_texture(_background_texture, "assets/background.jpg", sf::IntRect(), TX_BACKGROUND);
}

sf::Texture *Gui::texture(int type, int index)
{
	if (type == TX_BG)
		return &_texture;
	else if (type == TX_TILE)
		return &_tile_texture[index];
	else if (type == TX_PIECE)
		return &_button_texture;
	else if (type == TX_BACKGROUND)
		return &_background_texture;
	return &_texture;
}

void Gui::draw()
{
	_window->draw(_background);
}

sf::RenderWindow *Gui::window()
{
	return _window;
}

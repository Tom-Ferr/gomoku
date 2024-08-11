#ifndef BOARD_HPP
# define BOARD_HPP

# include <SFML/Graphics.hpp>
# include <Tile.hpp>

# define TILE_SIZE 500
# define BACKGROUND_SIZE 1024
# define BUTTON_SIZE 200

enum e_texturetype
{
	TX_BG,
	TX_TILE,
	TX_BLACK,
	TX_WHITE
};

class Tile;

class Board
{
	private:
		sf::Texture 		_button_texture[2];
		sf::Texture 		_tile_texture[9];
		sf::Texture 		_texture;
		size_t				_sqrt;
		size_t				_size;
		sf::RectangleShape	_shape;
		sf::RenderWindow*	_window;
		sf::Vector2f		_dimensions;
		sf::Vector2f		_position;
		std::vector<Tile>	_tiles;
		bool				_enabled;

	void _load_texture(sf::Texture &texture, std::string path,
						sf::IntRect rect = sf::IntRect(), int type=TX_BG);
	void _load_textures();


	public:
		bool				TURN;
		Board(size_t sqrt, sf::RenderWindow* window);
		Board();
		Board(Board const &other);
		~Board();
		Board &operator=(Board const &other);
		void resize();
		void draw();
		size_t sqrt();
		sf::RenderWindow &window() const;
		sf::Vector2f const &get_dimensions() const;
		sf::Vector2f const &get_position() const;
		bool hover();
		bool click();
		bool enabled();
		void enable();
		void disable();
		sf::Texture& get_tile_texture(int i);
		sf::Texture& get_button_texture(int i);

};

#endif

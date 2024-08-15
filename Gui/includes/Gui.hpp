#ifndef GUI_HPP
# define GUI_HPP
# include <SFML/Graphics.hpp>
# include <iostream>
# include <Board.hpp>

enum e_texturetype
{
	TX_BG,
	TX_TILE,
	TX_PIECE,
	TX_BACKGROUND
};

class Gui
{
	private:
		static sf::Texture 			_background_texture;
		static sf::Texture 			_button_texture;
		static sf::Texture 			_tile_texture[9];
		static sf::Texture 			_texture;
		static sf::RenderWindow*	_window;

		sf::RectangleShape			_background;
		Gui();
		void _load_texture(sf::Texture &texture, std::string path, sf::IntRect rect=sf::IntRect(), int type=TX_BG);
		void _load_textures();

	public:
		Gui(sf::RenderWindow* window);
		static sf::Texture *texture(int type, int index=0);
		static sf::RenderWindow *window();
		void draw();
};

#endif

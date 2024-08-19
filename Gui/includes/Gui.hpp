#ifndef GUI_HPP
# define GUI_HPP
#include <string.h>

# include <iostream>
# include <Rect.hpp>
# include <Board.hpp>
#include <MLX42/MLX42.h>

/*
enum e_texturetype
{
	TX_BG,
	TX_TILE,
	TX_PIECE,
	TX_BACKGROUND
};
*/

#define WIDTH 1280
#define HEIGHT 720
class Color ;

class Color
{
	public:
		static Color const white;
		static Color const black;
		static Color const yellow;
		static Color const white_alpha;
		static Color const black_alpha;

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		Color()
		: r(255), g(255), b(255), a(255)
		{};
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: r(r), g(g), b(b), a(a)
		{};
		Color(uint8_t r, uint8_t g, uint8_t b)
		: r(r), g(g), b(b), a(255)
		{};
		Color(const Color &other)
		: r(other.r), g(other.g), b(other.b), a(other.a)
		{};
		~Color()
		{};
		Color &operator=(const Color &other)
		{
			r = other.r;
			g = other.g;
			b = other.b;
			a = other.a;
			return *this;
		};
		bool operator==(const Color &other) const
		{
			return (r == other.r && g == other.g
					&& b == other.b && a == other.a);
		};
		bool operator!=(const Color &other) const
		{
			return (!(*this == other));
		};
};

class Gui
{
	private:
		bool _init();
		void _run();
		bool _load_textures();
		bool _load_texture(mlx_texture_t *&texture, const char* path);
		bool _load_background();
		void _resize(int width, int height);

	public:
		static mlx_t					*_mlx;
		static mlx_texture_t			*_button_texture;
		static mlx_texture_t			*_tile_texture;
		static mlx_texture_t			*_board_texture;
		static Rect						_dimensions;

		mlx_texture_t 					*_background_texture;
		mlx_image_t						*_background;
		Board							_board;

		/*
		** input hooks
		*/
		static void resize_hook(int32_t width, int32_t height, void* param);
		static void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void *param);
		static void	cursor_hook(double x, double y, void *param);
		static void	close_hook(void *param);
		static void key_hook(mlx_key_data_t keydata, void* param);
		//static int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
		//static void ft_randomize(void* param);
		//static void ft_hook(void* param);

		/*
		** helpers
		*/
		static void	apply_texture(mlx_image_t* image, mlx_texture_t* texture, Color const &color=Color::white);
		static void apply_texture(mlx_image_t *image, mlx_texture_t *texture, size_t index);

		/*
		** static getters
		*/
		static Rect const &dimensions();
		static mlx_t *mlx();

	public:
		Gui();
		~Gui();
		//static sf::Texture *texture(int type, int index=0);
		//static sf::RenderWindow *window();
		//void draw();
};

#endif

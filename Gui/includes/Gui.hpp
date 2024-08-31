#ifndef GUI_HPP
# define GUI_HPP
#include <string.h>
# include <chrono>
# include <iostream>
# include <Rect.hpp>
# include <Board.hpp>
# include <Menu.hpp>
#include <Color.hpp>
#include <MLX42/MLX42.h>


typedef enum e_gamestate
{
	GS_BOARD,
	GS_MENU
} t_gamestate;

#define WIDTH 1280
#define HEIGHT 720


class Gui
{
	private:
		bool _init();
		void _run();
		bool _load_textures();
		bool _load_texture(mlx_texture_t *&texture, const char* path);
		bool _load_background();
		void _resize(int width, int height);
		static std::map<std::string, mlx_texture_t*>	_textures;

	public:
		static mlx_t				*_mlx;
		static Rect					_dimensions;
		static Rect					_mouse;
		mlx_image_t					*_background;
		Board						_board;
		Menu						_menu;
		t_gamestate					_gamestate;

		/*
		** input hooks
		*/
		static void resize_hook(int32_t width, int32_t height, void* param);
		static void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void *param);
		static void	cursor_hook(double x, double y, void *param);
		static void	close_hook(void *param);
		static void key_hook(mlx_key_data_t keydata, void* param);
		static void board_loop_hook(void *param);

		/*
		** helpers
		*/
		static void	apply_texture(mlx_image_t* image, mlx_texture_t* texture, Color const &color=Color::white, size_t index=0, size_t gridsize=1, Rect dest = Rect(0, 0, 0, 0));

		/*
		** static getters
		*/
		static Rect const &dimensions();
		static mlx_t *mlx();
		static Rect const &mouse();
		static mlx_texture_t *&texture(std::string name);

	public:
		Gui();
		~Gui();
};

#endif

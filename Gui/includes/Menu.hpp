#ifndef MENU_HPP
# define MENU_HPP

# include <Rect.hpp>
# include <Text.hpp>
# include <Button.hpp>
# include <MLX42/MLX42.h>

class Menu
{
	private:
		mlx_image_t			*_logo;
		mlx_image_t			*_play_button;
		mlx_image_t			*_play_button_hover;
		Button				_button_vs_ai;
		Button				_button_vs_player;
		Rect				_dimensions;
		bool				_enabled;

		void _resize_buttons();
	public:
		Menu();
		Menu(Menu const &other);
		~Menu();
		bool init();
		void resize();
		Menu &operator=(Menu const &other) ;
		Rect const &dimensions();
		//bool hover();
		//bool click();
		bool enabled();
		void enable();
		void disable();
};

#endif

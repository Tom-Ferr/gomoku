#ifndef MENU_HPP
# define MENU_HPP

# include <Rect.hpp>
# include <Text.hpp>
# include <Button.hpp>
# include <ButtonGroup.hpp>
# include <MLX42/MLX42.h>

class Menu
{
	private:
		mlx_image_t			*_logo;
		mlx_image_t			*_play_button;
		mlx_image_t			*_play_button_hover;
		ButtonGroup			_bgroup_vs;
		ButtonGroup			_bgroup_starting;
		ButtonGroup			_bgroup_mode;
		Rect				_dimensions;
		Rect				_play_dimensions;

		void _resize_buttons();
	public:
		Menu();
		Menu(Menu const &other);
		~Menu();
		bool init();
		void resize();
		Menu &operator=(Menu const &other) ;
		Rect const &dimensions();
		void hover();
		bool click();
		void show();
		void hide();
};

#endif

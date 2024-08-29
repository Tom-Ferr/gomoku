#ifndef BOARDMODE_HPP
# define BOARDMODE_HPP

# include <string>
# include <Rect.hpp>
# include <Color.hpp>
# include <Text.hpp>
# include <Info.hpp>
# include <MLX42/MLX42.h>
# include <common.hpp>
# include <ButtonGroup.hpp>

class BoardMode
{
	private:
		mlx_image_t						*_background;
		mlx_image_t						*_centerbox;
		mlx_image_t						*_header;
		mlx_image_t						*_ok_button;
		mlx_image_t						*_ok_button_hover;
		Rect							_dimensions;
		Text							_mode;
		Text							_description;
		Text							_info;
		ButtonGroup						_buttons;


		void _resize_background();
		void _resize_box();
		void _box_texture();
		void _resize_ok();
		void _resize_header();

	public:
		BoardMode();
		BoardMode(BoardMode const &other);
		~BoardMode();
		void init();
		void resize();
		BoardMode &operator=(BoardMode const &other);
		Rect const &dimensions();
		void hide();
		void show(std::string mode, bool selecting=true);
};

#endif

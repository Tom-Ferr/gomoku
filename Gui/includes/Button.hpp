#ifndef BUTTON_HPP
# define BUTTON_HPP

# include <string>
# include <Rect.hpp>
# include <Color.hpp>
# include <Text.hpp>
# include <MLX42/MLX42.h>

enum e_btn
{
	BTN_NORMAL,
	BTN_HOVER,
	BTN_SELECTED
};

class Button
{
	private:
		static mlx_image_t			*_button;
		static mlx_image_t			*_button_hover;
		static mlx_image_t			*_button_selected;

		int32_t						_buttons[3];
		Text						_text;
		Rect						_dimensions;
		bool						_selected;
		bool						_hovered;
		void _init();
		void _init_static();

	public:
		Button();
		Button(std::string const &str);
		Button(Button const &other);
		~Button();
		void resize(Rect dimensions=Rect(255, 255, 50, 300));
		Button &operator=(Button const &other);
		Rect const &dimensions();
		void select(bool selected=true);
		void hover();
		void hide();
		void show();
		void depth(size_t depth);
};

#endif

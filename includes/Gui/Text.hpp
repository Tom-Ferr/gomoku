#ifndef TEXT_HPP
# define TEXT_HPP

# include <string>
# include <Rect.hpp>
# include <Color.hpp>
# include <MLX42/MLX42.h>

# define FONT_GRIDSIZE 10
# define LINE_WIDTH 70

class Text
{
	private:
		mlx_image_t		*_image;
		Rect			_dimensions;
		std::string		_text;
		bool			_bold;
		int				_instance;
		void			_init();

	public:
		Text();
		Text(std::string const &str, bool bold = false);
		Text(Text const &other);
		~Text();
		void resize(Rect const &dimensions);
		void resize(size_t height);
		void center(size_t center);
		void center(size_t x, size_t y);
		void depth(size_t depth);
		Text &operator=(Text const &other);
		Text &operator=(std::string const &str);
		Rect const &dimensions();
		std::string &text();
		void show();
		void hide();
		bool init();
};

#endif

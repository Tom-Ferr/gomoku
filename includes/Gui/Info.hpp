#ifndef INFO_HPP
# define INFO_HPP

# include <string>
# include <Rect.hpp>
# include <Color.hpp>
# include <Text.hpp>
# include <MLX42/MLX42.h>
# include <common.hpp>

/*
** Info class for displaying information on the board.
** Used with the Statusbar.
** Consists of a title and a description or image (piece).
*/
class Info
{
	private:
		static mlx_image_t			*_piece_images[2];
		size_t						_buttons[2];
		Rect						_dimensions;
		Text						_text;
		Text						_description;
		t_piecetype					_image;
		bool						_is_image;
		void _init();
		void _init_images();

	public:
		Info();
		Info(std::string const &str, std::string const &description="");
		Info(std::string const &str, t_piecetype image);
		Info(Info const &other);
		~Info();
		void resize(Rect const &dimensions=Rect(255, 255, 50, 300));
		Info &operator=(Info const &other);
		Info &operator=(std::string const &description);
		Info &operator=(t_piecetype image);
		Rect const &dimensions();
		void hide();
		void show();
		void set_title(std::string const &str);
};

#endif

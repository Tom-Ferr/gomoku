#include  <Gui.hpp>

mlx_t			*Gui::_mlx = nullptr;
mlx_texture_t	*Gui::_piece_texture = nullptr;
mlx_texture_t	*Gui::_tile_texture = nullptr;
mlx_texture_t	*Gui::_board_texture = nullptr;
mlx_texture_t	*Gui::_logo_texture = nullptr;
mlx_texture_t	*Gui::_playbutton_texture = nullptr;
mlx_texture_t	*Gui::_button_texture = nullptr;
mlx_texture_t	*Gui::_font_regular_texture = nullptr;
mlx_texture_t	*Gui::_font_bold_texture = nullptr;
mlx_texture_t	*Gui::_font_heavy_texture = nullptr;
std::map<std::string, mlx_texture_t*> Gui::_textures;
Rect			Gui::_dimensions;
Rect			Gui::_mouse;

Gui::Gui()
{
	if (_init())
		_run();
}

Gui::~Gui()
{
	if (Gui::_logo_texture)
		mlx_delete_texture(Gui::_logo_texture);
	if (Gui::_piece_texture)
		mlx_delete_texture(Gui::_piece_texture);
	if (Gui::_tile_texture)
		mlx_delete_texture(Gui::_tile_texture);
	if (Gui::_board_texture)
		mlx_delete_texture(Gui::_board_texture);
	if (Gui::_piece_texture)
		mlx_delete_texture(Gui::_playbutton_texture);
	if (Gui::_button_texture)
		mlx_delete_texture(Gui::_button_texture);
	if (Gui::_font_regular_texture)
		mlx_delete_texture(Gui::_font_regular_texture);
	if (Gui::_font_bold_texture)
		mlx_delete_texture(Gui::_font_bold_texture);
	if (Gui::_font_heavy_texture)
		mlx_delete_texture(Gui::_font_heavy_texture);
	if (_background)
		mlx_delete_texture(_background_texture);
	mlx_terminate(_mlx);
}
/*
** initializes minilibx and loads textures and background
** if something fails, returns false
*/
bool Gui::_init()
{
	Gui::_dimensions = Rect(0, 0, WIDTH, HEIGHT);
	Gui::_mouse = Rect(0, 0, 0, 0);
	_gamestate = GS_MENU;
	if (!(_mlx = mlx_init(WIDTH, HEIGHT, "Gomoku", true)))
	{
		std::cout << "Error: mlx_init failed. " << mlx_strerror(mlx_errno) << std::endl;
		return (false);
	}
	if (!(_load_textures()))
		return (false);
	if (!(_load_background()))
		return (false);
	mlx_image_to_window(_mlx, _background, 0, 0);
	mlx_set_instance_depth(&_background->instances[0], 0);
	_board.init();
	_menu.init();
	return (true);
}

bool Gui::_load_background()
{
	_background = mlx_new_image(_mlx, WIDTH, HEIGHT);
	if (!_background)
	{
		std::cout << "Error: could not create image. ";
		std::cout << mlx_strerror(mlx_errno) << std::endl;
		return (false);
	}
	apply_texture(_background, _background_texture);
	return (true);
}

bool Gui::_load_texture(mlx_texture_t	*&texture, const char* path)
{
	texture = mlx_load_png(path);
	if (!texture)
	{
		std::cout << "Error: could not load texture '";
		std::cout << path << "'. ";
		std::cout << mlx_strerror(mlx_errno) << std::endl;
		return (false);
	}
	return (true);
}

bool Gui::_load_textures()
{
	if (!_load_texture(_background_texture, "assets/background.png"))
		return (false);
	if (!_load_texture(Gui::_piece_texture, "assets/white_piece.png"))
		return (false);
	if (!_load_texture(Gui::_tile_texture, "assets/marble_tiles.png"))
		return (false);
	if (!_load_texture(Gui::_board_texture, "assets/board.png"))
		return (false);
	if (!_load_texture(Gui::_logo_texture, "assets/logo.png"))
		return (false);
	if (!_load_texture(Gui::_playbutton_texture, "assets/playbutton.png"))
		return (false);
	if (!_load_texture(Gui::_button_texture, "assets/wbutton.png"))
		return (false);
	if (!_load_texture(Gui::_font_regular_texture, "assets/fontregular.png"))
		return (false);
	if (!_load_texture(Gui::_font_bold_texture, "assets/fontbold.png"))
		return (false);
	if (!_load_texture(Gui::_font_heavy_texture, "assets/fontheavy.png"))
		return (false);
	if (!_load_texture(Gui::texture("playbuttons"), "assets/playbuttons.png"))
		return (false);
	if (!_load_texture(Gui::texture("selectbuttons"), "assets/selectbuttons.png"))
		return (false);
	return (true);
}

void Gui::_run()
{
	mlx_key_hook(_mlx, key_hook, this);
	mlx_cursor_hook(_mlx, cursor_hook, this);
	mlx_mouse_hook(_mlx, mouse_hook, this);
	mlx_close_hook(_mlx, close_hook, this);
	mlx_resize_hook(_mlx, resize_hook, this);
	mlx_loop(_mlx);
}

void	Gui::apply_texture(mlx_image_t* image, mlx_texture_t* texture, Color const &color, size_t index, size_t gridsize, Rect dest)
{
	struct s_coord<uint8_t *> pixel;
	struct s_coord<unsigned int> xy;
	struct s_coord<float> scale;
	Rect tex(0, 0, texture->width / gridsize, texture->height / gridsize);

	if (dest.width == 0 || dest.height == 0
		|| dest.width + dest.x > image->width
		|| dest.height + dest.y > image->height)
		dest = Rect(0, 0, image->width, image->height);
	xy.x = index % gridsize;
	xy.y = index / gridsize;
	scale.x = static_cast<float>(tex.width) / dest.width;
	scale.y = static_cast<float>(tex.height) / dest.height;
	for (unsigned int j = 0; j < dest.height; j++)
	{
		for (unsigned int i = 0; i < dest.width; i++)
		{
			tex.x = static_cast<unsigned int>(i * scale.x) + tex.width * xy.x;
			tex.y = static_cast<unsigned int>(j * scale.y) + tex.height * xy.y;
			if (tex.x >= texture->width)
				tex.x = texture->width - 1;
			if (tex.y >= texture->height)
				tex.y = texture->height - 1;
			pixel.x = &texture->pixels[(tex.y * texture->width + tex.x) * texture->bytes_per_pixel];
			pixel.y = &image->pixels[((j + dest.y) * image->width + i + dest.x) * texture->bytes_per_pixel];
			//pixel.y = &image->pixels[(j * image->width + i) * texture->bytes_per_pixel];
			if (color != Color::white)
			{
				pixel.y[0] = (pixel.x[0] * color.r) / 255;
				pixel.y[1] = (pixel.x[1] * color.g) / 255;
				pixel.y[2] = (pixel.x[2] * color.b) / 255;
				pixel.y[3] = pixel.x[3];
				if (pixel.x[3] == 255)
					pixel.y[3] = color.a;
			}
			else
				memmove(pixel.y, pixel.x, texture->bytes_per_pixel);
		}
	}
}

void	Gui::_resize(int width, int height)
{
	Gui::_dimensions.width = width;
	Gui::_dimensions.height = height;
	mlx_resize_image(_background, width, height);
	Gui::apply_texture(_background, _background_texture);
	_board.resize();
	_menu.resize();
}

/*
** input hooks
*/
void 	Gui::resize_hook(int width, int height, void* param)
{
	Gui *gui = static_cast<Gui*>(param);

	gui->_resize(width, height);
}

void	Gui::mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	Gui *gui = static_cast<Gui*>(param);
	(void)gui;
	(void)mods;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
	{
	if (gui->_gamestate == GS_MENU)
	{
		if (gui->_menu.click())
		{
			gui->_gamestate = GS_BOARD;
			gui->_board.show(19);
			gui->_menu.hide();
		}
	}
	else if (gui->_gamestate == GS_BOARD)
		gui->_board.click();
	}
}

void	Gui::cursor_hook(double x, double y, void *param)
{
	Gui *gui = static_cast<Gui*>(param);

	Gui::_mouse = Rect((int)x, (int)y, 0, 0);
	if (gui->_gamestate == GS_MENU)
		gui->_menu.hover();
	else if (gui->_gamestate == GS_BOARD)
		gui->_board.hover();
}

void	Gui::close_hook(void *param)
{
	Gui *gui = static_cast<Gui*>(param);
	(void)gui;
	std::cout << "Window closed" << std::endl;
}

void Gui::key_hook(mlx_key_data_t keydata, void* param)
{
	Gui *gui = static_cast<Gui*>(param);
	(void)gui;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		if (gui->_gamestate == GS_BOARD)
		{
			gui->_gamestate = GS_MENU;
			gui->_board.hide();
			gui->_menu.show();
		}
		else if (gui->_gamestate == GS_MENU)
		{
			std::cout << "Escape key pressed" << std::endl;
			mlx_close_window(Gui::mlx());
		}
	}
}

/*
** static getters
*/
Rect const &Gui::dimensions()
{
	return (Gui::_dimensions);
}

mlx_t *Gui::mlx()
{
	return (Gui::_mlx);
}

Rect const &Gui::mouse()
{
	return (Gui::_mouse);
}

mlx_texture_t *&Gui::texture(std::string name)
{
	return (Gui::_textures[name]);
}

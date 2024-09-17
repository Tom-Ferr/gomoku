#include  <Gui.hpp>

/*
** static variables initialization
*/
mlx_t									*Gui::_mlx = nullptr;
std::map<std::string, mlx_texture_t *>	Gui::_textures;
Rect									Gui::_dimensions;
Rect									Gui::_mouse;

/*
** Constructors and assignment operator.
*/
Gui::Gui()
: _background(nullptr)
{
	_board = Board(this);
	if (_init())
		_run();
}

Gui::Gui(Gui const &other)
{
	*this = other;
}

Gui &Gui::operator=(Gui const &other)
{
	_background = other._background;
	_board = other._board;
	_menu = other._menu;
	_gamestate = other._gamestate;
	return *this;
}

/*
** deletes the textures and let minilibx do its job by removing everything else
*/
Gui::~Gui()
{
	for (std::map<std::string, mlx_texture_t*>::iterator it
			= _textures.begin(); it != _textures.end(); it++)
	{
		if (it->second)
			mlx_delete_texture(it->second);
	}
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
		std::cout << "Error: mlx_init failed. "
				<< mlx_strerror(mlx_errno) << std::endl;
		return (false);
	}
	if (!(_load_textures()))
		return (false);
	if (!(_load_background()))
		return (false);
	mlx_image_to_window(_mlx, _background, 0, 0);
	mlx_set_instance_depth(&_background->instances[0], 0);
	if (!_board.init())
		return false;
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
	apply_texture(_background, Gui::texture("background"));
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
	if (!_load_texture(texture("background"), "assets/background.png"))
		return (false);
	if (!_load_texture(Gui::texture("piece"), "assets/white_piece.png"))
		return (false);
	if (!_load_texture(Gui::texture("tile"), "assets/marble_tiles.png"))
		return (false);
	if (!_load_texture(Gui::texture("board"), "assets/board.png"))
		return (false);
	if (!_load_texture(Gui::texture("logo"), "assets/logo.png"))
		return (false);
	if (!_load_texture(Gui::texture("font_regular"), "assets/fontregular.png"))
		return (false);
	if (!_load_texture(Gui::texture("font_heavy"), "assets/fontheavy.png"))
		return (false);
	if (!_load_texture(Gui::texture("playbuttons"), "assets/playbuttons.png"))
		return (false);
	if (!_load_texture(Gui::texture("selectbuttons"),
						"assets/selectbuttons.png"))
		return (false);
	if (!_load_texture(Gui::texture("box"), "assets/boxbg.png"))
		return (false);
	return (true);
}

void Gui::_run()
{
	mlx_key_hook(_mlx, key_hook, this);
	mlx_cursor_hook(_mlx, cursor_hook, this);
	mlx_mouse_hook(_mlx, mouse_hook, this);
	mlx_resize_hook(_mlx, resize_hook, this);
	mlx_loop_hook(_mlx, board_loop_hook, this);
	mlx_loop(_mlx);
}

/*
** Applies a texture to an image, with optional color, grid index
** and destination position.
** Arguments:
** - image: the image to apply the texture to
** - texture: the texture to apply
** - color: the color to apply to the texture. Defaults to white.
** - index: the index of the texture in the grid. Defaults to 0.
** - gridsize: the size of the grid. Defaults to 1.
** - dest: the destination rectangle. Defaults to the whole image.
*/
void	Gui::apply_texture(mlx_image_t* image, mlx_texture_t* texture,
							Color const &color, size_t index,
							size_t gridsize, Rect dest)
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
			pixel.x = &texture->pixels[(tex.y * texture->width + tex.x)
										* texture->bytes_per_pixel];
			pixel.y = &image->pixels[((j + dest.y) * image->width + i + dest.x)
										* texture->bytes_per_pixel];
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

/*
** resize the Gui elements and propagates resizing to _board and _menu.
*/
void	Gui::_resize(int width, int height)
{
	Gui::_dimensions.width = width;
	Gui::_dimensions.height = height;
	mlx_resize_image(_background, width, height);
	Gui::apply_texture(_background, Gui::texture("background"));
	if (_gamestate == GS_BOARD)
		_board.resize();
	else
		_menu.resize();
}

/*
** input hooks
*/
void Gui::board_loop_hook(void *param)
{
	Gui *gui = static_cast<Gui*>(param);

	if (gui->_gamestate == GS_BOARD)
		gui->_board.loop();
}

void 	Gui::resize_hook(int32_t width, int32_t height, void* param)
{
	Gui *gui = static_cast<Gui*>(param);

	if (width > 300 && height > 200)
		gui->_resize(width, height);
}

void	Gui::mouse_hook(mouse_key_t button, action_t action,
						modifier_key_t mods, void *param)
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
			gui->_board.show(gui->_menu.opt_vs(),
								gui->_menu.opt_starting(),
								gui->_menu.opt_mode());
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

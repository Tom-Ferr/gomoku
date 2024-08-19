#include  <Gui.hpp>

mlx_t			*Gui::_mlx = nullptr;
mlx_texture_t	*Gui::_button_texture = nullptr;
mlx_texture_t	*Gui::_tile_texture = nullptr;
mlx_texture_t	*Gui::_board_texture = nullptr;
Rect			Gui::_dimensions;
Rect			Gui::_mouse;

Gui::Gui()
{
	if (_init())
		_run();
}

Gui::~Gui()
{
	if (Gui::_button_texture)
		mlx_delete_texture(Gui::_button_texture);
	if (Gui::_tile_texture)
		mlx_delete_texture(Gui::_tile_texture);
	if (Gui::_board_texture)
		mlx_delete_texture(Gui::_board_texture);
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
	_board.init();
	_board.reset(19);
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
	if (!_load_texture(Gui::_button_texture, "assets/white_piece.png"))
		return (false);
	if (!_load_texture(Gui::_tile_texture, "assets/marble_tiles.png"))
		return (false);
	if (!_load_texture(Gui::_board_texture, "assets/board.png"))
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

void	Gui::apply_texture(mlx_image_t* image, mlx_texture_t* texture, Color const &color)
{
	if (image == nullptr or texture == nullptr)
		return;
	uint8_t* pixelx;
	uint8_t* pixeli;
	uint32_t tex_x;
	uint32_t tex_y;
	float scalex = (float)texture->width / (float)image->width;
	float scaley = (float)texture->height / (float)image->height;

	for (uint32_t y = 0; y < image->height; y++)
	{
		for (uint32_t x = 0; x < image->width; x++)
		{
			tex_x = (uint32_t)(x * scalex);
			tex_y = (uint32_t)(y * scaley);
			if (tex_x >= texture->width)
				tex_x = texture->width - 1;
			if (tex_y >= texture->height)
				tex_y = texture->height - 1;
			pixelx = &texture->pixels[(tex_y * texture->width + tex_x) * texture->bytes_per_pixel];
			pixeli = &image->pixels[(y * image->width + x) * texture->bytes_per_pixel];
			if (color != Color::white)
			{
				pixeli[0] = (pixelx[0] * color.r) / 255;
				pixeli[1] = (pixelx[1] * color.g) / 255;
				pixeli[2] = (pixelx[2] * color.b) / 255;
				pixeli[3] = pixelx[3];
				if (pixelx[3] == 255)
					pixeli[3] = color.a;
			}
			else
				memmove(pixeli, pixelx, texture->bytes_per_pixel);
		}
	}
}

/*
** apply a subtexture to an image.
** size is understood to be 3 x 3.
*/
void Gui::apply_texture(mlx_image_t *image, mlx_texture_t *texture, size_t index)
{
	if (image == nullptr or texture == nullptr)
		return;
	uint8_t* pixelx;
	uint8_t* pixeli;
	uint32_t tex_x;
	uint32_t tex_y;
	uint32_t tex_width = texture->width / 3;
	uint32_t tex_height = texture->height / 3;
	uint32_t x = index % 3;
	uint32_t y = index / 3;
	float scalex = (float)tex_width / (float)image->width;
	float scaley = (float)tex_height / (float)image->height;

	for (uint32_t j = 0; j < image->height; j++)
	{
		for (uint32_t i = 0; i < image->width; i++)
		{
			tex_x = (uint32_t)(i * scalex) + tex_width * x;
			tex_y = (uint32_t)(j * scaley) + tex_height * y;
			if (tex_x >= texture->width)
				tex_x = texture->width - 1;
			if (tex_y >= texture->height)
				tex_y = texture->height - 1;
			pixelx = &texture->pixels[(tex_y * texture->width + tex_x) * texture->bytes_per_pixel];
			pixeli = &image->pixels[(j * image->width + i) * texture->bytes_per_pixel];
			memmove(pixeli, pixelx, texture->bytes_per_pixel);
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
}

/*
** input hooks
*/
void 	Gui::resize_hook(int width, int height, void* param)
{
	Gui *gui = static_cast<Gui*>(param);

	std::cout << "Window resized to " << width << "x" << height << std::endl;
	gui->_resize(width, height);
}

void	Gui::mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	Gui *gui = static_cast<Gui*>(param);
	(void)gui;
	(void)mods;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
		std::cout << "Mouse left button pressed" << std::endl;
}

void	Gui::cursor_hook(double x, double y, void *param)
{
	Gui *gui = static_cast<Gui*>(param);

	Gui::_mouse = Rect((int)x, (int)y, 0, 0);
	gui->_board.hover();
	std::cout << "Cursor position: " << x << ", " << y << std::endl;
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
		std::cout << "Escape key pressed" << std::endl;
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

#include <Text.hpp>
#include <Gui.hpp>



Text::Text()
:_image(nullptr), _bold(false) {}

Text::Text(std::string const &str, bool bold)
:_image(nullptr), _bold(bold)
{
	_text = str;
	_init();
}

Text::Text(Text const &other)
:_image(other._image), _dimensions(other._dimensions), _text(other._text)
{
	*this = other;
}

Text::~Text() {}

Text &Text::operator=(Text const &other)
{
	_image = other._image;
	_bold = other._bold;
	_dimensions = other._dimensions;
	_text = other._text;
	return (*this);
}

Text &Text::operator=(std::string const &str)
{
	_text = str;
	_init();
	return (*this);
}

Rect const &Text::dimensions()
{
	return (_dimensions);
}

void Text::_init()
{
	mlx_texture_t *font = Gui::_font_regular_texture;

	if (_bold)
		font = Gui::_font_heavy_texture;
	Rect tex = Rect(0, 0, font->width  / FONT_GRIDSIZE,
					font->height / FONT_GRIDSIZE);
	_dimensions = Rect(0, 0, tex.width * _text.size(), tex.height);
	if (!_image)
	{
		_image = mlx_new_image(Gui::mlx(), _dimensions.width, _dimensions.height);
		if (!_image)
			return ;
		mlx_image_to_window(Gui::mlx(), _image, 0, 0);
	}
	resize(_dimensions.height);
}

void Text::resize(Rect const &dimensions)
{
	_dimensions.x = dimensions.x;
	_dimensions.y = dimensions.y;
	resize(dimensions.height);
}

void Text::resize(size_t height)
{
	mlx_texture_t *font = Gui::_font_regular_texture;

	if (!_image)
		return ;
	if (_bold)
		font = Gui::_font_heavy_texture;
	float ratio = static_cast<float>(
					font->width  / FONT_GRIDSIZE)
					/ (font->height / FONT_GRIDSIZE);
	size_t tex_width = ratio * height;

	_dimensions.width = tex_width * _text.size();
	_dimensions.height = height;
	mlx_resize_image(_image, _dimensions.width, _dimensions.height);
	if (!_image)
		return ;
	for (size_t i = 0; i < _text.size(); i++)
	{
		Rect r = Rect(tex_width * i, 0, tex_width, _dimensions.height);
		Gui::apply_texture(_image, font,
						Color::white, _text[i] - 32,
						FONT_GRIDSIZE, r);
	}
	_image->instances[0].x = _dimensions.x;
	_image->instances[0].y = _dimensions.y;
}

std::string &Text::text()
{
	return _text;
}

void Text::show()
{
	if (_image)
		_image->instances[0].enabled = true;
}

void Text::hide()
{
	if (_image)
		_image->instances[0].enabled = false;
}

#include <Text.hpp>
#include <Gui.hpp>



Text::Text()
:_image(nullptr) {}

Text::Text(std::string const &str)
:_image(nullptr)
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
	Rect tex = Rect(0, 0, Gui::_font_regular_texture->width  / FONT_GRIDSIZE,
					Gui::_font_regular_texture->height / FONT_GRIDSIZE);

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

void Text::resize(Rect &dimensions)
{
	_dimensions.x = dimensions.x;
	_dimensions.y = dimensions.y;
	resize(dimensions.height);
}

void Text::resize(size_t height)
{
	if (!_image)
		return ;
	float ratio = static_cast<float>(
					Gui::_font_regular_texture->width  / FONT_GRIDSIZE)
					/ (Gui::_font_regular_texture->height / FONT_GRIDSIZE);
	size_t tex_width = ratio * height;

	_dimensions.width = tex_width * _text.size();
	_dimensions.height = height;
	mlx_resize_image(_image, _dimensions.width, _dimensions.height);
	if (!_image)
		return ;
	for (size_t i = 0; i < _text.size(); i++)
	{
		Rect r = Rect(tex_width * i, 0, tex_width, _dimensions.height);
		std::cout << "Char " << _text[i] << " at " << r << std::endl;
		Gui::apply_texture(_image, Gui::_font_regular_texture,
						Color::white, _text[i] - 32,
						FONT_GRIDSIZE, r);
	}
	_image->instances[0].x = _dimensions.x;
	_image->instances[0].y = _dimensions.y;
}
#include <Text.hpp>
#include <Gui.hpp>
#include <algorithm>

Text::Text()
:_image(nullptr), _bold(false), _instance(-1) {}

Text::Text(std::string const &str, bool bold)
:_image(nullptr), _bold(bold), _instance(-1)
{
	_text = str;
	_init();
}

Text::Text(Text const &other)
:_image(other._image), _dimensions(other._dimensions),
_text(other._text)
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
	_instance = other._instance;
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
	mlx_texture_t *font = Gui::texture("font_regular");

	if (_bold)
		font = Gui::texture("font_heavy");
	Rect tex = Rect(0, 0, font->width  / FONT_GRIDSIZE,
					font->height / FONT_GRIDSIZE);
	_dimensions = Rect(0, 0, tex.width * _text.size(), tex.height);
	if (_text.size() > LINE_WIDTH)
	{
		_dimensions.width = tex.width * LINE_WIDTH;
		_dimensions.height = tex.height * ((_text.size() / LINE_WIDTH) + 1);
	}
	if (!_image)
	{
		_image = mlx_new_image(Gui::mlx(),
						 _dimensions.width, _dimensions.height);
		if (!_image)
			return ;
		_instance = mlx_image_to_window(Gui::mlx(), _image, 0, 0);
	}
}

bool Text::init()
{
	_init();
	if (!_image || _instance == -1)
		return false;
	return true;
}

void Text::resize(Rect const &dimensions)
{
	float ratio;
	size_t max_total_height;

	if (!_image || _instance == -1)
		return ;
	_dimensions.x = dimensions.x;
	_dimensions.y = dimensions.y;
	ratio = static_cast<float>(
			Gui::texture("font_regular")->height / FONT_GRIDSIZE)
			/ (Gui::texture("font_regular")->width / FONT_GRIDSIZE);
	if (_text.size() > LINE_WIDTH)
	{
		max_total_height = Gui::dimensions().height / 3;
		max_total_height /= ((_text.size() / LINE_WIDTH) + 1);
		max_total_height = std::min(max_total_height,
				static_cast<size_t>((dimensions.width / LINE_WIDTH) * ratio));
		resize(max_total_height);
	}
	else
		resize(dimensions.height);
}

void Text::resize(size_t height)
{
	mlx_texture_t *font = Gui::texture("font_regular");

	if (!_image || _instance == -1)
		return ;
	if (_bold)
		font = Gui::texture("font_heavy");
	float ratio = static_cast<float>(
					font->width  / FONT_GRIDSIZE)
					/ (font->height / FONT_GRIDSIZE);
	size_t tex_width = ratio * height;

	_dimensions.width = tex_width * _text.size();
	_dimensions.height = height;
	if (_text.size() > LINE_WIDTH)
	{
		_dimensions.width = tex_width * LINE_WIDTH;
		_dimensions.height = height * ((_text.size() / LINE_WIDTH));
	}
	mlx_resize_image(_image, _dimensions.width, _dimensions.height);
	if (!_image)
		return ;
	for (size_t i = 0; i < _text.size(); i++)
	{
		size_t x = i % LINE_WIDTH;
		size_t y = i / LINE_WIDTH;
		Rect r = Rect(tex_width * x, height * y, tex_width, height);
		Gui::apply_texture(_image, font,
						Color::white, _text[i] - 32,
						FONT_GRIDSIZE, r);
	}
	_image->instances[0].x = _dimensions.x;
	_image->instances[0].y = _dimensions.y;
}

void Text::center(size_t center)
{
	if (!_image || _instance == -1)
		return ;
	_dimensions.x = center - (_dimensions.width / 2);
	_image->instances[0].x = _dimensions.x;
}

void Text::center(size_t x, size_t y)
{
	if (!_image || _instance == -1)
		return ;
	_dimensions.x = x - (_dimensions.width / 2);
	_dimensions.y = y - (_dimensions.height / 2);
	_image->instances[0].x = _dimensions.x;
	_image->instances[0].y = _dimensions.y;
}

void Text::depth(size_t depth)
{
	if (!_image || _instance == -1)
		return ;
	mlx_set_instance_depth(&_image->instances[0], depth);
}

std::string &Text::text()
{
	return _text;
}

void Text::show()
{
	if (_image && _instance != -1)
		_image->instances[0].enabled = true;
}

void Text::hide()
{
	if (_image && _instance != -1)
		_image->instances[0].enabled = false;
}

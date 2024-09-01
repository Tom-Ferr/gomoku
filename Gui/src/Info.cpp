#include <Info.hpp>
#include <Gui.hpp>
# include <Board.hpp>

mlx_image_t			*Info::_piece_images[2] = {nullptr};

Info::Info()
: _image(PT_WHITE), _is_image(false)
{}

Info::Info(std::string const &str, std::string const &description)
:_image(PT_WHITE), _is_image(false)
{
	_text = Text(str + ": ", true);
	_description = description;
	_init();
}

Info::Info(std::string const &str, t_piecetype image)
:_image(PT_WHITE), _is_image(true)
{
	_image = image;
	_text = Text(str + ": ", true);
	_init();
}

Info::Info(Info const &other)
{
	*this = other;
}

Info::~Info() {}

Info &Info::operator=(Info const &other)
{
	_dimensions = other._dimensions;
	_text = other._text;
	_image = other._image;
	_is_image = other._is_image;
	_description = other._description;
	_buttons[PT_WHITE] = other._buttons[PT_WHITE];
	_buttons[PT_BLACK] = other._buttons[PT_BLACK];
	return (*this);
}

Info &Info::operator=(std::string const &description)
{
	_description = description;
	if (_is_image)
	{
		Board::piece_image(_image)->instances[_buttons[_image]].enabled = false;
		_is_image = false;
	}
	return (*this);
}

Info &Info::operator=(t_piecetype image)
{
	_image = image;
	_is_image = true;
	return (*this);
}

Rect const &Info::dimensions()
{
	return (_dimensions);
}

void Info::_init()
{
	_init_images();
	resize(Rect(300, 350, 0, 30));
}

void Info::_init_images()
{
	if (!(Info::_piece_images[0]))
	{
		for (int i = 0; i < 2; i++)
			Info::_piece_images[i] = mlx_new_image(Gui::mlx(), BUTTON_SIZE, BUTTON_SIZE);
	}
	for (int i = 0; i < 2; i++)
	{
		_buttons[i] = mlx_image_to_window(Gui::mlx(), Info::_piece_images[i], 0, 0);
		Info::_piece_images[i]->instances[_buttons[i]].enabled = false;
	}
	if (_is_image)
		Info::_piece_images[_image]->instances[_buttons[_image]].enabled = true;
}

void Info::resize(Rect const &dimensions)
{
	Rect text;

	text = Rect(dimensions.x, dimensions.y + 4, 0, dimensions.height);
	_text.resize(text);
	_description.resize(Rect(_text.dimensions().x + _text.dimensions().width, _text.dimensions().y, _text.dimensions().width, _text.dimensions().height));
	text.width =_text.dimensions().width + _description.dimensions().width;
	_dimensions = Rect(dimensions.x, dimensions.y, text.width + (dimensions.height) * 1.5, dimensions.height);
	if (_is_image)
	{
		if (dimensions.height != Info::_piece_images[_image]->height
			&& dimensions.width != Info::_piece_images[_image]->width)
		{
			mlx_resize_image(Info::_piece_images[PT_BLACK], dimensions.height, dimensions.height);
			Gui::apply_texture(Info::_piece_images[PT_BLACK], Gui::texture("piece"), Color::black);
			mlx_resize_image(Info::_piece_images[PT_WHITE], dimensions.height, dimensions.height);
			Gui::apply_texture(Info::_piece_images[PT_WHITE], Gui::texture("piece"), Color::white);
		}
		Info::_piece_images[_image]->instances[_buttons[_image]].x = _text.dimensions().x + _text.dimensions().width;
		Info::_piece_images[_image]->instances[_buttons[_image]].y = dimensions.y;
	}
}

void Info::hide()
{
	_text.hide();
	Info::_piece_images[PT_WHITE]->instances[_buttons[PT_WHITE]].enabled = false;
	Info::_piece_images[PT_BLACK]->instances[_buttons[PT_BLACK]].enabled = false;
	_description.hide();
}

void Info::show()
{
	_text.show();
	if (_is_image)
		Info::_piece_images[_image]->instances[_buttons[_image]].enabled = true;
	else
		_description.show();
}

#include <BoardStatusBar.hpp>
#include <Gui.hpp>
# include <Board.hpp>

/*
** Constructors and Destructor.
** Canonical Form.
*/
BoardStatusBar::BoardStatusBar()
: _background(nullptr), _hint_button(nullptr), _hint_button_hover(nullptr), _vs_human(false)
{ }

BoardStatusBar::BoardStatusBar(BoardStatusBar const &other)
{
	*this = other;
}

BoardStatusBar::~BoardStatusBar() {}

BoardStatusBar &BoardStatusBar::operator=(BoardStatusBar const &other)
{
	_dimensions = other._dimensions;
	_background = other._background;
	_hint_button = other._hint_button;
	_hint_button_hover = other._hint_button_hover;
	_hint_dimensions = other._hint_dimensions;
	_vs_human = other._vs_human;
	return (*this);
}

Rect const &BoardStatusBar::dimensions()
{
	return (_dimensions);
}

/*
** sets initial values for the status bar
*/
bool BoardStatusBar::init()
{
	_background = mlx_new_image(Gui::mlx(), 1, 1);
	_hint_button = mlx_new_image(Gui::mlx(),
							Gui::texture("hintbutton")->height,
							Gui::texture("hintbutton")->width);
	_hint_button_hover = mlx_new_image(Gui::mlx(),
							Gui::texture("hintbutton")->height,
							Gui::texture("hintbutton")->width);
	if (!_background || !_hint_button || !_hint_button_hover)
		return false;
	mlx_put_pixel(_background, 0, 0, 0x00000077);
	if ((mlx_image_to_window(
			Gui::mlx(), _background, _dimensions.x, _dimensions.y) == -1)
			|| (mlx_image_to_window(Gui::mlx(), _hint_button, 0, 0) == -1)
			|| (mlx_image_to_window(Gui::mlx(), _hint_button_hover, 0, 0) == -1))
		return false;
	std::cout << "initialized Statusbar" << std::endl;
	_vs = Info("Vs", "AI");
	_player1 = Info("Player1", PT_BLACK);
	_player2 = Info("Player2", PT_WHITE);
	_turn = Info("Turn", PT_BLACK);
	_last_time = Info("Last Move", "0");
	_avg_time = Info("Avg Time", "0");
	_captures = Text("Captures");
	_player1_captures = Info("Player1", "0");
	_player2_captures = Info("Player2", "0");

	if (!_vs._is_instanciated() || !_player1._is_instanciated()
			|| !_player2._is_instanciated() || !_turn._is_instanciated()
			|| !_last_time._is_instanciated() || !_avg_time._is_instanciated()
			|| !_captures.init() || !_player1_captures._is_instanciated()
			|| !_player2_captures._is_instanciated())
		return false;
	hide();
	resize();
	return true;
}

void BoardStatusBar::resize()
{
	size_t text_height;

	_dimensions = Rect::subrect(Gui::dimensions(), 1., .05, 2);
	text_height = _dimensions.height * .6;
	_dimensions = Rect::subrect(Gui::dimensions(), .2, 1, 0);
	_dimensions.x = Gui::dimensions().width - _dimensions.width;
	if (!(static_cast<float>(Gui::dimensions().width)
			/ Gui::dimensions().height > 1.5))
		_dimensions.x = Gui::dimensions().width;

	_resize_vertical(text_height);
	if (_background)
	{
		mlx_resize_image(_background, _dimensions.width, _dimensions.height);
		_background->instances[0].x = _dimensions.x;
		_background->instances[0].y = _dimensions.y;
	}
	if (_hint_button && _hint_button_hover)
	{
		size_t hint_size = text_height * 2;
		_hint_dimensions = Rect(_dimensions.x + _dimensions.width - hint_size - 20,
								_dimensions.y + _dimensions.height - hint_size - 10,
								hint_size, hint_size);
		mlx_resize_image(_hint_button, _hint_dimensions.height, _hint_dimensions.height);
		mlx_resize_image(_hint_button_hover, _hint_dimensions.height, _hint_dimensions.height);
		Gui::apply_texture(_hint_button, Gui::texture("hintbutton"),
													 Color::white);
		Gui::apply_texture(_hint_button_hover, Gui::texture("hintbutton"),
													Color(255, 255, 0 , 255));
		_hint_button->instances[0].x = _hint_dimensions.x;
		_hint_button->instances[0].y = _hint_dimensions.y;
		_hint_button_hover->instances[0].x = _hint_dimensions.x;
		_hint_button_hover->instances[0].y = _hint_dimensions.y;
	}
}

void BoardStatusBar::_resize_vertical(size_t text_height)
{
	float spacing = 1.1;

	_vs.resize(
		Rect(_dimensions.x + 5,
			 _dimensions.y + 1.05, 0, text_height));
	_player1.resize(
		Rect(_dimensions.x + 5,
			 _dimensions.y + 1.05 + text_height * spacing,
			 0, text_height));
	_player2.resize(
		Rect(_dimensions.x + 5,
			 _dimensions.y + 1.05 + text_height * 2 * spacing,
			 0, text_height));
	_turn.resize(
		Rect(_dimensions.x + 5,
			 _dimensions.y + 1.05 + text_height * 3 * spacing,
			 0, text_height));
	_last_time.resize(
		Rect(_dimensions.x + 5,
			 _dimensions.y + 1.05 + text_height * 4 * spacing,
			 0, text_height));
	_avg_time.resize(
		Rect(_dimensions.x + 5,
			 _dimensions.y + 1.05 + text_height * 5 * spacing,
			 0, text_height));
	_captures.resize(
		Rect(_dimensions.x + 5,
			 _dimensions.y + 1.05 + text_height * 6.2 * spacing,
			 0, text_height));
	_player1_captures.resize(
		Rect(_dimensions.x + 15,
			 _dimensions.y + 1.05 + text_height * 7 * spacing,
			 0, text_height));
	_player2_captures.resize(
		Rect(_dimensions.x + 15,
			 _dimensions.y + 1.05 + text_height * 8 * spacing,
			 0, text_height));
	_dimensions.height = _player2_captures.dimensions().y
							+ _player2_captures.dimensions().height + 5;
}

void BoardStatusBar::hover()
{
	if (!_vs_human)
		return ;
	if (_hint_dimensions.contains(Gui::mouse().x, Gui::mouse().y))
	{
		_hint_button_hover->enabled = true;
		_hint_button->enabled = false;
	}
	else
	{
		_hint_button_hover->enabled = false;
		_hint_button->enabled = true;
	}
}

/*
** if true, the hint button was clicked
*/
bool BoardStatusBar::click()
{
	if (!_vs_human)
		return false;
	if (_hint_dimensions.contains(Gui::mouse().x, Gui::mouse().y))
		return true;
	return false;
}

void BoardStatusBar::_resize_horizontal(size_t text_height)
{
	float ratio;
	float spacing;
	float current_width = _avg_time.dimensions().width
							+ _avg_time.dimensions().x;
	float current_height = _avg_time.dimensions().height;

	ratio = current_width / current_height;
	if ((ratio * text_height) > _dimensions.width)
		text_height = static_cast<float>(_dimensions.width) / ratio;
	_vs.resize(Rect(_dimensions.x + 20, _dimensions.y + 1.05, 0, text_height));
	spacing = text_height / 2;
	_player1.resize(
		Rect(_vs.dimensions().x + _vs.dimensions().width + spacing,
			 _dimensions.y + 1.05, 0, text_height));
	_player2.resize(
		Rect(_player1.dimensions().x + _player1.dimensions().width + spacing,
			 _dimensions.y + 1.05, 0, text_height));
	_turn.resize(
		Rect(_player2.dimensions().x + _player2.dimensions().width + spacing,
			 _dimensions.y + 1.05, 0, text_height));
	_last_time.resize(
		Rect(_turn.dimensions().x + _turn.dimensions().width + spacing,
			 _dimensions.y + 1.05, 0, text_height));
	_avg_time.resize(
		Rect(_last_time.dimensions().x
			 + _last_time.dimensions().width + spacing,
			 _dimensions.y + 1.05, 0, text_height));
	_captures.resize(
		Rect(0, Gui::dimensions().height + 100, 0, text_height));
	_player1_captures.resize(
		Rect(0, Gui::dimensions().height + 100, 0, text_height));
	_player2_captures.resize(
		Rect(0, Gui::dimensions().height + 100, 0, text_height));

}

void BoardStatusBar::hide()
{
	_background->enabled = false;
	_vs.hide();
	_player1.hide();
	_player2.hide();
	_turn.hide();
	_last_time.hide();
	_avg_time.hide();
	_player1_captures.hide();
	_player2_captures.hide();
	_captures.hide();
	_hint_button_hover->enabled = false;
	_hint_button->enabled = false;
}

void BoardStatusBar::show()
{
	_background->enabled = true;
	_vs.show();
	_player1.show();
	_player2.show();
	_turn.show();
	_last_time.show();
	_avg_time.show();
	_player1_captures.show();
	_player2_captures.show();
	_captures.show();
	if (_vs_human)
	{
		_hint_button_hover->enabled = false;
		_hint_button->enabled = true;
	}
}

void BoardStatusBar::set_vs(bool ai)
{
	if (ai)
	{
		_vs_human = false;
		_vs = "AI";
		_hint_button_hover->enabled = false;
		_hint_button->enabled = false;
	}
	else
	{
		_vs_human = true;
		_vs = "Player 2";
		_hint_button_hover->enabled = false;
		_hint_button->enabled = true;
	}
}

void BoardStatusBar::set_player1(bool black)
{
	if (black)
		_player1 = PT_BLACK;
	else
		_player1 = PT_WHITE;
}

void BoardStatusBar::set_player2(bool black, bool ai)
{
	if (ai)
		_player2.set_title("AI: ");
	else
		_player2.set_title("Player 2: ");
	if (black)
		_player2 = PT_BLACK;
	else
		_player2 = PT_WHITE;
}

void BoardStatusBar::set_turn(bool turn)
{
	if (turn)
		_turn = PT_BLACK;
	else
		_turn = PT_WHITE;
}

void BoardStatusBar::set_last_time(std::string const &last_time)
{
	_last_time = last_time;
}

void BoardStatusBar::set_avg_time(std::string const &avg_time)
{
	_avg_time = avg_time;
}

void BoardStatusBar::set_player1_captures(size_t captures)
{
	_player1_captures = std::to_string(captures);
}

void BoardStatusBar::set_player2_captures(size_t captures)
{
	_player2_captures = std::to_string(captures);
}


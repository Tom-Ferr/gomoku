#include <BoardStatusBar.hpp>
#include <Gui.hpp>
# include <Board.hpp>

/*
** Constructors and Destructor.
** Canonical Form.
*/
BoardStatusBar::BoardStatusBar()
: _background(nullptr)
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
	return (*this);
}

Rect const &BoardStatusBar::dimensions()
{
	return (_dimensions);
}

/*
** sets initial values for the status bar
*/
void BoardStatusBar::init()
{
	_background = mlx_new_image(Gui::mlx(), 1, 1);
	mlx_put_pixel(_background, 0, 0, 0x00000077);
	mlx_image_to_window(Gui::mlx(), _background, _dimensions.x, _dimensions.y);
	_vs = Info("Vs", "AI");
	_player1 = Info("Player1", PT_BLACK);
	_player2 = Info("Player2", PT_WHITE);
	_turn = Info("Turn", PT_BLACK);
	_last_time = Info("Last Move", "0");
	_avg_time = Info("Avg Time", "0");
	_captures = Text("Captures");
	_player1_captures = Info("Player1", "0");
	_player2_captures = Info("Player2", "0");
	hide();
	resize();
}

void BoardStatusBar::resize()
{
	bool vertical = false;
	size_t text_height;

	_dimensions = Rect::subrect(Gui::dimensions(), 1., .05, 2);
	text_height = _dimensions.height * .6;
	if (static_cast<float>(Gui::dimensions().width)
			/ Gui::dimensions().height > 1.5)
	{
		_dimensions = Rect::subrect(Gui::dimensions(), .2, 1, 0);
		_dimensions.x = Gui::dimensions().width - _dimensions.width;
		vertical = true;
	}
	if (vertical)
		_resize_vertical(text_height);
	else
		_resize_horizontal(text_height);
	if (_background)
	{
		mlx_resize_image(_background, _dimensions.width, _dimensions.height);
		_background->instances[0].x = _dimensions.x;
		_background->instances[0].y = _dimensions.y;
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
	std::cout << "Text Height: " << text_height << std::endl;
	std::cout << "Current Width: " << current_width << std::endl;
	std::cout << "Current Height: " << current_height << std::endl;
	std::cout << "Ratio: " << ratio << std::endl;
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
		Rect(Gui::dimensions().height + 100, 0, 0, text_height));
	_player1_captures.resize(
		Rect(Gui::dimensions().height + 100, 0, 0, text_height));
	_player2_captures.resize(
		Rect(Gui::dimensions().height + 100, 0, 0, text_height));
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

}

void BoardStatusBar::set_vs(bool ai)
{
	if (ai)
		_vs = "AI";
	else
		_vs = "Player 2";
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

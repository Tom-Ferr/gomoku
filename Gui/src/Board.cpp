#include <Board.hpp>
# include <Gui.hpp>

mlx_image_t			*Board::_tile_images[9] = {nullptr};
mlx_image_t			*Board::_piece_images[5] = {nullptr};
Rect				Board::_dimensions;
size_t				Board::_sqrt = DEFAULT_BOARD;
size_t				Board::_size = DEFAULT_BOARD * DEFAULT_BOARD;

Board::Board()
: _background(nullptr)
{ }

/*
** initializes the board, tiles, status bar, endgame and mode
** returns false if something fails, so it can gracefully exit
*/
bool Board::init()
{
	_background = mlx_new_image(Gui::mlx(), BACKGROUND_SIZE, BACKGROUND_SIZE);
	Board::_dimensions = Rect::subrect(Gui::dimensions(), .9);
	Gui::apply_texture(_background, Gui::_board_texture);
	mlx_image_to_window(Gui::mlx(), _background, 0, 0);
	mlx_set_instance_depth(&_background->instances[0], 1);
	_init();
	_statusbar.init();
	_endgame.init();
	_mode.init();
	disable();
	hide();
	return (true);
}

bool Board::show()
{
	_background->enabled = true;
	_visible=true;
	_hovered_tile = nullptr;
	_game = Game(_sqrt);
	for (std::vector<Tile>::iterator it = _tiles.begin(); it != _tiles.end(); ++it)
	{
		it->clear();
		it->show();
	}
	enable();
	_statusbar.show();
	//_endgame.show("AI");
	//_mode.show("Swap", false);
	resize();
	return (true);
}

Board::Board(Board const &other)
{
	*this = other;
}

Board::~Board()
{
//	if (_background)
//		mlx_delete_image(Gui::mlx(), _background);
	_background = nullptr;
}

Board &Board::operator=(Board const &other)
{
	(void)other;
//	_sqrt = other._sqrt;
//	_size = other._size;
//	_background = other._background;
//	_dimensions = other._dimensions;
//	_tiles = other._tiles;
	//_tiles = other._tiles;
	//_window = other._window;
	//_dimensions = other._dimensions;
	//_position = other._position;
	//_tiles = other._tiles;
	//_tiles_dimensions = other._tiles_dimensions;
	//_hovered_tile = other._hovered_tile;
	//_enabled = other._enabled;
	//_game = other._game;
	return *this;
}
bool Board::visible()
{
	return _visible;
}

void Board::hide()
{
	_background->enabled = false;
	_visible=false;
	_statusbar.hide();
	_endgame.hide();
	_mode.hide();
	for (std::vector<Tile>::iterator it = _tiles.begin(); it != _tiles.end(); ++it)
	{
		it->clear();
		it->hide();
	}
	disable();
}

bool Board::_init()
{
	for (size_t i = 0; i < 9; i++)
	{
		Board::_tile_images[i] = mlx_new_image(Gui::mlx(), TILE_SIZE, TILE_SIZE);
		Gui::apply_texture(Board::_tile_images[i], Gui::_tile_texture, Color::white, i, 3);
	}
	for (size_t i = 0; i < 5; i++)
		Board::_piece_images[i] = mlx_new_image(Gui::mlx(), BUTTON_SIZE, BUTTON_SIZE);
	for (size_t i = 0; i < Board::_size; i++)
		_tiles.push_back(Tile(i));
	resize();
	return true;
}

void Board::resize()
{
	if (!enabled())
		return;
	_dimensions = Rect::subrect(Gui::dimensions(), .9);
	mlx_resize_image(_background, _dimensions.width, _dimensions.height);
	Gui::apply_texture(_background, Gui::_board_texture);
	_background->instances[0].x = _dimensions.x;
	_background->instances[0].y = _dimensions.y;
	_tile_dimensions = Rect::subrect(_dimensions, 1.0 / sqrt());
	for (size_t i = 0; i < 9; i++)
	{
		mlx_resize_image(Board::_tile_images[i], _tile_dimensions.width, _tile_dimensions.height);
		Gui::apply_texture(Board::_tile_images[i], Gui::_tile_texture, Color::white, i, 3);
	}
	for (size_t i = 0; i < 5; i++)
		mlx_resize_image(Board::_piece_images[i], _tile_dimensions.width, _tile_dimensions.height);
	Gui::apply_texture(Board::_piece_images[PT_BLACK], Gui::_piece_texture, Color::black);
	Gui::apply_texture(Board::_piece_images[PT_WHITE], Gui::_piece_texture, Color::white);
	Gui::apply_texture(Board::_piece_images[PT_HINT], Gui::_piece_texture, Color::yellow);
	Gui::apply_texture(Board::_piece_images[PT_BLACKHOVER], Gui::_piece_texture, Color::black_alpha);
	Gui::apply_texture(Board::_piece_images[PT_WHITEHOVER], Gui::_piece_texture, Color::white_alpha);
	for (std::vector<Tile>::iterator it = _tiles.begin(); it != _tiles.end(); ++it)
	{
		it->resize(_tile_dimensions);
	}
	_statusbar.resize();
	_endgame.resize();
	_mode.resize();
}

size_t Board::size()
{
	return Board::_size;
}

size_t Board::sqrt()
{
	return Board::_sqrt;
}

Rect const &Board::dimensions()
{
	return Board::_dimensions;
}

bool Board::hover()
{
	Tile				*hovered;
	int					tile;
	_enabled = true;

	if (enabled() && Board::dimensions().contains(Gui::_mouse.x, Gui::_mouse.y))
	{
		tile = get_hovered_tile();
		if (tile == -1)
		{
			if (_hovered_tile)
			{
				_hovered_tile->hover(false, true);
				_hovered_tile = nullptr;
			}
			return false;
		}
		if (_game.is_double_free_three(tile))
			return false;
		hovered = &_tiles[tile];
		if (hovered == _hovered_tile)
			return true;
		if (_hovered_tile)
			_hovered_tile->hover(false, true);
		_hovered_tile = hovered;
		_hovered_tile->hover(true, true);
		return true;
	}
	else if (_hovered_tile)
	{
		_hovered_tile->hover(false, true);
		_hovered_tile = NULL;
	}
	return false;
}

void Board::_remove_captures()
{
	std::vector<int> &captures = _game.captures();
	for (std::vector<int>::iterator it = captures.begin(); it != captures.end(); ++it)
		_tiles[*it].clear();
}

bool Board::click()
{
	if (enabled() && _hovered_tile)
	{
		if (_game.is_double_free_three(_hovered_tile->pos()))
			return false;
		if (_hovered_tile->click(true))
		{
			disable();
			_game.check_capture(_hovered_tile->pos(), false);
			_remove_captures();
			_game.board().applymove(_hovered_tile->pos());
			_hovered_tile = nullptr;
			_game.update_freechecker();
			if (_game.step(false))
			{
				_tiles[_game.move()].hover(true, false);
				_tiles[_game.move()].click(false);
				_remove_captures();
				enable();
			}
		}
	}
	return true;
}

bool Board::enabled()
{
	return _enabled;
}

void Board::enable()
{
	_enabled = true;
}

void Board::disable()
{
	_enabled = false;
}

Rect	&Board::get_tile_dimensions()
{
	return _tile_dimensions;
}

/*
** returns -1 if no tile is hovered.
*/
int Board::get_hovered_tile()
{
	size_t x = (Board::dimensions().x + Board::dimensions().width - Gui::_mouse.x) / get_tile_dimensions().width;
	size_t y = (Board::dimensions().y + Board::dimensions().height - Gui::_mouse.y) / get_tile_dimensions().height;
	if (x >= sqrt() || y >= sqrt())
		return -1;
    return (y * sqrt() + x);
}

mlx_image_t *Board::piece_image(t_piecetype piecetype)
{
	return Board::_piece_images[piecetype];
}

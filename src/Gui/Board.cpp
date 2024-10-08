#include <Board.hpp>
#include <Gui.hpp>

mlx_image_t			*Board::_tile_images[9] = {nullptr};
mlx_image_t			*Board::_piece_images[5] = {nullptr};
Rect				Board::_dimensions;
size_t				Board::_sqrt = DEFAULT_BOARD;
size_t				Board::_size = DEFAULT_BOARD * DEFAULT_BOARD;

Board::Board(Gui *gui)
: _background(nullptr), _gui(gui), _hovered_tile(nullptr),
_enabled(false), _gameover(false), _hinted_tile(nullptr)
{ }

/*
** initializes the board, tiles, status bar, endgame and mode
** returns false if something fails, so it can gracefully exit
*/
bool Board::init()
{
	_background = mlx_new_image(Gui::mlx(), BACKGROUND_SIZE, BACKGROUND_SIZE);
	if (!_background)
		return false;
	Board::_dimensions = Rect::subrect(Gui::dimensions(), .9);
	Gui::apply_texture(_background, Gui::texture("board"));
	if (mlx_image_to_window(Gui::mlx(), _background, 0, 0) == -1)
		return false;
	mlx_set_instance_depth(&_background->instances[0], 1);
	if (!_init_tiles())
		return false;
	if (!_statusbar.init())
		return false;
	if (!_endgame.init())
		return false;
	if (!_mode.init())
		return false;
	disable();
	hide();
	return true;
}

bool Board::show(t_vs vs, t_startingplayer starting, t_gamemode mode)
{
	_gameover = false;
	_loop_count = 0;
	_background->enabled = true;
	_visible=true;
	_hovered_tile = nullptr;
	_hinted_tile = nullptr;
	_game = Game(_sqrt, vs, starting, mode);
	for (std::vector<Tile>::iterator it = _tiles.begin(); it != _tiles.end(); ++it)
	{
		it->clear();
		it->show();
	}
	enable();
	_statusbar.show();
	update_statusbar();
	resize();
	return (true);
}

Board::Board(Board const &other)
{
	*this = other;
}

Board::~Board()
{ }

Board &Board::operator=(Board const &other)
{
	_background = other._background;
	_gui = other._gui;
	_tiles = other._tiles;
	_hovered_tile = other._hovered_tile;
	_hinted_tile = other._hinted_tile;
	_tile_dimensions = other._tile_dimensions;
	_enabled = other._enabled;
	_visible = other._visible;
	_game = other._game;
	_statusbar = other._statusbar;
	_endgame = other._endgame;
	_mode = other._mode;
	_gameover = other._gameover;
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
	for (std::vector<Tile>::iterator it = _tiles.begin();
			it != _tiles.end(); ++it)
	{
		it->clear();
		it->hide();
	}
	disable();
}

/*
** creates all tile image and their pieces and initialize tiles.
*/
bool Board::_init_tiles()
{
	for (size_t i = 0; i < 9; i++)
	{
		Board::_tile_images[i] = mlx_new_image(
						Gui::mlx(), TILE_SIZE, TILE_SIZE);
		if (!Board::_tile_images[i])
			return false;
		Gui::apply_texture(
			Board::_tile_images[i], Gui::texture("tile"), Color::white, i, 3);
	}
	for (size_t i = 0; i < 5; i++)
	{
		Board::_piece_images[i] = mlx_new_image(
						Gui::mlx(), BUTTON_SIZE, BUTTON_SIZE);
		if (!Board::_piece_images[i])
			return false;
	}
	for (size_t i = 0; i < Board::_size; i++)
		_tiles.push_back(Tile(i));
	for (std::vector<Tile>::iterator it = _tiles.begin();
				it != _tiles.end(); ++it)
	{
		if (!it->init())
			return false;
	}
	resize();
	return true;
}

void Board::resize()
{
	if (!enabled())
		return;
	_dimensions = Rect::subrect(Gui::dimensions(), .9);
	mlx_resize_image(_background, _dimensions.width, _dimensions.height);
	Gui::apply_texture(_background, Gui::texture("board"));
	_background->instances[0].x = _dimensions.x;
	_background->instances[0].y = _dimensions.y;
	_tile_dimensions = Rect::subrect(_dimensions, 1.0 / sqrt());
	for (size_t i = 0; i < 9; i++)
	{
		mlx_resize_image(Board::_tile_images[i],
				 _tile_dimensions.width, _tile_dimensions.height);
		Gui::apply_texture(Board::_tile_images[i],
								Gui::texture("tile"), Color::white, i, 3);
	}
	for (size_t i = 0; i < 5; i++)
		mlx_resize_image(Board::_piece_images[i],
			_tile_dimensions.width, _tile_dimensions.height);
	Gui::apply_texture(Board::_piece_images[PT_BLACK],
							Gui::texture("piece"), Color::black);
	Gui::apply_texture(Board::_piece_images[PT_WHITE],
							 Gui::texture("piece"), Color::white);
	Gui::apply_texture(Board::_piece_images[PT_HINT],
							Gui::texture("piece"), Color::yellow);
	Gui::apply_texture(Board::_piece_images[PT_BLACKHOVER],
							Gui::texture("piece"), Color::black_alpha);
	Gui::apply_texture(Board::_piece_images[PT_WHITEHOVER],
							Gui::texture("piece"), Color::white_alpha);
	for (std::vector<Tile>::iterator it = _tiles.begin();
									it != _tiles.end(); ++it)
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

	if (_gameover)
		return false;
	if (_mode.enabled())
	{
		_mode.hover();
		return true;
	}
	if (_endgame.enabled())
	{
		_endgame.hover();
		return true;
	}
	if (enabled() && _statusbar.dimensions().contains(
								Gui::mouse().x, Gui::mouse().y))
		_statusbar.hover();
	if (enabled() && Board::dimensions().contains(
								Gui::mouse().x, Gui::mouse().y))
	{
		tile = get_hovered_tile();
		if (tile == -1)
		{
			if (_hovered_tile)
			{
				_hovered_tile->hover(false, _game.turn());
				_hovered_tile = nullptr;
			}
			return false;
		}
		if (_game.is_invalid_move(tile))
			return false;
		hovered = &_tiles[tile];
		if (hovered == _hovered_tile)
			return true;
		if (_hovered_tile)
			_hovered_tile->hover(false, _game.turn());
		_hovered_tile = hovered;
		_hovered_tile->hover(true, _game.turn());
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
	for (std::vector<int>::iterator it = captures.begin();
				it != captures.end(); ++it)
		_tiles[*it].clear();
}

bool Board::click()
{
	if (_gameover)
		return false;

	if (_mode.enabled())
	{
		 if (_mode.click())
		{
			if (_mode.buttons().size() == 3
				&& _mode.buttons().selected() == 2)
			{
				_game.set_defer_message();
				update_statusbar();
			}
			else if (_mode.buttons().size())
			{
				if (!_game.turn()
					&& ((_mode.buttons().selected() == 1
							&& !_game.is_deferred_turn())
					|| (_mode.buttons().selected() == 0
							&& _game.is_deferred_turn())))
					_game.set_player(!_game.player());
                _game.set_init_game(false);
				update_statusbar();
			}
			_mode.hide();
			enable();
		}
		return true;
	}
	if (_endgame.enabled())
	{
		if (_endgame.click())
		{
			_endgame.hide();
			_gameover = true;
			disable();
			return false;
		}
	}
	if (enabled() && _hovered_tile)
	{
		if (_game.is_invalid_move(_hovered_tile->pos()))
			return false;
		if (_hovered_tile->click(_game.turn()))
		{
			clear_hints();
			disable();
			_game.human_step(_hovered_tile->pos(), _game.turn());
			_remove_captures();
			_hovered_tile->hover(false, _game.turn());
			_hovered_tile = nullptr;
			update_statusbar();
		}
	}
	if (enabled() && !_game.vs_ai() && !_hinted_tile && _statusbar.click())
	{
		//disable();
		_game.dummy_step(_game.turn());
		_hinted_tile = &_tiles[_game.move()];
		_hinted_tile->hint(true);
		update_statusbar();
		//enable();
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
	size_t x = (Board::dimensions().x
					+ Board::dimensions().width
					- Gui::mouse().x) / get_tile_dimensions().width;
	size_t y = (Board::dimensions().y
					+ Board::dimensions().height
					- Gui::mouse().y) / get_tile_dimensions().height;
	if (x >= sqrt() || y >= sqrt())
		return -1;
    return (y * sqrt() + x);
}

void Board::clear_hints()
{
	if (_hinted_tile)
	{
		_hinted_tile->hint(false);
		_hinted_tile = nullptr;
	}
}

mlx_image_t *Board::piece_image(t_piecetype piecetype)
{
	return Board::_piece_images[piecetype];
}

void Board::loop()
{
	bool turn;

	if (_gameover)
		return ;
	if (++_loop_count % 10)
		return ;
	if (_mode.enabled())
		return ;
	if (_game.message())
	{
		_mode.show(_game.message());
		_game.message().clear();
		return ;
	}
	if (_game.vs_ai()
			&& ((_game.is_player_turn() ^ _game.is_game_swap_special_move())
			|| (!_game.is_player_turn() && _game.is_game_swap_deferred_move())))
	{
		enable();
		return ;
	}
	if (_game.vs_ai()
		&& (!_game.is_player_turn()
		|| _game.is_game_swap_special_move()))
	{
		turn = _game.turn();
		if (_game.step(turn))
		{
			_tiles[_game.move()].hover(true, turn);
			_tiles[_game.move()].click(turn);
			_remove_captures();
			enable();
			update_statusbar();
		}
	}
	if (!_game.vs_ai())
		enable();
}

void Board::update_statusbar()
{
	int endgame;
	_statusbar.set_turn(_game.turn());
	_statusbar.set_vs(_game.vs_ai());
	_statusbar.set_player1(_game.player());
	_statusbar.set_player2(!_game.player(), _game.vs_ai());
	_statusbar.set_last_time(std::to_string(_game.last_time()));
	_statusbar.set_avg_time(std::to_string(_game.average_time()));
	if (_game.player())
	{
		_statusbar.set_player1_captures(_game.board().maxi_captures());
		_statusbar.set_player2_captures(_game.board().mini_captures());
	}
	else
	{
		_statusbar.set_player1_captures(_game.board().mini_captures());
		_statusbar.set_player2_captures(_game.board().maxi_captures());
	}
	_statusbar.resize();

	endgame = _game.end_game();
	if (!endgame)
		return ;
	else if (endgame == 1)
		_endgame.show("Black");
	else if (endgame == 2)
		_endgame.show("White");
	else
		_endgame.show("Draw");
	disable();
}

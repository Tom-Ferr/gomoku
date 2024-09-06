#include <Board.hpp>
#include <Gui.hpp>

mlx_image_t			*Board::_tile_images[9] = {nullptr};
mlx_image_t			*Board::_piece_images[5] = {nullptr};
Rect				Board::_dimensions;
size_t				Board::_sqrt = DEFAULT_BOARD;
size_t				Board::_size = DEFAULT_BOARD * DEFAULT_BOARD;

Board::Board(Gui *gui)
: _background(nullptr), _gui(gui)
{ }

/*
** initializes the board, tiles, status bar, endgame and mode
** returns false if something fails, so it can gracefully exit
*/
bool Board::init()
{
	_background = mlx_new_image(Gui::mlx(), BACKGROUND_SIZE, BACKGROUND_SIZE);
	Board::_dimensions = Rect::subrect(Gui::dimensions(), .9);
	Gui::apply_texture(_background, Gui::texture("board"));
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

bool Board::show(t_vs vs, t_startingplayer starting, t_gamemode mode)
{
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
	//_endgame.show("AI");
	//_mode.show("Swap", true);
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
		Gui::apply_texture(Board::_tile_images[i], Gui::texture("tile"), Color::white, i, 3);
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
	Gui::apply_texture(_background, Gui::texture("board"));
	_background->instances[0].x = _dimensions.x;
	_background->instances[0].y = _dimensions.y;
	_tile_dimensions = Rect::subrect(_dimensions, 1.0 / sqrt());
	for (size_t i = 0; i < 9; i++)
	{
		mlx_resize_image(Board::_tile_images[i], _tile_dimensions.width, _tile_dimensions.height);
		Gui::apply_texture(Board::_tile_images[i], Gui::texture("tile"), Color::white, i, 3);
	}
	for (size_t i = 0; i < 5; i++)
		mlx_resize_image(Board::_piece_images[i], _tile_dimensions.width, _tile_dimensions.height);
	Gui::apply_texture(Board::_piece_images[PT_BLACK], Gui::texture("piece"), Color::black);
	Gui::apply_texture(Board::_piece_images[PT_WHITE], Gui::texture("piece"), Color::white);
	Gui::apply_texture(Board::_piece_images[PT_HINT], Gui::texture("piece"), Color::yellow);
	Gui::apply_texture(Board::_piece_images[PT_BLACKHOVER], Gui::texture("piece"), Color::black_alpha);
	Gui::apply_texture(Board::_piece_images[PT_WHITEHOVER], Gui::texture("piece"), Color::white_alpha);
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
	if (enabled() && Board::dimensions().contains(Gui::_mouse.x, Gui::_mouse.y))
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
	for (std::vector<int>::iterator it = captures.begin(); it != captures.end(); ++it)
		_tiles[*it].clear();
}

bool Board::click()
{
	if (_mode.enabled())
	{
		if (_mode.click())
		{
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
			/*
			** will change this maybe
			*/
			mlx_key_data_t key;
			key.action = MLX_PRESS;
			key.key = MLX_KEY_ESCAPE;
			Gui::key_hook(key, _gui);
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

	if (++_loop_count % 20)
		return ;

	/*vs AI and waiting for player turn, do nothing*/
	if (_game.vs_ai () && _game.is_player_turn())
		return ;
	/*vs AI and is AI turn, do something*/
	if (_game.vs_ai() && !(_game.is_player_turn()))
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
		else
		{
			// we've got a game over... (no possible move for AI)
			// still need to determine the winner
		}
	}
	/*vs P2 and there is no hint set, show hint for whatever player is the turn*/
	if (!_game.vs_ai() && !_hinted_tile)
	{
		_game.dummy_step(_game.turn());
		_hinted_tile = &_tiles[_game.move()];
		_hinted_tile->hint(true);
		enable();
	}
	std::cout << "Enabled: " << enabled() << std::endl;
	//std::cout << "Board queue loop" << std::endl;
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

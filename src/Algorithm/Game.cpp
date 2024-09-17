#include <Game.hpp>
#include <Logger.hpp>



Game::Game(int size, t_vs vs, t_startingplayer startingplayer, t_gamemode mode)
: _board(size), _ftc(_board), _turn(true), _player(true), _vs_ai(!static_cast<bool>(vs)),
_init_game(true), _game_mode(mode), _ai_nmoves(0), _total_nmoves(0),
_p1_nmoves(0), _p2_nmoves(0), _total_time(0), _last_time(0)
{
	/*
	** lets try to init those only if never initialized before...
	*/
	Free_Three_Checker::set_masks(6, size);
	Heuristics::set_masks(5, size);
	BoardState::set_masks(4, size, true);
	BoardState::set_masks(4, size, false);

	if (_game_mode == GM_STANDARD)
		_board.set_captures(true);
	else
		_board.set_captures(false);

    /*
	** who will play as BLACK?
	** _player is the player that will play as BLACK?
	** true for black, false for white
	*/
	if (startingplayer == SP_RANDOM)
		_player = static_cast<bool>(rand() % 2);
	else
		_player = !static_cast<bool>(startingplayer);

	/*
	** turn starts always as true (as its black to play)
	*/
	_turn = true;
	std::cout << "vs AI/P2: " << _vs_ai << std::endl;
	std::cout << "starting Player: " << _player << std::endl;
	std::cout << "Game Mode: " << _game_mode << std::endl;

    /*
	** Sets up initial message
	*/
	if (_game_mode == GM_PRO)
		_message = GameMessage(true, false, MSG_PRO_INFO, "Pro");
	else if (_game_mode == GM_LONGPRO)
		_message = GameMessage(true, false, MSG_LONGPRO_INFO, "Long Pro");
	else if (_game_mode == GM_SWAP)
		_message = GameMessage(true, false, MSG_SWAP_INFO, "Swap");
	else if (_game_mode == GM_SWAP2)
		_message = GameMessage(true, false, MSG_SWAP2_INFO, "Swap2");
}

Game::Game(const Game& other)
:_board(other._board), _ftc(_board)
{
	*this = other;
}

Game &Game::operator=(const Game& other)
{
	if (this != &other)
	{
		_board = other._board;
		_ftc = Free_Three_Checker(_board);
		_turn = other._turn;
		_player = other._player;
		_vs_ai = other._vs_ai;
		_init_game = other._init_game;
		_game_mode = other._game_mode;
		_ai_nmoves = other._ai_nmoves;
		_total_nmoves = other._total_nmoves;
		_p1_nmoves = other._p1_nmoves;
		_p2_nmoves = other._p2_nmoves;
		_total_time = other._total_time;
		_last_time = other._last_time;
		_message = other._message;
	}
	return *this;
}

Game::~Game() {}

bool Game::human_step(size_t pos, bool turn)
{
	std::string mode_name;

	check_capture(pos, !turn);
	_board.applymove(pos, turn);
	_ftc = Free_Three_Checker(_board);
	_turn = !_turn;
	_total_nmoves++;
	if ((game_mode() == GM_PRO || game_mode() == GM_LONGPRO) && _total_nmoves == 2)
		set_init_game(false);
	if ((_total_nmoves == 3 || _total_nmoves == 5) && _init_game &&  (game_mode() == GM_SWAP2 || game_mode() == GM_SWAP))
	{
		mode_name = MSG_SWAP;
		if (game_mode() == GM_SWAP2 && _total_nmoves == 3)
			mode_name = MSG_SWAP2;
		if (vs_ai())
		{
			if (_total_nmoves == 3 || (_total_nmoves == 5 && game_mode() == GM_SWAP2))
			{
				_message = GameMessage(false, false, MSG_AI_CHOSE_WHITE, mode_name);
				set_init_game(false);
				_player = true; /* set player to black*/
			}
		}

		else if (_total_nmoves == 3) /* player 1 is black so player two must choose. */
			_message = GameMessage(player(), true, (player() ? MSG_P2_CHOOSES : MSG_P1_CHOOSES), mode_name);
		else
			_message = GameMessage(player(), true, (player() ? MSG_P1_CHOOSES : MSG_P2_CHOOSES), mode_name);
	}
	Logger::log_move(_total_nmoves, "Human", pos, _turn, _board);
	std::cout << "Appying move: " << pos << std::endl;
	return true;
}

/*
** asks AI for the best move but does not apply it to the board.
** no need to check for captures.
*/
bool Game::dummy_step(bool turn)
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	/*
	** create a dummy board
	*/
	BoardState dummy(_board);
	std::pair<int, BigInt> result;

	/*
	** starting state, just as step.
	** lets please move this into a function and get this code
	** out of here.
	*/
	if (is_init_game() && _init_game_handler(turn, true))
		return true;
	if (!turn)
		dummy.swap_states();
	_captures.clear();
	result = Node(3, INT_MIN, INT_MAX, dummy).minimax();
	if (result.second == 0)
	{
		std::cout << "No move found" << std::endl;
		return false;
	}
	_move = result.second.pos();
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	_last_time = duration.count();
	_total_time += duration.count();
	Logger::log_move(_total_nmoves, "Dummy", _move, turn);
	_ai_nmoves++;
	Logger::log_move(_total_nmoves, "dummy", _move, _turn);
	return true;
}

bool Game::step(bool turn)
{
	Node::node_count = 0;
	std::cout << "Step" << std::endl;
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::pair<int, BigInt> result;

	if (is_init_game() && _init_game_handler(turn))
    {
        update_freechecker();
		return true;
    }
	if (turn)
		result = Node(3, INT_MIN, INT_MAX, _board).minimax();
	else
	{
		BoardState other(_board);
		other.swap_states();
		result = Node(3, INT_MIN, INT_MAX, other).minimax();
	}
	if (result.second == 0)
		std::cout << "No move found" << std::endl;
	else
	{
		_move = result.second.pos();
		check_capture(_move, !turn);
		_board.applymove(_move, turn);
		std::cout << "Move: " << _move << std::endl;
	}
	//std::cout << _board << std::endl;
	//_board.print();
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	_last_time = duration.count();
	_total_time += duration.count();
	_ai_nmoves++;
	std::cout << "Time taken (step): " << duration.count() << " seconds" << std::endl;
	std::cout << "Move: " << _move << std::endl;
	std::cout << "Heuristic: " << result.first << std::endl;
	std::cout << "Node Count: " << Node::node_count << std::endl;
	Logger::log_move(_total_nmoves, "AI", _move, _turn, _board);
	_ftc = Free_Three_Checker(_board);
	_turn = !_turn;
	if (result.second == 0)
		return false;
	return true;
}

void Game::check_capture(size_t pos, bool turn)
{
	if (_board.with_captures() == false)
		return ;
	BigInt state =  _board.totalboard();

	_captures.clear();
	_board.check_capture(pos, turn);
	state = state ^ _board.totalboard();
	if (state != 0)
	{
		for (size_t i = 0; i < state.size() * state.size(); i++)
		{
			if (state.get_bit(i))
				_captures.push_back(i);
		}
	}
	std::cout << "Captures: " << state << std::endl;
}

BoardState &Game::board()
{
	return _board;
}

size_t Game::move() const
{
	return _move;
}

std::vector<int> &Game::captures()
{
	return _captures;
}


/*
** Valid moves checker exclusive for the pro/longpro game modes.
*/
bool Game::_is_pro_invalid_move(size_t pos)
{
	int padding;
	size_t black_piece;
	Rect p;

	if (!_turn) /*rule applies only to black pieces*/
		return false;
	if (_board.mystate(true) == 0) /*no black pieces on the board yet. all moves are valid*/
	{
		return (pos != static_cast<size_t>(_board.size() / 2));
	}
	black_piece = _board.mystate(true).pos();
	if (_game_mode == GM_PRO)
		padding = 2;
	else
		padding = 3;
	p = Rect(black_piece % _board.sqrt(), black_piece / _board.sqrt(), 0, 0);
	p.width = p.x;
	p.height = p.y;
	while (padding)
	{
		if (p.height < static_cast<size_t>(_board.sqrt()))
			p.height++;
		if (p.width < static_cast<size_t>(_board.sqrt()))
			p.width++;
		if (p.x)
			p.x--;
		if (p.y)
			p.y--;
		padding--;
	}
	p.height-= p.y;
	p.width -= p.x;
	if (p.contains(pos % _board.sqrt(), pos / _board.sqrt()))
		return true;
	return false;
}


bool Game::is_invalid_move(const size_t &pos)
{
	if (is_init_game() && (_game_mode == GM_PRO
		|| _game_mode == GM_LONGPRO))
	{
		return _is_pro_invalid_move(pos);
	}
	return _ftc.is_free_three(pos, _turn);
}

void Game::update_freechecker()
{
	_ftc = Free_Three_Checker(_board);
}

/*
** whose turn
** false for white, true for black
*/
bool &Game::turn()
{
	return _turn;
}

/*
** player one piece.
** false for white, true for black
*/
bool Game::player()
{
	return _player;
}

void Game::set_player(bool player)
{
	_player = player;
}

/*
** false for AI, true for vs Player2
*/
bool Game::vs_ai()
{
	return _vs_ai;
}

bool Game::vs_p2()
{
	return !_vs_ai;
}

bool Game::is_player_turn()
{
	return (_turn == _player);
}

bool Game::is_init_game()
{
	return _init_game;
}

t_gamemode &Game::game_mode()
{
	return _game_mode;
}

float Game::average_time()
{
	if (_ai_nmoves == 0)
		return 0.0;
	return _total_time / _ai_nmoves;
}

float Game::last_time()
{
	return _last_time;
}

/*
** 0 = false
** 1 = black wins
** 2 = white wins
** 3 = draw
*/
int Game::end_game()
{
	Heuristics h(_board);
	int value = h.run(true);
	std::cout << "Current Heuristic: " << value << std::endl;
	h.describe_heuristic();
	if (((~_board.totalboard()) & BoardState::mask) == 0)
		return 0;
	if (h.maxi_wins() || _board.maxi_captures() >= 5)
		return 1;
	else if (h.mini_wins() || _board.mini_captures() >= 5)
		return 2;
	else if (_board.totalboard() == 0)
		return 3;
	return 0;
}

/*
** A special move is needed for the swap game modes.
** if AI is black, it should play the white move if _total_nmoves is 1
** if AI would defer, it would also maks move 4 special, but
** for now AI will always choose whites.
*/
bool Game::is_game_swap_special_move()
{
	if (!_init_game)
		return false;
	if (_game_mode == GM_SWAP2 || _game_mode == GM_SWAP)
	{
		if (_total_nmoves == 1)
			return true;
	}
	return false;
}

bool Game::is_game_swap_deferred_move()
{
	if (!_init_game)
		return false;
	if (_game_mode == GM_SWAP2)
	{
		if (_total_nmoves == 4)
			return true;
	}
	return false;
}
/*
** initialization handler for game swap mode.
*/
bool Game::_init_game_swap(bool turn, bool dummy)
{
	std::string mode;

	if (!(_total_nmoves)) /*if no black pieces on the board*/
	{
		_init_game_standard(turn, dummy); /*apply the same initial move*/
		set_init_game(true);	/*since 3 moves are needed, the game is still in init state*/
		if (!dummy)
			_total_nmoves++;
		return true; /*game step has been handled*/
	}
	else if (!_init_game)
		return false;
	else if (_total_nmoves <= 3) /*next white and black moves*/
	{
		if (!dummy)
			_total_nmoves++;
		if (_total_nmoves == 3)
		{
			mode = MSG_SWAP;
			if (_game_mode == GM_SWAP2)
				mode = MSG_SWAP2;
			if (turn == !_player && vs_ai())
				_message = GameMessage(true, true, MSG_P1_CHOOSES, mode);
			if (_game_mode == GM_SWAP)
				set_init_game(false);
		}
		return false; /*moves are handled normally by minimax*/
	}
	if (!dummy)
		_total_nmoves++;
	if (!dummy || _total_nmoves >= 5)
		set_init_game(false);
	return false;
}

/*
** initialization handler for game pro mode.
*/
bool Game::_init_game_pro(bool turn, bool dummy)
{
	size_t padding;
	BoardState tmp;

	if (!turn)
		return false;
	if (_board.mystate(true) == 0) /*if no black pieces on the board*/
	{
		_init_game_standard(turn, dummy); /*apply the same initial move*/
		set_init_game(true);	/*since 2 moves are needed, the game is still in init state*/
		return true;
	}
	else if (dummy && _board.mystate(true).bitCount() >= 2)
	{
		/*
		two or more black pieces
		game is no longer in init mode
		*/
		set_init_game(false);
		return false;
	}
	else
	{
		tmp.applymove(_board.mystate(true));
		padding = 3;
		if (_game_mode == GM_LONGPRO)
			padding = 4;
		while (--padding > 0)
			tmp.applymove(tmp.expanded_free());
		std::cout << tmp << std::endl;
		_move = tmp.expanded_free().pos();
		if (_move == _board.otherstate(true).pos())
			_move = (tmp.expanded_free() ^ _board.otherstate(true)).pos();
		if (dummy)
			return true;
		set_init_game(false);
		_board.applymove(_move, turn);
		_ftc = Free_Three_Checker(_board);
		_turn = !_turn;
	}
	return true;
}

/*
** places the first move in the center of the board.
*/
bool Game::_init_game_standard(bool turn, bool dummy)
{
	set_init_game(false);
	if (!turn)
		return false;
	_move = _board.size() / 2;
	if (dummy)
		return true;
	_board.applymove(_move, turn);
	_ftc = Free_Three_Checker(_board);
	_turn = !_turn;
	return true;
}

/*
** returns true if the game was handled by the init_game_handler
** if dummy, do not apply the move, as it means it's just a hint.
*/
bool Game::_init_game_handler(bool turn, bool dummy)
{

	if (_game_mode == GM_STANDARD)
		return _init_game_standard(turn, dummy);
	else if (_game_mode == GM_PRO || _game_mode == GM_LONGPRO)
		return _init_game_pro(turn, dummy);
	else if (_game_mode == GM_SWAP || _game_mode == GM_SWAP2)
		return _init_game_swap(turn, dummy);
	return false;
}

bool Game::is_deferred_turn()
{
	if (_game_mode == GM_SWAP2)
		return _total_nmoves == 5;
	return false;
}

GameMessage &Game::message()
{
	return _message;
}

void Game::set_defer_message()
{
	_message = GameMessage(_turn, false, MSG_DEFFERRED, MSG_SWAP2);
	set_init_game(true);
}

void Game::set_init_game(bool on)
{
    if (on)
    {
        _init_game = true;
        _board.set_captures(false);
    }
    else
    {
        _init_game = false;
        _board.set_captures(true);
    }
}

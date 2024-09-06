#include <Game.hpp>


Game::Game(int size, t_vs vs, t_startingplayer startingplayer, t_gamemode mode)
: _board(size), _ftc(_board), _turn(true), _player(true), _vs_ai(!static_cast<bool>(vs)),
_init_game(true), _game_mode(mode), _ai_nmoves(0), _total_nmoves(0), _p1_nmoves(0), _p2_nmoves(0), _total_time(0), _last_time(0)
{
	/*
	** lets try to init those only if never initialized before...
	*/
	Free_Three_Checker::set_masks(6, size);
	Heuristics::set_masks(5, size);
	BoardState::set_masks(4, size, true);
	BoardState::set_masks(4, size, false);
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
	}
	return *this;
}

Game::~Game() {}

bool Game::human_step(size_t pos, bool turn)
{
	check_capture(pos, !turn);
	_board.applymove(pos, turn);
	_ftc = Free_Three_Checker(_board);
	_turn = !_turn;
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
	result = Node(3, INT_MIN, INT_MAX, _board).minimax();
	if (result.second == 0)
	{
		std::cout << "No move found" << std::endl;
		return false;
	}
	_move = result.second.pos();
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	_last_time = duration.count();
	_total_time += duration.count();
	_ai_nmoves++;
	return true;
}

bool Game::step(bool turn)
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::pair<int, BigInt> result;

	if (is_init_game() && _init_game_handler(turn))
		return true;
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
	std::cout << _board << std::endl;
	_board.print();
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	_last_time = duration.count();
	_total_time += duration.count();
	_ai_nmoves++;
	std::cout << "Time taken (step): " << duration.count() << " seconds" << std::endl;
	std::cout << "Move: " << _move << std::endl;
	std::cout << "Heuristic: " << result.first << std::endl;
	_ftc = Free_Three_Checker(_board);
	_turn = !_turn;
	if (result.second == 0)
		return false;
	return true;
}

void Game::check_capture(size_t pos, bool turn)
{
	return; (void)pos;(void)turn;
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
bool Game::_is_pro_valid_move(size_t pos)
{
	size_t padding;
	size_t black_piece;

	if (!_turn) /*rule applies only to black pieces*/
		return true;
	if (_board.mystate(true) == 0) /*no black pieces on the board yet. all moves are valid*/
		return true;
	black_piece = _board.mystate(true).pos();
	if (_game_mode == GM_PRO)
		padding = 2;
	else
		padding = 3;
	/* black_piece and pos is a value from 0 to 360 representing a 19x19 board
	write a function that checks if pos is within *padding* squares of the blackpiece
	keep in mind that its types are both size_t
	*/
	


	return false;
}


bool Game::is_valid_move(const size_t &pos)
{
	if (is_init_game() && _game_mode == GM_PRO
		|| _game_mode == GM_LONGPRO)
	{
		return _is_pro_valid_move(pos);
	}
	return _ftc.is_free_three(pos);
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
	int value = h.run();

	if (((~_board.totalboard()) & BoardState::mask) == 0)
		return 0;
	if (value == 32)
		return 1;
	else if (value == -32)
		return 2;
	else if (_board.totalboard() == 0)
		return 3;
	return 0;
}


/*
** initialization handler for game swap2 mode.
*/
bool Game::_init_game_swap2(bool turn, bool dummy)
{
	(void)turn;(void)dummy;
	return true;
}

/*
** initialization handler for game swap mode.
*/
bool Game::_init_game_swap(bool turn, bool dummy)
{
	(void)turn;(void)dummy;
	return true;
}


/*
** initialization handler for game long pro mode.
*/
bool Game::_init_game_longpro(bool turn, bool dummy)
{
	(void)turn;(void)dummy;
	return true;
}


/*
** initialization handler for game pro mode.
*/
bool Game::_init_game_pro(bool turn, bool dummy)
{
	(void)turn;(void)dummy;
	return true;
}

/*
** places the first move in the center of the board.
*/
bool Game::_init_game_standard(bool turn, bool dummy)
{
	_move = _board.size() / 2;
	_init_game = false;
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
	else if (_game_mode == GM_PRO)
	{
		if (_total_nmoves == 0)
		{
			_move = _board.size() / 2;
			_init_game = false;
			if (dummy)
				return true;
			_board.applymove(_move, turn);
			_ftc = Free_Three_Checker(_board);
			_turn = !_turn;
			return true;
		}
	}
	return false;
}


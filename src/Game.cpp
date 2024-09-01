#include <Game.hpp>


Game::Game(int size, t_vs vs, t_startingplayer startingplayer, t_gamemode mode)
: _board(size), _ftc(_board), _turn(true), _player(true), _vs_ai(true),
_init_game(true), _game_mode(GM_STANDARD)
{
	/*
	** lets try to init those only if never initialized before...
	*/
	Free_Three_Checker::set_masks(6, size);
	Heuristics::set_masks(5, size);
	BoardState::set_masks(4, size);

	_game_mode = mode;

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
	_vs_ai = !static_cast<bool>(vs);
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
	}
	return *this;
}

Game::~Game() {}

/*
** if both players are maximizing, it makes no sense to have a turn
** in the board state (this will always be true, unless during minimax)
** where next depth is min, then the following is max and so on...
** So the turn will be determined by the game itself
*/
bool Game::step(bool turn)
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::pair<int, BigInt> result;

	if (((~_board.totalboard()) & BoardState::mask) == 0)
	{
		_move = _board.size() / 2;
		_board.applymove(_move, turn);
		_ftc = Free_Three_Checker(_board);
		std::cout << "Move: " << _move << std::endl;
		_turn = !_turn;
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
	(void)start;
	//std::cout << _board << std::endl;
	_board.print();
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Time taken (step): " << duration.count() << " seconds" << std::endl;
	std::cout << "Move: " << _move << std::endl;
	_ftc = Free_Three_Checker(_board);
	_turn = !_turn;
	if (result.second == 0)
		return false;
	return true;
}

void Game::check_capture(size_t pos, bool turn)
{
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

bool Game::is_double_free_three(const size_t &pos)
{
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


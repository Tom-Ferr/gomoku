#include <Game.hpp>

Game::Game(int size)
: _board(size), _ftc(_board)
{
	Free_Three_Checker::set_masks(6, size);
	Heuristics::set_masks(5, size);
	BoardState::set_masks(4, size);
}

Game::Game(const Game& other)
:_board(other._board), _ftc(_board)
{
}

Game &Game::operator=(const Game& other)
{
	if (this != &other)
	{
		_board = other._board;
		_ftc = Free_Three_Checker(_board);
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
	_board.print();
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Time taken (step): " << duration.count() << " seconds" << std::endl;
	std::cout << "Move: " << _move << std::endl;
	_ftc = Free_Three_Checker(_board);
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

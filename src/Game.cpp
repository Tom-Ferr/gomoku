#include <Game.hpp>

Game::Game(int size)
: _board(size)
{
}

Game::Game(const Game& other)
{
	_board = other._board;
}

Game &Game::operator=(const Game& other)
{
	if (this != &other)
		_board = other._board;
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
		_board.check_capture(_move, !turn);
		_board.applymove(_move, turn);
		std::cout << "Move: " << _move << std::endl;
	}
	std::cout << _board << std::endl;
	_board.print();
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Time taken (step): " << duration.count() << " seconds" << std::endl;
	std::cout << "Move: " << _move << std::endl;
	if (result.second == 0)
		return false;
	return true;
}

BoardState &Game::board()
{
	return _board;
}

size_t Game::move() const
{
	return _move;
}

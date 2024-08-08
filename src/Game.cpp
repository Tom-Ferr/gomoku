#include <Game.hpp>

Game::Game(int size)
: _board(size)
{
	_board.applymove(180, false);
	std::cout << _board << std::endl;

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

bool Game::step()
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::pair<int, BigInt> result;
	if (_board.turn())
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
		_board.applymove(result.second, _board.turn());
		std::cout << "Move: " << result.second << std::endl;
		_board.flip_turn();
	}
	std::cout << _board << std::endl;
	_board.print();
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Time taken (step): " << duration.count() << " seconds" << std::endl;
	if (result.second == 0)
		return false;
	return true;
}

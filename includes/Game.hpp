#ifndef GAME_HPP
# define GAME_HPP

# include <chrono>
# include "gomoku.hpp"
# include "BoardState.hpp"
# include "Node.hpp"

class Game
{
	private:
		BoardState		_board;

	public:
		Game(int size=19);
		Game(const Game& other);
		Game &operator=(const Game& other);
		~Game();
		bool step();
};


#endif
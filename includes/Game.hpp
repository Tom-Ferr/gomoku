#ifndef GAME_HPP
# define GAME_HPP

# include <chrono>
# include <gomoku.hpp>
# include <BoardState.hpp>
# include <Free_Three_Checker.hpp>
# include <Node.hpp>
# include <vector>

class Game
{
	private:
		BoardState			_board;
		size_t				_move;
		std::vector<int>	_captures;
		Free_Three_Checker	_ftc;

	public:
		Game(int size=19);
		Game(const Game& other);
		Game &operator=(const Game& other);
		~Game();
		bool step(bool turn=true);
		size_t move() const;
		void check_capture(size_t pos, bool turn);
		std::vector<int> &captures();
		BoardState &board();
		bool is_double_free_three(const size_t &pos);
		void update_freechecker();
};

#endif
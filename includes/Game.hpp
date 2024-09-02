#ifndef GAME_HPP
# define GAME_HPP

# include <chrono>
# include <gomoku.hpp>
# include <BoardState.hpp>
# include <Free_Three_Checker.hpp>
# include <Heuristics.hpp>
# include <Node.hpp>
# include <vector>

typedef enum e_gamemode
{
	GM_STANDARD,
	GM_PRO,
	GM_SWAP,
	GM_SWAP2,
	GM_LONGPRO
}	t_gamemode;

typedef enum e_startingplayer
{
	SP_PLAYER1,
	SP_PLAYER2AI,
	SP_RANDOM
}	t_startingplayer;

typedef enum e_vs
{
	VS_AI,
	VS_P2
}	t_vs;

class Game
{
	private:
		BoardState						_board;
		size_t							_move;
		std::vector<int>				_captures;
		Free_Three_Checker				_ftc;
		/*
		** board info
		*/
		bool							_turn;
		bool							_player;
		bool							_vs_ai;
		bool							_init_game;
		t_gamemode						_game_mode;
		size_t							_ai_nmoves;
		float						 	_total_time;
		float						 	_last_time;


	public:
		Game(int size=19, t_vs vs=VS_AI, t_startingplayer startingplayer=SP_PLAYER1, t_gamemode mode=GM_STANDARD);
		Game(const Game& other);
		Game &operator=(const Game& other);
		~Game();
		bool step(bool turn=true);
		bool human_step(size_t pos, bool turn);
		bool dummy_step(bool turn);
		size_t move() const;
		void check_capture(size_t pos, bool turn);
		std::vector<int> &captures();
		BoardState &board();
		bool is_double_free_three(const size_t &pos);
		void update_freechecker();
		bool &turn();
		bool player();
		bool vs_ai();
		bool vs_p2();
		t_gamemode &game_mode();
		bool is_player_turn();
		bool is_init_game();
		float average_time();
		float last_time();
		int end_game();
};

#endif
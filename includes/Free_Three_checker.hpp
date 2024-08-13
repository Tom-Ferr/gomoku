# include <gomoku.hpp>

#ifndef FREE_THREE_CHECKER_HPP
# define FREE_THREE_CHECKER_HPP
# include <Mask.hpp>
# include <BoardState.hpp>

class Free_Three_Checker
{

private:
	static Mask _masks;
	BoardState &_state;

public:
		Free_Three_Checker(BoardState &state);
        Free_Three_Checker(const Free_Three_Checker& other);
		Free_Three_Checker &operator=(const Free_Three_Checker& other);
        ~Free_Three_Checker();

        bool check(int pos, char orientation);

        static void set_masks(int mask_size, int board_sqrt);

};

#endif
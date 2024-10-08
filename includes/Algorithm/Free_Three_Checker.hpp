#ifndef FREE_THREE_CHECKER_HPP
# define FREE_THREE_CHECKER_HPP

# include <Mask.hpp>
# include <BoardState.hpp>
# include <gomoku.hpp>

class Free_Three_Checker
{

	private:
		static Mask _masks;
		BoardState &_state;
		static BigInt _static_state;
		Free_Three_Checker();

	public:
			Free_Three_Checker(BoardState &state);
			Free_Three_Checker(const Free_Three_Checker& other);
			Free_Three_Checker &operator=(const Free_Three_Checker& other);
			~Free_Three_Checker();

			bool check(int pos, char orientation, bool turn=true);
			bool is_free_three(int pos, bool turn=true);
			static void set_masks(int mask_size, int board_sqrt);
};

#endif

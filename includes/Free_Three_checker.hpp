# include <gomoku.hpp>

#ifndef FREE_THREE_CHECKER_HPP
# define FREE_THREE_CHECKER_HPP
# include <Mask.hpp>


class Free_Three_Checker
{

private:
	static Mask _masks;
        size_t _board_sqrt;
        size_t _board_size;
        BigInt _my_state;
        BigInt _other_state;
public:
        Free_Three_Checker(int board_sqrt, BigInt my_state, BigInt other_state);
        Free_Three_Checker(const Free_Three_Checker& other);
	Free_Three_Checker &operator=(const Free_Three_Checker& other);
        ~Free_Three_Checker();

        bool check(int pos, char orientation);

        static void set_masks(int mask_size, int board_sqrt);

};

#endif
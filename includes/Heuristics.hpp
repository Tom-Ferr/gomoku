#ifndef HEURISTICS_HPP
# define HEURISTICS_HPP
# include <gomoku.hpp>
# include <vector>
#include <BoardState.hpp>

class Heuristics
{
private:
	BoardState &_state;

    // std::vector<int> _scores = std::vector<int>();
    int _max_score = 0;
    int _min_score = 0;
    int _heuristic = 0;

    const int _mask_size = 5;
    const int _edge_mask_size = 7;
    const char _modes[4] = {HORIZONTAL, VERTICAL, CRESCENDO, DECRESCENDO};

    static Mask _masks;
public:
	Heuristics(BoardState &state);
    Heuristics(int board_sqrt, BigInt my_state, BigInt other_state);
    Heuristics(const Heuristics& other);
    Heuristics& operator=(const Heuristics& other);
    ~Heuristics();

    int run();
    int endgame(size_t pos);

    static void set_masks(int mask_size, int board_sqrt);
private:
    int check_captures(const BigInt &target, const BigInt &other_target, const size_t &pos, const Mask::inner_map &masks);
    int get_score(const BigInt &target, const BigInt &edge, const BigInt &other_target, const size_t &pos, const Mask::inner_map &masks);
    bool board_eval(int pos, char orienation, bool endgame);
};



#endif

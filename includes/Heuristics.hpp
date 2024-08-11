#ifndef HEURISTICS_HPP
# define HEURISTICS_HPP
# include <gomoku.hpp>
# include <vector>

class Heuristics
{
private:
    int _board_sqrt;
    int _board_size;
    BigInt _my_state;
    BigInt _other_state;
    
    // std::vector<int> _scores = std::vector<int>();
    int _max_score = 0;
    int _min_score = 0;
    int _heuristic = 0;
    
    const int _mask_size = 5;
    const int _edge_mask_size = 7;
    const char _modes[4] = {'h', 'v', 'c', 'd'};

    static Mask _masks;
public:
    Heuristics(int board_sqrt, BigInt my_state, BigInt other_state);
    Heuristics(const Heuristics& other);
    Heuristics& operator=(const Heuristics& other);
    ~Heuristics();

    int run();

    static void set_masks(int mask_size, int board_sqrt);
private:
    int check_captures(const BigInt &target, const BigInt &other_target, const size_t &pos, Mask::inner_map &masks);
    int get_score(const BigInt &target, const BigInt &edge, const BigInt &other_target, const size_t &pos, Mask::inner_map &masks);
    bool board_eval(int pos, char orienation);
};



#endif

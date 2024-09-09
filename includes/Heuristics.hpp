#ifndef HEURISTICS_HPP
# define HEURISTICS_HPP
# include <gomoku.hpp>
# include <vector>
# include <BoardState.hpp>
# include <map>

class Heuristics
{
private:
	BoardState &_state;
    std::map<char, std::vector<int>> _my_scores ;
    std::map<char, std::vector<int>> _other_scores ;
    int _max_score = 0;
    int _min_score = 0;
    int _heuristic = 0;

    const int _mask_size = 5;
    const int _edge_mask_size = 7;
    const char _modes[4] = {HORIZONTAL, VERTICAL, CRESCENDO, DECRESCENDO};

    static Mask _masks;


    std::map<std::string, int> _points;
    void _set_points(bool my, int points);
public:
	Heuristics(BoardState &state);
    Heuristics(int board_sqrt, BigInt my_state, BigInt other_state);
    Heuristics(const Heuristics& other);
    Heuristics& operator=(const Heuristics& other);
    ~Heuristics();

    int run();
    int endgame(size_t pos);
    void describe_heuristic() const;

    static void set_masks(int mask_size, int board_sqrt);
private:
    size_t is_capturable(const size_t &pos, const Mask::inner_map &masks, bool maximizing);
    int get_score(const BigInt &target, const BigInt &edge, const BigInt &other_target, const size_t &pos, const Mask::inner_map &masks);
    bool board_eval(int pos, char orienation, bool endgame);
    void to_compute(bool my, std::vector<int> &target, size_t pos, const Mask::inner_map &masks);
};



#endif

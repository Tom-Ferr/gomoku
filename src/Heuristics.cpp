#include <Heuristics.hpp>

Mask Heuristics::_masks = Mask();

Heuristics::Heuristics( int board_sqrt, BigInt my_state, BigInt other_state)
: _board_sqrt(board_sqrt),
_board_size(board_sqrt * board_sqrt),
_my_state(~my_state),
_other_state(~other_state) {}

Heuristics::Heuristics(const Heuristics& other)
{
    _board_sqrt = other._board_sqrt;
    _board_size = other._board_size;
    _my_state = other._my_state;
    _other_state = other._other_state;

    // _scores = other._scores;
    _max_score = other._max_score;
    _min_score = other._min_score;
}

Heuristics &Heuristics::operator=(const Heuristics& other)
{
	if (this != &other)
    {
		_board_sqrt = other._board_sqrt;
        _board_size = other._board_size;
        _my_state = other._my_state;
        _other_state = other._other_state;

        // _scores = other._scores;
        _max_score = other._max_score;
        _min_score = other._min_score;
    }
	return *this;
}

Heuristics::~Heuristics() {}

void Heuristics::set_masks(int mask_size, int board_sqrt)
{
    Heuristics::_masks = Mask(mask_size, board_sqrt);
}



int Heuristics::check_captures(const BigInt &target, const BigInt &other_target, const size_t &pos, Mask::inner_map &masks)
{
        // size_t my_bits = (target & masks['f'][pos][0]).bitCount();
        // size_t other_bits = (other_target & masks['f'][pos][0]).bitCount();
        // if (other_bits == 2 and my_bits < 3)
        // {
        //     size_t sub_bits = 0;
        //     bool capture = false;
        //     for (size_t i = 0; i < 2; i++)
        //     {
        //         BigInt other_sub_target = other_target & masks['s'][pos][i];
        //         sub_bits += other_sub_target.bitCount();
        //         BigInt sub_target = target & masks['s'][pos][i];
        //         if (sub_target.bitCount() == 1 and other_sub_target > sub_target)
        //             capture = true;
        //     }
        //     if (sub_bits == 4 and capture == true)
        //         return (1 << captures) + 1;
        // }
        // else
            return 0;
}

int Heuristics::get_score(const BigInt &target, const BigInt &edge, const BigInt &other_target, const size_t &pos, Mask::inner_map &masks)
{
    if ((masks['f'][pos][0] & other_target) != 0)
        return check_captures(target, other_target, pos, masks);

    size_t bits = (target & masks['f'][pos][0]).bitCount();
    int score = 1 << bits;

    for (size_t i = 0; i < 2; i++)
    {
        BigInt sub_target = target & masks['s'][pos][i];
        size_t sub_bits = sub_target.bitCount();
        if (bits == sub_bits && bits > 1)
        {
            BigInt target_first = target & masks['v'][pos][1];
            BigInt target_last = target & masks['v'][pos][5];
            BigInt edge_first;
            BigInt edge_last;
            // if (masks['e'][pos][0] != o)
                edge_first = edge & masks['v'][pos][0];
            // else
            //     edge_first = target_first;
            // if (masks['e'][pos][0] != o)
                edge_last = edge & masks['v'][pos][6];
            // else
                // edge_last = target_last;
            if ((target_first != 0 && edge_first == 0)
            || (target_last != 0 && edge_last == 0)
            || ((target_first == 0 && target_last == 0) && (edge_first == 0 || edge_last == 0))
            )
                score += 3;
            break ;
        }
    }
    return score;
}

bool Heuristics::board_eval(int pos, char orientation)
{
        Mask::inner_map masks = _masks[orientation];

        if (masks['f'][pos][0] == 0)
            return false;
        BigInt edge = (_other_state & masks['e'][pos][0]);
        edge = edge | (_my_state & masks['e'][pos][0]);

        BigInt target = (_my_state & masks['f'][pos][0]);
        if (target == masks['f'][pos][0])
        {
            _heuristic = 32;
            return true;
        }
        BigInt other_target = (_other_state & masks['f'][pos][0]);
        if (other_target == masks['f'][pos][0])
        {
            _heuristic = -32;
            return true;
        }

        int score = get_score(target, edge, other_target, pos, masks);
        if (score == 32)
        {
            _max_score = score;
            return true;
        }
        if (score > _max_score)
            _max_score = score;

        score = get_score(other_target, edge, target, pos, masks) * -1;
        if (score == -32)
        {
            _min_score = score;
            return true;
        }
        if (score < _min_score)
            _min_score = score;
        return false;
}

int Heuristics::run()
{
    for (size_t pos = 0; pos < _board_size; pos++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (board_eval(pos, _modes[i]))
                return _heuristic;
        }
    }
    if (_max_score > std::abs(_min_score))
    {
        _heuristic = _max_score;
        return _max_score;
    }
    _heuristic = _min_score;
    return _min_score;
}
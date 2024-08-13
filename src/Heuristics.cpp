#include <Heuristics.hpp>

Mask Heuristics::_masks = Mask();

Heuristics::Heuristics(BoardState &state)
: _state(state)
{

}

/*
Heuristics::Heuristics( int board_sqrt, BigInt my_state, BigInt other_state)
: _state_sqrt(board_sqrt),
_state_size(board_sqrt * board_sqrt)
{
	//std::cout << "Board Size:" << _state_size << std::endl;
	//std::cout << "my_state: " << my_state << std::endl;
	//std::cout << "other_state: " << other_state << std::endl;
	_my_state = (~my_state) & BoardState::mask;
	_other_state = (~other_state) & BoardState::mask;
	//std::cout << "_my_state: " << _my_state << std::endl;
	//std::cout << "_other_state: " << _other_state << std::endl;
}
*/

Heuristics::Heuristics(const Heuristics& other)
: _state(other._state)
{
    // _scores = other._scores;
    _max_score = other._max_score;
    _min_score = other._min_score;
    _heuristic = other._heuristic;
}

Heuristics &Heuristics::operator=(const Heuristics& other)
{
	if (this != &other)
    {
        // _scores = other._scores;
        _max_score = other._max_score;
        _min_score = other._min_score;
        _heuristic = other._heuristic;
    }
	return *this;
}

Heuristics::~Heuristics() {}

void Heuristics::set_masks(int mask_size, int board_sqrt)
{
    Heuristics::_masks = Mask(mask_size, board_sqrt, true);
}



int Heuristics::check_captures(const BigInt &target, const BigInt &other_target, const size_t &pos, const Mask::inner_map &masks)
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

int Heuristics::get_score(const BigInt &target, const BigInt &edge, const BigInt &other_target, const size_t &pos, const Mask::inner_map &masks)
{
    if ((masks.at('f')[pos][0] & other_target) != 0)
        return check_captures(target, other_target, pos, masks);

    size_t bits = target.bitCount();
    int score = 1 << bits;

    const Mask::mask_vector &vectorized = masks.at('v')[pos];

    for (size_t i = 0; i < 2; i++)
    {
        BigInt sub_target = target & masks.at('s')[pos][i];
        size_t sub_bits = sub_target.bitCount();
        if (bits == sub_bits && bits > 1)
        {
            BigInt target_first = target & vectorized[1];
            BigInt target_last = target & vectorized[5];
            BigInt edge_first;
            BigInt edge_last;
            // if (masks['e'][pos][0] != o)
                edge_first = edge & vectorized[0];
            // else
            //     edge_first = target_first;
            // if (masks['e'][pos][0] != o)
                edge_last = edge & vectorized[6];
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

bool Heuristics::board_eval(int pos, char orientation, bool endgame)
{
    const Mask::inner_map &masks = _masks.at(orientation);

	const BigInt &full_mask = masks.at('f').at(pos)[0];
    if (full_mask == 0)
        return false;
    BigInt target = _state.mystate(true) & full_mask;
    BigInt other_target = _state.otherstate(true) & full_mask;

    if(endgame)
    {
        if (target == full_mask)
        {
            _heuristic = 32;
            return true;
        }
        if (other_target == full_mask)
        {
            _heuristic = -32;
            return true;
        }
        return false;
    }

    BigInt edge = (_state.otherstate(true) & masks.at('e')[pos][0]);
    edge = edge | (_state.mystate(true) & masks.at('e')[pos][0]);

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

int Heuristics::run(bool endgame)
{
    for (size_t pos = 0; pos < _state.size(); pos++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (board_eval(pos, _modes[i], endgame))
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
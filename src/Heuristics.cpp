#include <Heuristics.hpp>

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
    
    _scores = other._scores;
    _heuristic = other._heuristic;
}

Heuristics &Heuristics::operator=(const Heuristics& other)
{
	if (this != &other)
    {
		_board_sqrt = other._board_sqrt;
        _board_size = other._board_size;
        _my_state = other._my_state;
        _other_state = other._other_state;
    
        _scores = other._scores;
        _heuristic = other._heuristic;
    }
	return *this;
}

Heuristics::~Heuristics() {}

void Heuristics::set_masks(int mask_size, int board_sqrt)
{
    Heuristics::_masks = Mask(mask_size, board_sqrt);
}

int Heuristics::get_score(const BigInt &target, const BigInt &edge, const BigInt &other_target, const size_t &pos, Mask::inner_map &masks)
{


    if ((masks["full"][pos][0] & other_target) != 0)
        return 0;
    //         return self.check_captures(target, other_target, pos, masks)
        size_t bits = (target & masks["full"][pos][0]).bitCount();
        int score = 1 << bits;
        BigInt zero = BigInt(0);

        for (size_t i = 0; i < 2; i++)
        {
            BigInt sub_target = target & masks["submask"][pos][i];
            size_t sub_bits = sub_target.bitCount();

            if (bits == sub_bits && bits > 1)
            {
                BigInt target_first = target & _masks.targets(pos);
                BigInt target_last = target & _masks.targets(pos + _mask_size-1);
                BigInt edge_first;
                BigInt edge_last;
                if (masks["edge"][pos][0] != 0)
                    edge_first = edge & _masks.targets(masks["edge"][pos][0]);
                else
                    edge_first = target_first;
                if (masks["edge"][pos][0] != 0)
                    edge_last = edge & _masks.targets(masks["edge"][pos][0]);
                else
                    edge_last = target_last;
                if ((target_first && edge_first == 0)
                || (target_last && edge_last == 0)
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
        
        if (masks["full"][pos][0] == 0)
            return false;
        BigInt edge = (_other_state & masks["edge"][pos][0]);
        edge = edge | (_my_state & masks["edge"][pos][0]);

        BigInt target = (_my_state & masks["full"][pos][0]);
        if (target == masks["full"][pos][0])
        {
            _heuristic = 32;
            return true;
        }
        BigInt other_target = (_other_state & masks["full"][pos][0]);
        if (other_target == masks["full"][pos][0])
        {
            _heuristic = -32;
            return true;
        }
        
        int score = get_score(target, edge, other_target, pos, masks);
        if (score == 32)
        {
            _heuristic = score;
            return true;
        }
        _scores.push_back(score);

        score = get_score(other_target, edge, target, pos, masks) * -1;
        if (score == -32)
        {
            _heuristic = score;
            return true;
        }
        _scores.push_back(score);
        return false;
}
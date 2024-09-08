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



size_t Heuristics::is_capturable(const size_t &pos, const Mask::inner_map &masks, bool maximizing)
{
    const Mask::mask_vector &vectorized = masks.at(VECTOR)[pos];
    size_t points;
    size_t greater_points = 0;
    for (size_t i = 1; i <= 5; i++)
    {
        points = _state.check_capture(vectorized[i], maximizing);
        if(points > greater_points)
            greater_points = points;
    }
    return greater_points;
}

int Heuristics::get_score(const BigInt &target, const BigInt &edge, const BigInt &other_target, const size_t &pos, const Mask::inner_map &masks)
{
    if ((masks.at(FULL)[pos][0] & other_target) != 0)
        return 0;

    size_t bits = target.bitCount();
    if (bits < 1)
        return 0;
    if (bits == 1)
        return 2;
    int score = 1 << bits;

    const Mask::mask_vector &vectorized = masks.at(VECTOR)[pos];

    BigInt target_first = target & vectorized[1];
    BigInt target_last = target & vectorized[5];
    BigInt edge_first = edge & vectorized[0];
    BigInt  edge_last = edge & vectorized[6];
    for (size_t i = 0; i < 2; i++)
    {
        BigInt sub_target = target & masks.at(SUBMASK)[pos][i];
        size_t sub_bits = sub_target.bitCount();
        if (bits == sub_bits)
        {
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
//ones = 2
//closed two = 4
//open two = 7
//closed three = 8
//open three = 11
//closed four = 16
//open four = 19
//fives = 32
void Heuristics::_set_points(bool my, int points)
{
    if (points == 0)
        return ;
    std::string str = "my_";
    if (!(my))
        str = "ot_";
    if (points == 32)
        str+="five";
    else if (points == 2)
        str+="one";
    else if (points == 4)
        str+="ctwo";
    else if (points == 7)
        str+="otwo";
    else if (points == 8)
        str+="cthree";
    else if (points == 11)
        str+="othree";
    else if (points == 16)
        str+="cfour";
    else if (points == 19)
        str+="ofour";
    else
        std::cout << "What is this value: " << points << std::endl;
    _points[str]++;
}

bool Heuristics::board_eval(int pos, char orientation, bool endgame)
{
    const Mask::inner_map &masks = _masks.at(orientation);

	const BigInt &full_mask = masks.at(FULL).at(pos)[0];
    if (full_mask == 0)
        return false;
    BigInt target = _state.mystate(true) & full_mask;
    BigInt other_target = _state.otherstate(true) & full_mask;


    if (target == full_mask)
    {
        if (endgame)
        {
            _heuristic = 60000;
            return true;
        }
        _set_points(true, 32);
        
    }
    if (other_target == full_mask)
    {
        if (endgame)
        {
            _heuristic = -60000;
            return true;
        }
        _set_points(false, 32);
    }
    if(endgame)
        return false;

    BigInt edge = (_state.otherstate(true) & masks.at(EDGE)[pos][0]);
    edge = edge | (_state.mystate(true) & masks.at(EDGE)[pos][0]);

    int score = get_score(target, edge, other_target, pos, masks);
    if (score > _max_score)
        _max_score = score;
    if (score < _max_score)
    {
        _set_points(true, _max_score);
        _max_score = 0;
    }
    if(_state.check_capture(_state.mystate(true), _state.otherstate(true), pos) != 0)
        _points["my_potential_captures"]++;
    score = get_score(other_target, edge, target, pos, masks);
    if (score > _min_score)
        _min_score = score;
    if (score < _min_score)
    {
        _set_points(false, _min_score);
        _min_score = 0;
    }
    if(_state.check_capture(_state.otherstate(true), _state.mystate(true), pos) != 0)
        _points["ot_potential_captures"]++;
    return false;
}

int Heuristics::run()
{
    for (int pos = 0; pos < _state.size(); pos++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (board_eval(pos, _modes[i], false))
                return _heuristic;
        }
    }
    //for (std::map<std::string, int>::iterator it = _points.begin(); it != _points.end(); it++)
    //    std::cout << it->first << ": " << it->second << std::endl;

    _heuristic = 60000 * (_points["my_five"] - _points["ot_five"])
                + 4800 * (_points["my_ofour"] - _points["ot_ofour"])
                + 500 * (_points["my_cfour"] - _points["ot_cfour"])
                + 500 * (_points["my_othree"] - _points["ot_othree"])
                + 200 * (_points["my_cthree"] - _points["ot_cthree"])
                + 50 * (_points["my_otwo"] - _points["ot_otwo"])
                + 10 * (_points["my_ctwo"] - _points["ot_ctwo"]);
    if (_state.with_captures())
        _heuristic +=  500 * (_points["my_potential_captures"] * _state.maxi_captures()  - _points["ot_potential_captures"] *  _state.mini_captures());
    return _heuristic;
}

int Heuristics::endgame(size_t pos)
{
    for (size_t i = 0; i < 4; i++)
    {
        if (board_eval(pos, _modes[i], true))
            return _heuristic;
    }
    return 0;
}

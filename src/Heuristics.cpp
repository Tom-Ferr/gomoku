#include <Heuristics.hpp>

Mask Heuristics::_masks = Mask();

Heuristics::Heuristics(BoardState &state)
: _state(state)
{
    _my_scores[HORIZONTAL] = std::vector<int>(_state.size(), 0);
    _my_scores[VERTICAL] = std::vector<int>(_state.size(), 0);
    _my_scores[CRESCENDO] = std::vector<int>(_state.size(), 0);
    _my_scores[DECRESCENDO] = std::vector<int>(_state.size(), 0);
    _other_scores[HORIZONTAL] = std::vector<int>(_state.size(), 0);
    _other_scores[VERTICAL] = std::vector<int>(_state.size(), 0);
    _other_scores[CRESCENDO] = std::vector<int>(_state.size(), 0);
    _other_scores[DECRESCENDO] = std::vector<int>(_state.size(), 0);
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
    if (_state.with_captures() == false)
        return 0;
    const Mask::mask_vector &vectorized = masks.at(VECTOR)[pos];
    size_t points;
    size_t capturable_points = 0;
    for (size_t i = 1; i <= 5; i++)
    {
        points = _state.check_capture(vectorized[i], maximizing);
        if(points != 0)
            capturable_points++;
    }
    return capturable_points;
}

int Heuristics::get_score(const BigInt &target, const BigInt &edge, const BigInt &other_target, const size_t &pos, const Mask::inner_map &masks)
{
    if ((masks.at(FULL)[pos][0] & other_target) != 0)
        return 0;

    size_t bits = target.bitCount();
    if (bits < 2)
        return 0;
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
    else if (points == 21)
        str+="cfive1";
    else if (points == 22)
        str+="cfive2";
    else if (points == 23)
        str+="cfive3";
    else if (points == 24 || points == 25)
        str+="cfive4";
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

void Heuristics::to_compute(bool my, std::vector<int> &target, size_t pos, const Mask::inner_map &masks)
{
    const Mask::mask_vector &vectorized = masks.at(VECTOR)[pos];
    size_t prev = vectorized[0].pos();
    size_t last = vectorized[5].pos();
    size_t edge = vectorized[6].pos();
    int target_score = target[pos];
    int prev_score = target[prev];

    if (target_score < prev_score && target_score == 0)
        _set_points(my, prev_score);
    else if (target_score < prev_score)
        target[pos] = prev_score;
    else if((target_score == prev_score && target_score == 32) || last == edge )
        _set_points(my, target_score);

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
        int points = is_capturable(pos, masks, true);
        if (points == 0)
        {
            if (endgame)
            {
                _points["my_five"]++;
                return true;
            }
        }
        else
        {
            if (endgame)
                return false;
            _set_points(true, 20 + points);
            _points["my_five"]--;
        }
    }
    if (other_target == full_mask)
    {
        int points = is_capturable(pos, masks, false);
        if (points == 0)
        {
            if (endgame)
            {
                _points["ot_five"]++;
                return true;
            }
        }
        else
        {
            if (endgame)
                return false;
            _set_points(false, 20 + points);
            _points["ot_five"]--;
        }
    }
    if(endgame)
        return false;

    BigInt edge = (_state.otherstate(true) & masks.at(EDGE)[pos][0]);
    edge = edge | (_state.mystate(true) & masks.at(EDGE)[pos][0]);

    _my_scores[orientation][pos] = get_score(target, edge, other_target, pos, masks);
    to_compute(true, _my_scores[orientation], pos, masks);

    if(_state.check_capture(_state.mystate(true), _state.otherstate(true), pos, orientation) != 0)
        _points["my_potential_captures"]++;

    _other_scores[orientation][pos] = get_score(other_target, edge, target, pos, masks);
    to_compute(false, _other_scores[orientation], pos, masks);

    if(_state.check_capture(_state.otherstate(true), _state.mystate(true), pos, orientation) != 0)
        _points["ot_potential_captures"]++;
    return false;
}

int Heuristics::run()
{
    for (int pos = 0; pos < _state.size(); pos++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            board_eval(pos, _modes[i], false);
        }
    }

    _heuristic = 80000 * (_points["my_five"] - _points["ot_five"])
                + 10000 * (_points["my_ofour"] - _points["ot_ofour"])
                + 500 * (_points["my_cfour"] - _points["ot_cfour"])
                + 500 * (_points["my_othree"] - _points["ot_othree"])
                + 200 * (_points["my_cthree"] - _points["ot_cthree"])
                + 50 * (_points["my_otwo"] - _points["ot_otwo"])
                + 10 * (_points["my_ctwo"] - _points["ot_ctwo"]);
    if (_state.with_captures())
        _heuristic +=   10000 * (_points["my_cfive1"] - _points["ot_cfive1"])
                        + 500 * (_points["my_cfive2"] - _points["ot_cfive2"])
                        + 50  * (_points["my_cfive3"] - _points["ot_cfive3"])
                        + 500 * ((_points["my_potential_captures"] / 2) * _state.maxi_captures()  - (_points["ot_potential_captures"] / 2) *  _state.mini_captures());
    return _heuristic;
}

bool Heuristics::endgame(size_t pos)
{
    for (size_t i = 0; i < 4; i++)
    {
        if (board_eval(pos, _modes[i], true))
            return true;
    }
    return false;
}

void Heuristics::describe_heuristic() const
{
    std::map<std::string, int>::const_iterator it = _points.begin();
    for (; it != _points.end(); it++)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    
}

bool Heuristics::maxi_wins()
{
    if (_points["my_five"] != 0)    
        return true;
    return false;
}

bool Heuristics::mini_wins()
{
    if (_points["ot_five"] != 0)    
        return true;
    return false;
}

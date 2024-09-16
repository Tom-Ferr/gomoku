#include <Heuristics.hpp>

Mask Heuristics::_masks = Mask();
std::unordered_map<std::string, int> Heuristics::_hashes;

Heuristics::Heuristics(BoardState &state)
: _state(state)
{
	_my_scores = std::vector<std::vector<int> >(4, std::vector<int>(_state.size(), 0));
	_other_scores = std::vector<std::vector<int> >(4, std::vector<int>(_state.size(), 0));
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

int Heuristics::get_score(const BigInt &target, const BigInt &edge, const BigInt &other_target, const size_t &pos, const Mask::inner_map &masks, const BigInt &fullmask)
{
	if (!BigInt::and_equal_zero(other_target, fullmask))
		return 0;

    size_t bits = target.bitCount();
    if (bits < 2)
        return 0;
    int score = 1 << bits;

    const Mask::mask_vector &vectorized = masks.at(VECTOR)[pos];
	const Mask::mask_vector &submask = masks.at(SUBMASK)[pos];
    _target_first = target & vectorized[1];
    _target_last = target & vectorized[5];
    _edge_first = edge & vectorized[0];
    _edge_last = edge & vectorized[6];
    for (size_t i = 0; i < 2; i++)
    {
        size_t sub_bits = (target & submask[i]).bitCount();
        if (bits == sub_bits)
        {
            if ((_target_first != 0 && _edge_first == 0)
            || (_target_last != 0 && _edge_last == 0)
            || ((_target_first == 0 && _target_last == 0) && (_edge_first == 0 || _edge_last == 0))
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
    size_t prev = vectorized[0].get_maskpos();
    size_t last = vectorized[5].get_maskpos();
    size_t edge = vectorized[6].get_maskpos();
    int target_score = target[pos];
    int prev_score = target[prev];

    if ((target_score < prev_score && prev_score != 32) && target_score == 0)
        _set_points(my, prev_score);
    else if (target_score < prev_score && prev_score != 32)
        target[pos] = prev_score;
    else if(target_score == 32 || last == edge )
        _set_points(my, target_score);
}

bool Heuristics::board_eval(int pos, char orientation, bool endgame)
{
    const Mask::inner_map &masks = _masks.at(orientation);
	std::vector<int> &my_scores = _my_scores[orientation];
	std::vector<int> &other_scores = _other_scores[orientation];
	int &my_score = my_scores[pos];
	int &other_score = other_scores[pos];

	const BigInt &full_mask = masks.at(FULL).at(pos)[0];
    if (full_mask == 0)
        return false;
    _target = _state.mystate(true) & full_mask;
    _other_target = _state.otherstate(true) & full_mask;

    if (_target == full_mask)
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
    if (_other_target == full_mask)
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

    _edge = BigInt::masked_bitwise_or(_state.otherstate(true), _state.mystate(true), masks.at(EDGE)[pos][0]);

    my_score = get_score(_target, _edge, _other_target, pos, masks, full_mask);
    to_compute(true, my_scores, pos, masks);

    other_score = get_score(_other_target, _edge, _target, pos, masks, full_mask);
    to_compute(false, other_scores, pos, masks);

    int caps =_state.check_capture(_state.mystate(true), _state.otherstate(true), pos, orientation);
    if (caps > 0)
        _points["my_potential_captures"]++;
    else if (caps < 0)
        _points["ot_potential_captures"]++;

    // if(_state.check_capture(_state.otherstate(true), _state.mystate(true), pos, orientation) != 0)
    //     _points["ot_potential_captures"]++;
    return false;
}

int Heuristics::run(bool endgame)
{

	//std::string hash = _state.hash();
	(void)endgame;
	//if (!endgame)
	//{
	//	std::unordered_map<std::string, int>::const_iterator it = _hashes.find(hash);
	//	if (it != _hashes.end())
	//		return it->second;
	//}

	BigInt expanded = _state.expanded_free() | _state.mystate(true) | _state.otherstate(true);
	bool expanded_bit; (void)expanded;(void)expanded_bit;

    for (int pos = 0; pos < _state.size(); pos++)
    {
		expanded_bit = expanded.get_bit(pos);
		if (expanded_bit)
		{
			for (size_t i = 0; i < 4; i++)
			{
				board_eval(pos, _modes[i], false);
			}
		}
    }

    _heuristic = 80000 * (_points["my_five"] - _points["ot_five"])
                + 10000 * (_points["my_ofour"] - (_points["ot_ofour"] * 2))
                + 550 * (_points["my_cfour"] - _points["ot_cfour"])
                + 500 * (_points["my_othree"] - _points["ot_othree"])
                + 200 * (_points["my_cthree"] - _points["ot_cthree"])
                + 50 * (_points["my_otwo"] - _points["ot_otwo"])
                + 10 * (_points["my_ctwo"] - _points["ot_ctwo"]);
    if (_state.with_captures())
        _heuristic +=   10000 * (_points["my_cfive1"] - _points["ot_cfive1"])
                        + 500 * (_points["my_cfive2"] - _points["ot_cfive2"])
                        + 50  * (_points["my_cfive3"] - _points["ot_cfive3"])
                        + 500 * ((_points["my_potential_captures"]) * _state.maxi_captures()  - (_points["ot_potential_captures"]) *  _state.mini_captures());
    //_hashes[hash] = _heuristic;
//	std::cout << "Hash:" << hash << std::endl;

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

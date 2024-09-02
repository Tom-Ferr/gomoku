#include "BoardState.hpp"

BigInt BoardState::mask = BigInt(0);
BigInt BoardState::leftmask = BigInt(0);
BigInt BoardState::rightmask = BigInt(0);
Mask BoardState::_masks = Mask();
Mask BoardState::_capturable_masks = Mask();

BoardState::BoardState(int _sqrt)
:
	_maximizing(true),
	_size(_sqrt * _sqrt),
	_sqrt(_sqrt)
{
	if (BoardState::mask == BigInt(0))
	{
		BoardState::mask = (BigInt(1) << _size) - 1;
		BoardState::leftmask = mask;
		BoardState::rightmask = mask;
		for (int i = 0; i < _sqrt; i++)
		{
			BoardState::rightmask.clear_bit(i * _sqrt);
			BoardState::leftmask.clear_bit((i + 1) * _sqrt - 1);
		}
	}
	_mystate = BoardState::mask;
	_otherstate = BoardState::mask;
	_inv_mystate = BigInt(0);
	_inv_otherstate = BigInt(0);
	_totalboard = BoardState::mask;
}

BoardState::BoardState(const BoardState& other)
: _maximizing(other._maximizing), _capture_move(other._capture_move), _size(other._size), _sqrt(other._sqrt),  _maxi_captures(other._maxi_captures), _mini_captures(other._mini_captures),
_move(other._move),
_mystate(other._mystate), _otherstate(other._otherstate),
_inv_mystate(other._inv_mystate), _inv_otherstate(other._inv_otherstate),
_totalboard(other._totalboard)
{ }

BoardState::BoardState(BoardState&& other) noexcept
: _maximizing(other._maximizing), _capture_move(other._capture_move), _size(other._size), _sqrt(other._sqrt), _maxi_captures(other._maxi_captures), _mini_captures(other._mini_captures),
_move(other._move), _mystate(std::move(other._mystate)),
_otherstate(std::move(other._otherstate)),
_inv_mystate(other._inv_mystate), _inv_otherstate(other._inv_otherstate),
_totalboard(other._totalboard)
{ }

BoardState::BoardState(const BoardState& other, BigInt move)
: _maximizing(!other._maximizing), _capture_move(other._capture_move), _size(other._size), _sqrt(other._sqrt), _maxi_captures(other._maxi_captures), _mini_captures(other._mini_captures),
_move(move), _mystate(other._mystate), _otherstate(other._otherstate),
_inv_mystate(other._inv_mystate),
_inv_otherstate(other._inv_otherstate),
_totalboard(other._totalboard)
{
	_capture_move = false;
	size_t pos = _move.pos();
	check_capture(pos, _maximizing);
	applymove(_move, !_maximizing);
}

void BoardState::applymove(BigInt move, bool mystate)
{
	if (mystate)
	{
		_mystate = _mystate ^ move;
		_inv_mystate = (~_mystate) & BoardState::mask;
	}
	else
	{
		_otherstate = _otherstate ^ move;
		_inv_otherstate = (~_otherstate) & BoardState::mask;
	}
	_totalboard = _totalboard ^ move;
}

void BoardState::applymove(size_t pos, bool mystate)
{
	applymove(BigInt(1) << pos, mystate);
}

BoardState::~BoardState() { }

void BoardState::set_masks(int mask_size, int board_sqrt, bool mirrored)
{
	if (mirrored)
    	BoardState::_masks = Mask(mask_size, board_sqrt, false, true);
	else
		BoardState::_capturable_masks = Mask(mask_size, board_sqrt, false, false);
}

void BoardState::check_capture(size_t pos, bool maximizing)
{
	BigInt* rival;
	BigInt* self;
	int *counter;

	if (!maximizing == true)
	{
		self = &_inv_mystate;
		rival = &_inv_otherstate;
		counter = &_maxi_captures;
	}
	else
	{
		self = &_inv_otherstate;
		rival = &_inv_mystate;
		counter = &_mini_captures;
	}
	for (size_t i = 0; i < 4; i++)
    {
    	const Mask::inner_map &masks = BoardState::_masks.at(_modes[i]);
		const Mask::variations_vector &mid_vec = masks.at(MIDDLE);
		const Mask::variations_vector &edge_vec = masks.at(EDGE);
		Mask::mask_vector::const_iterator mid_mask = mid_vec[pos].begin();
		Mask::mask_vector::const_iterator end = mid_vec[pos].end();
		Mask::mask_vector::const_iterator edge_mask = edge_vec[pos].begin();

		for (; mid_mask != end; mid_mask++, edge_mask++)
		{
			if ((*rival & *mid_mask) != *mid_mask )
				continue ;
			if ((*self & *edge_mask) == 0)
				continue;

			(*counter)++;
			_capture_move = true;
			applymove(*mid_mask, maximizing);
		}
    }
}

size_t BoardState::check_capture(const BigInt &self, const BigInt &rival, const size_t &pos)
{
	size_t points = 0;

	
	for (size_t i = 0; i < 4; i++)
    {
    	const Mask::inner_map &masks = BoardState::_masks.at(_modes[i]);
		const Mask::variations_vector &mid_vec = masks.at(MIDDLE);
		const Mask::variations_vector &edge_vec = masks.at(EDGE);
		Mask::mask_vector::const_iterator mid_mask = mid_vec[pos].begin();
		Mask::mask_vector::const_iterator end = mid_vec[pos].end();
		Mask::mask_vector::const_iterator edge_mask = edge_vec[pos].begin();

		for (; mid_mask != end; mid_mask++, edge_mask++)
		{
			if ((rival & *mid_mask) != *mid_mask)
				continue ;
			if ((self & *edge_mask) == 0)
				continue ;
			points++;
		}
    }
	return points;
}

size_t BoardState::check_capture(const BigInt &target, bool maximizing)
{
	BigInt* rival;
	BigInt* self;
	size_t pos = target.pos();
	size_t points = 0;

	if (maximizing == true)
	{
		self = &_inv_mystate;
		rival = &_inv_otherstate;
	}
	else
	{
		self = &_inv_otherstate;
		rival = &_inv_mystate;
	}
	for (size_t i = 0; i < 4; i++)
    {
    	const Mask::inner_map &masks = BoardState::_capturable_masks.at(_modes[i]);
		const Mask::variations_vector &mid_vec = masks.at(MIDDLE);
		const Mask::variations_vector &edge_vec = masks.at(EDGE);
		Mask::mask_vector::const_iterator mid_mask = mid_vec[pos].begin();
		Mask::mask_vector::const_iterator end = mid_vec[pos].end();
		Mask::mask_vector::const_iterator edge_mask = edge_vec[pos].begin();

		for (; mid_mask != end; mid_mask++, edge_mask++)
		{
			if ((*self & *mid_mask) != *mid_mask )
				continue ;
			if ((*rival & *edge_mask) == 0)
				continue;
			if ((*rival & *edge_mask) == *edge_mask)
				continue;
			points++;
		}
    }
	return points;
}
BigInt const &BoardState::mystate(bool inverted) const
{
	if (inverted)
		return _inv_mystate;
	return _mystate;
}

BigInt const &BoardState::otherstate(bool inverted) const
{
	if (inverted)
		return _inv_otherstate;
	return _otherstate;
}

BigInt const &BoardState::totalboard() const
{
	return _totalboard;
}

int const &BoardState::size() const
{
	return _size;
}

int const &BoardState::sqrt() const
{
	return _sqrt;
}

int const &BoardState::maxi_captures() const
{
	return _maxi_captures;
}

int const &BoardState::mini_captures() const
{
	return _mini_captures;
}

bool const &BoardState::is_capture() const
{
	return _capture_move;
}

BigInt const &BoardState::move() const
{
	return _move;
}

void BoardState::increment_captures(bool turn)
{
	if (turn)
		_maxi_captures++;
	else
		_mini_captures++;
}

void BoardState::increment_captures(bool turn, size_t points)
{
	if (turn)
		_maxi_captures += points;
	else
		_mini_captures += points;
}

void BoardState::swap_states()
{
	BigInt temp = _mystate;
	_mystate = _otherstate;
	_otherstate = temp;
	temp = _inv_mystate;
	_inv_mystate = _inv_otherstate;
	_inv_otherstate = temp;
	_maximizing = true;

	int tmp = _maxi_captures;
	_maxi_captures = _mini_captures;
	_mini_captures = tmp;
}

BigInt BoardState::expanded_free() const
{
	BigInt e[8];
	BigInt t = ~totalboard() & mask;
	e[0] = ((t & BoardState::rightmask) >> 1) & BoardState::mask;
	e[1] = ((t & BoardState::leftmask) << 1) & BoardState::mask;
	e[2] = (t << _sqrt) & BoardState::mask;
	e[3] = (t >> _sqrt) & BoardState::mask;
	e[4] = (e[0] << _sqrt) & BoardState::mask;
	e[5] = (e[1] << _sqrt) & BoardState::mask;
	e[6] = (e[0] >> _sqrt) & BoardState::mask;
	e[7] = (e[1] >> _sqrt) & BoardState::mask;
	return ((e[0] | e[1] | e[2] | e[3] | e[4] | e[5] | e[6] | e[7])
				& totalboard() & mask);
}

void BoardState::print()
{
	std::cout << "\033[2J\033[1;1H";

	for (int y = _sqrt - 1; y >= 0; y--)
	{
		for (int x = _sqrt - 1; x >= 0; x--)
		{
			if (!_mystate.get_bit(y * _sqrt + x))
				std::cout << "\033[1;36m X \033[0m";
			else if (!_otherstate.get_bit(y * _sqrt + x))
				std::cout << "\033[1;35m 0 \033[0m";
			else
				std::cout << "\033[1;33m . \033[0m";
			if (x == 0)
				std::cout << std::endl;
		}
	}
}

BoardState& BoardState::operator=(const BoardState& other)
{
	if (this != &other)
	{
		_maximizing = other._maximizing;
		_size = other._size;
		_sqrt = other._sqrt;
		_move = other._move;
		_mystate = other._mystate;
		_otherstate = other._otherstate;
		_totalboard = other._totalboard;
		_inv_mystate = other._mystate;
		_inv_otherstate = other._inv_otherstate;
		_maxi_captures = other._maxi_captures;
		_mini_captures = other._mini_captures;
		_capture_move = other._capture_move;
	}
	return *this;
}

bool const &BoardState::maximizing() const
{
	return _maximizing;
}

std::ostream &operator<<(std::ostream &os, const BoardState &bs)
{
	//os << "Mask: " << BoardState::mask << std::endl;
	//os << "Left Mask: " << BoardState::leftmask << std::endl;
	//os << "Right Mask: " << BoardState::rightmask << std::endl;
	os << "_maximizing: " << bs.maximizing() << std::endl;
	os << "_size: " << bs.size() << std::endl;
	os << "_sqrt: " << bs.sqrt() << std::endl;
	os << "_mystate: " << bs.mystate() << std::endl;
	os << "_otherstate: " << bs.otherstate() << std::endl;
	os << "_totalboard: " << bs.totalboard() << std::endl;
	BigInt freepos = bs.expanded_free();
	os << "expanded_free: " << freepos << std::endl;
	os << "possible moves: " << std::endl;
	for (size_t i = 0; i < freepos.size(); i++)
	{
		if (freepos.get_bit(i))
			std::cout << "\t" << i;
	}
	std::cout  << std::endl;
	return os;
}
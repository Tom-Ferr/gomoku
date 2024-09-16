#include <Free_Three_Checker.hpp>

Mask Free_Three_Checker::_masks = Mask();
BigInt Free_Three_Checker::_static_state = BigInt();

Free_Three_Checker::Free_Three_Checker(BoardState &state)
: _state(state) {}

Free_Three_Checker::Free_Three_Checker(const Free_Three_Checker& other)
: _state(other._state)
{}

Free_Three_Checker &Free_Three_Checker::operator=(const Free_Three_Checker& other)
{
	(void)other;
	return *this;
}

Free_Three_Checker::~Free_Three_Checker(){};

bool Free_Three_Checker::check(int pos, char orientation, bool turn)
{
	const Mask::inner_map &masks = Free_Three_Checker::_masks.at(orientation);
	const Mask::variations_vector &full_vec = masks.at(FULL);
	Mask::mask_vector::const_iterator full_mask = full_vec[pos].begin();
	Mask::mask_vector::const_iterator end = full_vec[pos].end();
	Mask::mask_vector::const_iterator mid_mask = masks.at(MIDDLE)[pos].begin();
	Mask::mask_vector::const_iterator edge_mask = masks.at(EDGE)[pos].begin();

	for (; full_mask != end; full_mask++, mid_mask++, edge_mask++)
	{
		if (((_state.maximizing() && turn) && (BigInt::bi_and(_state.otherstate(true), *full_mask)) != 0)
				|| ((!_state.maximizing() || !turn) && (BigInt::bi_and(_state.mystate(true), *full_mask)) != 0))
			continue;

		if (_state.maximizing() && turn)
			_static_state = BigInt::bi_and(_state.mystate(false), *full_mask);
		else
			_static_state = BigInt::bi_and(_state.otherstate(false), *full_mask);
		if((BigInt::bi_and(_static_state, *edge_mask)) == *edge_mask)
			if ((BigInt::bi_and(*mid_mask, _static_state)).bitCount() == 2)
				return true;
	}
	return false;
}

bool Free_Three_Checker::is_free_three(int pos, bool turn)
{
	char modes[4] = {HORIZONTAL, VERTICAL, CRESCENDO, DECRESCENDO};
	int c = 0;

	for (size_t i = 0; i < 4; i++)
	{
		if(check(pos, modes[i], turn))
			c++;
		if(c == 2)
			return true;
	}
	return false;
}

/*
bool Free_Three_Checker::check(int pos, char orientation)
{
	const Mask::inner_map &masks = Free_Three_Checker::_masks.at(orientation);
	// BigInt target = _state.mystate(true) & Mask::targets(pos);

	const Mask::variations_vector &full_vec = masks.at(FULL);
	Mask::mask_vector::const_iterator full_mask = full_vec[pos].begin();
	Mask::mask_vector::const_iterator end = full_vec[pos].end();

	Mask::mask_vector::const_iterator mid_mask = masks.at(MIDDLE)[pos].begin();
	Mask::mask_vector::const_iterator edge_mask = masks.at(EDGE)[pos].begin();

	for (; full_mask != end; full_mask++, mid_mask++, edge_mask++)
	{
		if((_state.otherstate(true) & *full_mask) != 0)
			continue;
		BigInt state = _state.mystate(true) & *full_mask;
		if((state & *edge_mask) == *edge_mask)
			if ((*mid_mask & state).bitCount() == 2)
				return true;
	}
	return false;
}


 bool Free_Three_Checker::check(int pos)
 {
	const Mask::inner_map &masks = Free_Three_Checker::_masks.at(SUPERPOSITION);
	const Mask::variations_vector &full_vec = masks.at(FULL);

	Mask::mask_vector::const_iterator full_masks = full_vec[pos].begin();
	Mask::mask_vector::const_iterator end = full_vec[pos].end();

	Mask::mask_vector::const_iterator mid_masks = masks.at(MIDDLE)[pos].begin();
	Mask::mask_vector::const_iterator other_masks = masks.at(OUTER)[pos].begin();
	Mask::mask_vector::const_iterator edge_masks = masks.at(EDGE)[pos].begin();


	for (; full_masks != end; full_masks++, edge_masks++, mid_masks++, other_masks++)
	{
		 if ((_state.otherstate(true) & *full_masks) != 0)
			 continue;

		 if ((_state.mystate(true) & *edge_masks) != 0)
			 continue;

		 if((_state.mystate(true) & *mid_masks) == *mid_masks)
			 continue;

		 if((_state.mystate(true) & *other_masks) == *other_masks)
			 continue;

		 if ((_state.mystate(true) & *full_masks).bitCount() != 4)
			 continue;

		 return true;
	}
	return false;
 }
*/
void Free_Three_Checker::set_masks(int mask_size, int board_sqrt)
{
	_masks = Mask(mask_size, board_sqrt);
}
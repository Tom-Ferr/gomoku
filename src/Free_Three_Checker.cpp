#include <Free_Three_Checker.hpp>

Mask Free_Three_Checker::_masks = Mask();

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

bool Free_Three_Checker::check(int pos, char orientation)
{
    const Mask::inner_map &masks = Free_Three_Checker::_masks.at(orientation);
    BigInt target = _state.mystate(true) & Free_Three_Checker::_masks.targets(pos);

    const Mask::variations_vector &full_vec = masks.at('f');
    Mask::mask_vector::const_iterator full_mask = full_vec[pos].begin();
    Mask::mask_vector::const_iterator end = full_vec[pos].end();

    Mask::mask_vector::const_iterator mid_mask = masks.at('m')[pos].begin();
    Mask::mask_vector::const_iterator edge_mask = masks.at('e')[pos].begin();

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
	const Mask::inner_map &masks = Free_Three_Checker::_masks.at('s');
    const Mask::variations_vector &full_vec = masks.at('f');

    Mask::mask_vector::const_iterator full_masks = full_vec[pos].begin();
    Mask::mask_vector::const_iterator end = full_vec[pos].end();

    Mask::mask_vector::const_iterator mid_masks = masks.at('m')[pos].begin();
    Mask::mask_vector::const_iterator other_masks = masks.at('o')[pos].begin();
    Mask::mask_vector::const_iterator edge_masks = masks.at('e')[pos].begin();


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

void Free_Three_Checker::set_masks(int mask_size, int board_sqrt)
{
    _masks = Mask(mask_size, board_sqrt);
}
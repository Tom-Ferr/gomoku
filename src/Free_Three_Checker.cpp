#include <Free_Three_Checker.hpp>

Mask Free_Three_Checker::_masks = Mask();

Free_Three_Checker::Free_Three_Checker(int board_sqrt, BigInt my_state, BigInt other_state)
:
_board_sqrt(board_sqrt * board_sqrt),
_board_size(board_sqrt),
_my_state(my_state),
_other_state(~other_state) {};

Free_Three_Checker::Free_Three_Checker(const Free_Three_Checker& other)
{
	_my_state = other._my_state;
	_other_state = other._other_state;
	_board_sqrt = other._board_sqrt;
	_board_size = other._board_size;
}

Free_Three_Checker &Free_Three_Checker::operator=(const Free_Three_Checker& other)
{
	if (this != &other)
    {
		_my_state = other._my_state;
	    _other_state = other._other_state;
	    _board_sqrt = other._board_sqrt;
	    _board_size = other._board_size;
    }
	return *this;
}

Free_Three_Checker::~Free_Three_Checker(){};

bool Free_Three_Checker::check(int pos, char orientation)
{
    Mask::inner_map masks = Free_Three_Checker::_masks[orientation];
    BigInt target = _my_state & Free_Three_Checker::_masks.targets(pos);
    Mask::mask_vector::iterator full_mask = masks["full"][pos].begin();
    Mask::mask_vector::iterator mid_mask = masks["middle"][pos].begin();
    Mask::mask_vector::iterator edge_mask = masks["edge"][pos].begin();
    for (; full_mask != masks["full"][pos].end(); full_mask++, mid_mask++, edge_mask++)
    {
        if((_other_state & *full_mask) != 0)
            continue;
        BigInt state = _my_state & *full_mask;
        if((state & *edge_mask) == *edge_mask)
            if ((*mid_mask & state).bitCount() == 2)
                return true;
    }
    return false;
}

// bool Free_Three_Checker::check(int pos, char orientation)
// {
//    BigInt inv_other = ~_other_state;
//    BigInt inv_my_state = ~_my_state;
//    Mask::mask_vector::iterator full_masks = _masks[orientation]["full"][pos].begin();
//    Mask::mask_vector::iterator mid_masks = _masks[orientation]["middle"][pos].begin();
//    Mask::mask_vector::iterator edge_masks = _masks[orientation]["edge"][pos].begin();

//    for (; full_masks != _masks[orientation]["full"][pos].end(); full_masks++)
//    {
//         if ((inv_other & *full_masks) != 0)
//             continue;
//         if ((inv_my_state & *edge_masks) != 0)
//             continue;
//         if ((inv_my_state & *full_masks).bitCount() != 4)
//             continue;

        
//    }
//    return false;

void Free_Three_Checker::set_masks(int mask_size, int board_sqrt)
{
    _masks = Mask(mask_size, board_sqrt);
}
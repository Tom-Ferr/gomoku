#include <Free_Three_Checker.hpp>

Mask Free_Three_Checker::_masks = Mask();

Free_Three_Checker::Free_Three_Checker(int board_sqrt, BigInt my_state, BigInt other_state)
:
_board_sqrt(board_sqrt * board_sqrt),
_board_size(board_sqrt),
_my_state(my_state),
_other_state(other_state) {};

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
        _masks = other._masks;
    }
	return *this;
}

Free_Three_Checker::~Free_Three_Checker(){};

bool Free_Three_Checker::check(int pos, char orientation)
{
    Mask::inner_map masks = _masks[orientation];
    BigInt target = _my_state & _masks.targets(pos - 1);
    Mask::mask_vector::iterator full_mask = masks["full"][pos-1].begin();
    Mask::mask_vector::iterator mid_mask = masks["middle"][pos-1].begin();
    Mask::mask_vector::iterator edge_mask = masks["edge"][pos-1].begin();
    BigInt zero = BigInt(0);
    for (; full_mask != masks["full"][pos-1].end(); full_mask++, mid_mask++, edge_mask++)
    {
        if((_other_state & *full_mask) != zero)
            continue;
        BigInt state = _my_state & *full_mask;
        if((state & *edge_mask) == *edge_mask)
            if ((*mid_mask & state).bitCount() == 2)
                return true;
    }
    return false;
}

void Free_Three_Checker::set_masks(int mask_size, int board_sqrt)
{
    _masks = Mask(mask_size, board_sqrt);
}
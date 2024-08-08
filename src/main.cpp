#include <gomoku.hpp>
#include <Mask.hpp>

int main (void)
{
	Mask m = Mask(6, 19);
	m.print_mask(m['d']["edge"]);
	return (0);
}

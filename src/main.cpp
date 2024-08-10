#include <gomoku.hpp>
#include <Mask.hpp>

int main (void)
{
	Mask m = Mask(5, 19, false);
	m.print_mask(m['s']["full"]);
	return (0);
}

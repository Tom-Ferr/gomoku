#include <gomoku.hpp>
#include <Mask.hpp>

int main (void)
{
	Mask m = Mask(5, 19, true);
	m.print_mask(m['c']["vectors"]);
	return (0);
}

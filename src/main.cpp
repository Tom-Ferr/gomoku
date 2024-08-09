#include <gomoku.hpp>
#include <Mask.hpp>

int main (void)
{
	Mask m = Mask(5, 19, true);
	m.print_mask(m['d']["submask"]);
	return (0);
}

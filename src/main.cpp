// #include <gomoku.hpp>
// #include <Mask.hpp>

// int main (void)
// {
// 	Mask m = Mask(5, 19, true);
// 	m.print_mask(m['v']["vectors"]);
// 	return (0);
// }

#include <gomoku.hpp>
#include <Node.hpp>
#include <Game.hpp>

int main (void)
{
	Free_Three_Checker::set_masks(6, 19);
	Game g;

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	g.step();
	while (g.step())
		continue;
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Time taken (TOTAL): " << duration.count() << " seconds" << std::endl;
	std::cout << "Node count: " << Node::node_count << std::endl;
	return (0);
}
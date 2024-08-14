#include <gomoku.hpp>
#include <Node.hpp>
#include <Game.hpp>

int main (void)
{
	std::srand(42);
	Free_Three_Checker::set_masks(6, 19);
	Heuristics::set_masks(5, 19);
	Game g;
	g.board().applymove(180, false);
	std::cout << g.board() << std::endl;
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	//g.step();
	while (g.step())
		;
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Time taken (TOTAL): " << duration.count() << " seconds" << std::endl;
	std::cout << "Node count: " << Node::node_count << std::endl;
	return (0);
}
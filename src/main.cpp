#include <gomoku.hpp>
#include <Node.hpp>
#include <Game.hpp>

int main (void)
{
	std::srand(42);
	Free_Three_Checker::set_masks(6, 19);
	Heuristics::set_masks(5, 19);
	Game g;

	/*
	size_t p1[9] = {0, 1, 3, 5, 6, 40, 80, 120, 160};
	size_t p2[10] = {2, 4, 20, 25, 38, 39, 60, 100, 140, 180};
	for (size_t i = 0; i < 9; i++)
			g.board().applymove(p1[i], true);
	for (size_t i = 0; i < 10; i++)
		g.board().applymove(p2[i], false);
	X to play.
	optimal moves would be either 7 (to make 3) or 58 (to prevent 3)
	g.board().print();
	g.step();
	exit(0);
	*/

	std::cout << g.board() << std::endl;
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	bool turn = true; /* indicates the starting player */
	while (g.step(turn))
		turn = !turn;
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Time taken (TOTAL): " << duration.count() << " seconds" << std::endl;
	std::cout << "Node count: " << Node::node_count << std::endl;
	return (0);
}

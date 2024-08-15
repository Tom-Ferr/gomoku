// #include <gomoku.hpp>
// #include <Mask.hpp>
// #include <Heuristics.hpp>

// int main (void)
// {
// 	// Mask m = Mask(5, 19, true);
// 	// m.print_mask(m[CRESCENDO][VECTOR]);
// 	// return 0;
// 	BoardState b = BoardState(19);
// 	Heuristics::set_masks(5, 19);
// 	const char* me = (
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000100010000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000011110000000111"
// 	"0000001000000000000"
// 	"0000001000000000000"
// 	"0000001000000000000"
// 	"0000001000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	);
// 	const char* vs = (
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000011110000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000100010000001000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	"0000000000000000000"
// 	);
// 	BigInt my_state = BigInt(std::string(me), 2);
// 	BigInt other_state = BigInt(std::string(vs), 2);
// 	BigInt test = BigInt(std::string(vs), 2);
// 	b.applymove(my_state, true);
// 	b.applymove(other_state, false);
// 	Heuristics h = Heuristics(b);
// 	int score = h.run();
// 	std::cout << score << '\n';
// 	return (0);
// }

#include <gomoku.hpp>
#include <Node.hpp>
#include <Game.hpp>

int main (void)
{
	std::srand(std::time(nullptr));
	Free_Three_Checker::set_masks(6, 19);
	Heuristics::set_masks(5, 19);
	Game g;

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	while (g.step())
		;
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Time taken (TOTAL): " << duration.count() << " seconds" << std::endl;
	std::cout << "Node count: " << Node::node_count << std::endl;
	return (0);
}
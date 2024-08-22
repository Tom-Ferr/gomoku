#include <gomoku.hpp>
#include <Mask.hpp>
#include <Heuristics.hpp>
#include <Node.hpp>

int main (void)
{
	Mask m = Mask(4, 19, false, true);
	m.print_mask(m[DECRESCENDO][MIDDLE]);
	return 0;
	BoardState b = BoardState(19);
	Heuristics::set_masks(5, 19);
	Free_Three_Checker::set_masks(6, 19);
	const char* me = (
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000001000000000"
    "0000000000000000000"
    "0000000000010000000"
    "0000000000000000000"
    "0000000000011110000"
    "0000000000000000000"
    "0000000000000001000"
    "0000000000000000000"
    "0000000000000000010"
    "0000000000000000010"
    );
    const char* vs = (
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000000000000"
    "0000000000100000000"
    "0000000000000000000"
    "0000000000001000000"
    "0000000000000000000"
    "0000000000000010000"
    "0000000000000000000"
    "0000000000000000100"
    "0000000000000000000"
    "0000000000000000101"
    );
	BigInt my_state = BigInt(std::string(me), 2);
	BigInt other_state = BigInt(std::string(vs), 2);
	BigInt test = BigInt(std::string(vs), 2);
	b.applymove(my_state, true);
	b.applymove(other_state, false);
	std::pair<int, BigInt> result;
	// result = Node(3, INT_MIN, INT_MAX, b).minimax();
    BigInt test_move = BigInt("0100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 2);
    BoardState x = BoardState(b, test_move);
	Heuristics h = Heuristics(x);
	int score = h.run();
	// std::cout << result.first << '\n';
	// std::cout << result.second << '\n';
	std::cout << score << '\n';
	return (0);
}

// #include <gomoku.hpp>
// #include <Node.hpp>
// #include <Game.hpp>

// int main (void)
// {
// 	std::srand(std::time(nullptr));
// 	Free_Three_Checker::set_masks(6, 19);
// 	Heuristics::set_masks(5, 19);
// 	Game g;

// 	/*
// 	size_t p1[9] = {0, 1, 3, 5, 6, 40, 80, 120, 160};
// 	size_t p2[10] = {2, 4, 20, 25, 38, 39, 60, 100, 140, 180};
// 	for (size_t i = 0; i < 9; i++)
// 			g.board().applymove(p1[i], true);
// 	for (size_t i = 0; i < 10; i++)
// 		g.board().applymove(p2[i], false);
// 	X to play.
// 	optimal moves would be either 7 (to make 3) or 58 (to prevent 3)
// 	g.board().print();
// 	g.step();
// 	exit(0);
// 	*/

// 	std::cout << g.board() << std::endl;
// 	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
// 	bool turn = true; /* indicates the starting player */
// 	g.board().applymove(180, false);
// 	while (g.step(turn))
// 		turn = !turn;
// 	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
// 	std::cout << "Time taken (TOTAL): " << duration.count() << " seconds" << std::endl;
// 	std::cout << "Node count: " << Node::node_count << std::endl;
// 	return (0);
// }

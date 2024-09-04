#include <gomoku.hpp>
#include <Mask.hpp>
#include <Heuristics.hpp>
#include <Node.hpp>
#include <Game.hpp>

int main (void)
{
    BoardState::set_masks(4, 19, true);
    BoardState::set_masks(4, 19, false);
    Heuristics::set_masks(5, 19);
    Free_Three_Checker::set_masks(6, 19);
    // Mask m = Mask(4, 19, false, true);
    // m.print_mask(m[VERTICAL][EDGE]);
    // return 0;

  


   const char* me = (
        // "0000000000000000000" 
        // "0000000000000000000" 
        // "0001000000000000000" 
        // "0000000000000000000" 
        // "0000000000000000000" 
        // "0000001000010000000" 
        // "0000001001001000000" 
        // "0000000010111100000" 
        // "0000000001100000000" 
        // "0000000010100000000" 
        // "0000000000111000000" 
        // "0000000000000000000" 
        // "0000000000000000000" 
        // "0000000000000000000" 
        // "0000000000000000000" 
        // "0000000000000000000" 
        // "0000000000000000000" 
        // "0000000000000000000" 
        // "0000000000000000000" 

        "0000000000000000000"
        "0000000000000000000"
        "0000000000000000000"
        "0000000000000000000"
        "0000000000000000000"
        "0000000001000000000"
        "0000000001000000000"
        "0000000001000000000"
        "0000000000101111000"
        "0000000000000001000"
        "0000000000000000000"
        "0000000000000011100"
        "0000000000000000000"
        "0000000000000000000"
        "0000000000000000000"
        "0000000000000000000"
        "0000000000000000000"
        "0000000000000000000"
    );
    const char* vs = (
        "0000000000000000000" 
        "0000000000000000000" 
        "0000000000000000000" 
        "0000000000000000000" 
        "0000000001000000000" 
        "0000000000000000000" 
        "0000000000000000000" 
        "0000000000000000000" 
        "0000000001010000100" 
        "0000000000000000000" 
        "0000000000000000000" 
        "0000000000000100000" 
        "0000000000000000000" 
        "0000000000000000000" 
        "0000000000000000000" 
        "0000000000000000000" 
        "0000000000000000000" 
        "0000000000000000000"

        // "0100000000000000000"
        // "0010000000000000000"
        // "0000000000000000000"
        // "0000100000000000000"
        // "0000010000000000000"
        // "0000000000000000000"
        // "0000000100100000000"
        // "0000000001000010000"
        // "0000000000000000000"
        // "0000000000000000000"
        // "0000000101000000000"
        // "0000000000101000000"
        // "0000000000000000000"
        // "0000000000000000000"
        // "0000000000000000000"
        // "0000000000000000000"
        // "0000000000000000000"
        // "0000000000000000000"
        // "0000000000000000000"
    );

    BigInt my_state = BigInt(std::string(me), 2);
    BigInt other_state = BigInt(std::string(vs), 2);

    Game g;
    g.board().applymove(my_state, true);
    g.board().applymove(other_state, false);
    Heuristics h(g.board());
    int result = h.run();
    // g.board().increment_captures(false, 2);
    g.board().print();
    std::cout << result << std::endl;
    g.step(false);


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
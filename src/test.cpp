#include <gomoku.hpp>
#include <Mask.hpp>
#include <Heuristics.hpp>
#include <Node.hpp>
#include <Game.hpp>

void test(void)
{
    BoardState::set_masks(4, 19, true);
    BoardState::set_masks(4, 19, false);
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
			"0000001000001000000"
			"0000000001000100000"
			"0000000001010010000"
			"0000000000100000000"
			"0000000000000000000"
			"0000000001011000000"
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
			"0000000000000000000"
			"0000000000000000000"
			"0000000000000000000"
			"0000000000000000000"
			"0000000110101000000"
			"0000000010001000000"
			"0000000001011000000"
			"0000000010101000000"
			"0000000000000000000"
			"0000000000000100000"
			"0000000000000000000"
			"0000000000000000000"
			"0000000000000000000"
			"0000000000000000000"
			"0000000000000000000"
	);


    BigInt my_state = BigInt(std::string(me), 2);
    BigInt other_state = BigInt(std::string(vs), 2);

    Game g;
    g.board().applymove(my_state, true);
    g.board().applymove(other_state, false);
	{
		Heuristics h(g.board());
		g.board().print();
		int result = h.run();
		std::cout << "Heuristics:" << result << std::endl;
	}

    g.step(false);

	{
		Heuristics h(g.board());
		g.board().print();
		int result = h.run();
		std::cout << "Heuristics:" << result << std::endl;
	}
}

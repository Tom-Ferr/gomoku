#ifndef NODE_HPP
# define NODE_HPP

# include <gomoku.hpp>
# include <vector>
# include <utility>
# include <random>
# include <Free_Three_Checker.hpp>
# include <Heuristics.hpp>


typedef  int (*comp_func)(int&, int&);

class Node
{

	public:
		static size_t node_count;
		int					_heuristic;

	private:
		static BigInt		_freepos;
		int					_depth;
		int					_alpha;
		int 				_beta;
		BigInt				_move;
		BoardState			_state;
		std::vector<Node> 	_children;
		Free_Three_Checker	_ftc;
		Node();

	public:
		Node(int depth, int alpha, int beta, BoardState state);
		Node(const Node& other);
		~Node();

		Node& operator=(const Node& other);
		std::pair<int, BigInt> minimax();
		std::pair<int, BigInt> alpha_beta_prune(int &x, comp_func f);
		bool possible_moves(std::vector<size_t>& moves);
		bool is_double_free_three(const size_t &pos);
		bool is_valid(const size_t &pos, BigInt &freepos);
};

int max(int &a, int &b);
int min(int &a, int &b);
#endif

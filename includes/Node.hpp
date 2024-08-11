#ifndef NODE_HPP
# define NODE_HPP

# include "gomoku.hpp"
# include <vector>
# include <utility>
# include <random>
# include <Free_Three_Checker.hpp>

class Node
{
	public:
		static size_t node_count;

	private:
		int					_depth;
		int					_alpha;
		int 				_beta;
		BoardState			_state;
		std::vector<Node> 	_children;
		Node();

	public:
		Node(int depth, int alpha, int beta, BoardState state);
		Node(const Node& other);
		~Node();

		Node& operator=(const Node& other);
		std::pair<int, BigInt> minimax();
		std::pair<int, BigInt> alpha_beta_prune(int x, bool maximizing=true);
		std::vector<BigInt> possible_moves();
		bool is_double_free_three(const size_t &pos);
		bool is_valid(const size_t &pos, BigInt &freepos);
};
#endif

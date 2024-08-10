#include <Node.hpp>

size_t Node::node_count = 0;

Node::Node() {}

Node::Node(int depth, int alpha, int beta, BoardState state)
: _depth(depth), _alpha(alpha), _beta(beta), _state(state)
{
	Node::node_count++;
}

Node::Node(const Node& other)
: _depth(other._depth), _alpha(other._alpha), _beta(other._beta), _state(other._state)
{}

Node::~Node() {}

Node& Node::operator=(const Node& other)
{
	if (this != &other)
	{
		_depth = other._depth;
		_alpha = other._alpha;
		_beta = other._beta;
		_state = other._state;
	}
	return *this;
}

std::pair<int, BigInt> Node::minimax()
{
	if (_depth == 0)
	{
		/*
		** calculates all the score here
		*/

		return std::make_pair(std::rand() % 65 - 32, 0);
	}
	if (_state.turn())
		return alpha_beta_prune(_alpha, true);
	return alpha_beta_prune(_beta, false);
}

std::pair<int, BigInt> Node::alpha_beta_prune(int x, bool maximizing)
{
	BigInt best_child = 0;
	std::vector<BigInt> moves = possible_moves();
	for (std::vector<BigInt>::iterator it = moves.begin(); it != moves.end(); it++)
	{
		Node child(_depth - 1, _alpha, _beta, BoardState(_state, *it));
		std::pair<int, BigInt> score = child.minimax();
		x = maximizing ? std::max(x, score.first) : std::min(x, score.first);
		if(x == score.first)
			best_child = *it;
		if (maximizing)
			_alpha = std::max(_alpha, score.first);
		else
			_beta = std::min(_beta, score.first);
		if (_alpha >= _beta)
			break;
	}
	return std::make_pair(x, best_child);
}

std::vector<BigInt> Node::possible_moves()
{
	std::vector<BigInt> moves;
	BigInt freepos = _state.expanded_free();

	for (size_t i = 0; i < freepos.size(); i++)
	{
		if (freepos.get_bit(i) && !check_double_tree(i))
			moves.push_back(BigInt(1) << i);
	}
	return moves;
}

bool Node::check_double_tree(size_t pos)
{
	(void)pos;
	return false;
}

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

bool Node::is_double_free_three(const size_t &pos)
{
	Free_Three_Checker ftc = Free_Three_Checker(_state.sqrt(), _state.mystate(), _state.otherstate());

	char modes[4] = {'h', 'v', 'c', 'd'};
	int c = 0;

	for (size_t i = 0; i < 4; i++)
	{
		if(ftc.check(pos, modes[i]))
			c++;
		if(c == 2)
			return true;
	}
	return false;
}

bool Node::is_valid(const size_t &pos, BigInt &freepos)
{
	if(freepos.get_bit(pos) && is_double_free_three(pos) == false)
		return true;
	return false;
}

std::vector<BigInt> Node::possible_moves()
{
	std::vector<BigInt> moves;
	BigInt freepos = _state.expanded_free();

	for (size_t pos = 0; pos < freepos.size(); pos++)
	{
		if (freepos.get_bit(pos) && is_double_free_three(pos) == false)
			moves.push_back(BigInt(1) << pos);
	}
	return moves;
}

// std::vector<BigInt> Node::possible_moves()
// {
// 	std::vector<BigInt> moves;
// 	BigInt freepos = _state.expanded_free();

// 	for (size_t i = 0; i < freepos.size(); i++)
// 	{
// 		if (freepos.get_bit(i))
// 			moves.push_back(BigInt(1) << i);
// 	}
// 	return moves;
// }
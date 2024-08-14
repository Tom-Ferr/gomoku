#include <Node.hpp>

size_t Node::node_count = 0;

int max(int &a, int &b)
{
	return std::max(a,b);
}

int min(int &a, int &b)
{
	return std::min(a,b);
}

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
		/*comment this line to test with heuristics*/
		return std::make_pair(std::rand() % 65 - 32, 0);

		Heuristics h = Heuristics(_state);
		_heuristic = h.run();
		std::cout << "heuristic" << _heuristic << '\n';
		return std::make_pair(_heuristic, 0);
	}
	if (_state.turn())
		return alpha_beta_prune(_alpha, max);
	return alpha_beta_prune(_beta, min);
}

std::pair<int, BigInt> Node::alpha_beta_prune(int &x, comp_func f)
{
	BigInt best_child = 0;
	std::vector<BigInt> moves;
	if(possible_moves(moves) == false)
		return std::make_pair(_heuristic, _state.move());
	for (std::vector<BigInt>::iterator it = moves.begin(); it != moves.end(); it++)
	{
		Node child(_depth - 1, _alpha, _beta, BoardState(_state, *it));
		std::pair<int, BigInt> score = child.minimax();
		_heuristic = f(x, score.first);
		if(_heuristic == score.first && _heuristic != x)
		{
			x = _heuristic;
			best_child = *it;
		}
		if (_alpha >= _beta)
			break;
	}
	return std::make_pair(x, best_child);
}

bool Node::is_double_free_three(const size_t &pos)
{
	Free_Three_Checker ftc = Free_Three_Checker(_state);
	//uncomment from here

	// if(ftc.check(pos))
	// 	return true;
	// return false;

	// to here to change to the other check method

	char modes[4] = {HORIZONTAL, VERTICAL, CRESCENDO, DECRESCENDO};
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
	if(freepos.get_bit(pos) && !is_double_free_three(pos))
		return true;
	return false;
}

bool Node::possible_moves(std::vector<BigInt>& moves)
{
	BigInt freepos = _state.expanded_free();

	//Heuristics h = Heuristics(_state);
	for (size_t pos = 0; pos < _state.size(); pos++)
	{
		//{
			//_heuristic = h.endgame(pos);
			//if(_heuristic == 32 || _heuristic == -32)
			//	return false;
		//}
		if (is_valid(pos, freepos))
			moves.push_back(Mask::targets(pos));
	}
	return true;
}
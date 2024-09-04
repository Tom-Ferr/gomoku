#include <Node.hpp>

size_t Node::node_count = 0;
BigInt Node::_freepos;

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
{
}

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

void Node::print(std::pair<int, BigInt>&result)
{
	std::cerr << "{";
	std::cerr << "\"depth\": " << _depth << ", ";
	std::cerr << "\"maximizing\": \"";
	if (_state.maximizing())
		std::cerr << "True";
	else
		std::cerr << "False";
	std::cerr << "\", ";
	std::cerr << "\"orig_move\": " << _state.move().pos() << ", ";
	std::cerr << "\"heuristics\": " << result.first << ", ";
	std::cerr << "\"best_child\": " << result.second.pos() << ", ";
	std::cerr << "\"mystate\": \"" << _state.mystate() << "\", ";
	std::cerr << "\"otherstate\": \"" << _state.otherstate() << "\", ";
	std::cerr << "\"alpha\": " << _alpha << ", ";
	std::cerr << "\"beta\": " << _beta;
	std::cerr << "}" << std::endl;
}

std::pair<int, BigInt> Node::minimax()
{
	_heuristic = 0;
	std::pair<int, BigInt> result;
	// if(_state.is_capture())
	// {
	// 	// if(_state.maximizing() == false)
	// 	// {
	// 	// 	int score = (1 << _state.maxi_captures()) + 1;
	// 	// 	if(score > std::abs(_heuristic))
	// 	// 		_heuristic = score;
	// 	// }
	// 	// else
	// 	{
	// 		int score = (1 << _state.mini_captures()) + 1;
	// 		if(score > std::abs(_heuristic))
	// 			_heNode::_uristic = score * -1;
	// 	}
	// }
	if (_depth == 0)
	{
		Heuristics h = Heuristics(_state);
		 _heuristic = h.run();
		//int heur = h.run();
		//if (heur < _heuristic)
		//	_heuristic = heur;
		result = std::make_pair(_heuristic, _state.move());
		print(result);
		return result;
	}
	if (_state.maximizing())
	{
		result =  alpha_beta_prune(_alpha, max);
		print(result);
		return result;
	}
	result = alpha_beta_prune(_beta, min);
	print(result);
	return result;
}

std::pair<int, BigInt> Node::alpha_beta_prune(int &x, comp_func f)
{
	BigInt best_child;
	std::vector<size_t> moves;
	BigInt *move;

	if(possible_moves(moves) == false)
		return std::make_pair(_heuristic, _state.move());
	_heuristic = INT_MAX;
	if (_state.maximizing())
		_heuristic = INT_MIN;
	for (std::vector<size_t>::iterator it = moves.begin(); it != moves.end(); it++)
	{
		move = &Mask::targets(*it);
		Node child(_depth - 1, _alpha, _beta, BoardState(_state, *move));
		std::pair<int, BigInt> score = child.minimax();

		int eval = f(_heuristic, score.first);
		x = f(x, score.first);
		if(eval == score.first && _heuristic != score.first)
		{
			_heuristic = eval;
			best_child = *move;
		}
		if (_alpha >= _beta)
			break;
	}
	return std::make_pair(_heuristic, best_child);
}

bool Node::is_double_free_three(const size_t &pos)
{
	Free_Three_Checker ftc = Free_Three_Checker(_state);
	return ftc.is_free_three(pos);
}

bool Node::is_valid(const size_t &pos, BigInt &freepos)
{
	if(freepos.get_bit(pos) && !is_double_free_three(pos))
		return true;
	return false;
}

bool Node::possible_moves(std::vector<size_t>& moves)
{
	Node::_freepos = _state.expanded_free();

	Heuristics h = Heuristics(_state);
	for (int pos = 0; pos < _state.size(); pos++)
	{
		_heuristic = h.endgame(pos);
		if(_heuristic >= 32 || _heuristic <= -32)
        {
            _heuristic <<= _depth;
            return false;
        }
		else if(_state.maxi_captures() >= 5 || _state.mini_captures() >= 5)
		{
			_heuristic = 32 << _depth;
			if (_state.mini_captures() >= 5)
				_heuristic *= -1;
			return false;
		}
		if (is_valid(pos, _freepos))
			moves.push_back(pos);
	}
	return true;
}

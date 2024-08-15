/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boardstate.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 14:43:15 by iwillens          #+#    #+#             */
/*   Updated: 2024/08/15 13:41:22 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BoardState.hpp"

BigInt BoardState::mask = BigInt(0);
BigInt BoardState::leftmask = BigInt(0);
BigInt BoardState::rightmask = BigInt(0);

BoardState::BoardState(int _sqrt)
:
	_turn(true),
	_size(_sqrt * _sqrt),
	_sqrt(_sqrt)
{
	if (BoardState::mask == BigInt(0))
	{
		BoardState::mask = (BigInt(1) << _size) - 1;
		BoardState::leftmask = mask;
		BoardState::rightmask = mask;
		for (int i = 0; i < _sqrt; i++)
		{
			BoardState::rightmask.clear_bit(i * _sqrt);
			BoardState::leftmask.clear_bit((i + 1) * _sqrt - 1);
		}
	}
	_mystate = BoardState::mask;
	_otherstate = BoardState::mask;
	_inv_mystate = BigInt(0);
	_inv_otherstate = BigInt(0);
	_totalboard = BoardState::mask;
}

BoardState::BoardState(const BoardState& other)
: _turn(other._turn), _size(other._size), _sqrt(other._sqrt),
_move(other._move),
_mystate(other._mystate), _otherstate(other._otherstate),
_inv_mystate(other._inv_mystate), _inv_otherstate(other._inv_otherstate),
_totalboard(other._totalboard)
{ }

BoardState::BoardState(BoardState&& other) noexcept
: _turn(other._turn), _size(other._size), _sqrt(other._sqrt),
_move(other._move),
_mystate(std::move(other._mystate)),
_otherstate(std::move(other._otherstate)),
_inv_mystate(other._inv_mystate), _inv_otherstate(other._inv_otherstate),
_totalboard(other._totalboard)
{ }

BoardState::BoardState(const BoardState& other, BigInt move)
: _turn(!other._turn), _size(other._size), _sqrt(other._sqrt),
_move(move),
_mystate(other._otherstate), _otherstate(other._mystate ^ move),
_inv_mystate(other._inv_otherstate),
_inv_otherstate((~_otherstate) & BoardState::mask),
_totalboard(~(_mystate ^ _otherstate) & BoardState::mask)
{ }

void BoardState::applymove(size_t pos, bool mystate)
{
	BigInt mv = BigInt(1) << pos;
	if (mystate)
	{
		_mystate = _mystate ^ mv;
		_inv_mystate = (~_mystate) & BoardState::mask;
	}
	else
	{
		_otherstate = _otherstate ^ mv;
		_inv_otherstate = (~_otherstate) & BoardState::mask;
	}
	_totalboard = _totalboard ^ mv;
}


BoardState::~BoardState() { }

BigInt const &BoardState::mystate(bool inverted) const
{
	if (inverted)
		return _inv_mystate;
	return _mystate;
}

BigInt const &BoardState::otherstate(bool inverted) const
{
	if (inverted)
		return _inv_otherstate;
	return _otherstate;
}

BigInt const &BoardState::totalboard() const
{
	return _totalboard;
}

bool const &BoardState::turn() const
{
	return _turn;
}

int const &BoardState::size() const
{
	return _size;
}

int const &BoardState::sqrt() const
{
	return _sqrt;
}

BigInt const &BoardState::move() const
{
	return _move;
}

void BoardState::swap_states()
{
	BigInt temp = _mystate;
	_mystate = _otherstate;
	_otherstate = temp;
	temp = _inv_mystate;
	_inv_mystate = _inv_otherstate;
	_inv_otherstate = temp;
	_turn = true;
}

void BoardState::flip_turn()
{
	_turn = !_turn;
}

BigInt BoardState::expanded_free() const
{
	BigInt e[8];
	BigInt t = ~totalboard() & mask;
	e[0] = ((t & BoardState::rightmask) >> 1) & BoardState::mask;
	e[1] = ((t & BoardState::leftmask) << 1) & BoardState::mask;
	e[2] = (t << _sqrt) & BoardState::mask;
	e[3] = (t >> _sqrt) & BoardState::mask;
	e[4] = (e[0] << _sqrt) & BoardState::mask;
	e[5] = (e[1] << _sqrt) & BoardState::mask;
	e[6] = (e[0] >> _sqrt) & BoardState::mask;
	e[7] = (e[1] >> _sqrt) & BoardState::mask;
	return ((e[0] | e[1] | e[2] | e[3] | e[4] | e[5] | e[6] | e[7])
				& totalboard() & mask);
}

void BoardState::print()
{
	std::cout << "\033[2J\033[1;1H";

	for (int y = _sqrt - 1; y >= 0; y--)
	{
		for (int x = _sqrt - 1; x >= 0; x--)
		{
			if (!_mystate.get_bit(y * _sqrt + x))
				std::cout << "\033[1;36m X \033[0m";
			else if (!_otherstate.get_bit(y * _sqrt + x))
				std::cout << "\033[1;35m 0 \033[0m";
			else
				std::cout << "\033[1;33m . \033[0m";
			if (x == 0)
				std::cout << std::endl;
		}
	}
}

BoardState& BoardState::operator=(const BoardState& other)
{
	if (this != &other)
	{
		_turn = other._turn;
		_size = other._size;
		_sqrt = other._sqrt;
		_move = other._move;
		_mystate = other._mystate;
		_otherstate = other._otherstate;
		_totalboard = other._totalboard;
		_inv_mystate = other._mystate;
		_inv_otherstate = other._inv_otherstate;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const BoardState &bs)
{
	//os << "Mask: " << BoardState::mask << std::endl;
	//os << "Left Mask: " << BoardState::leftmask << std::endl;
	//os << "Right Mask: " << BoardState::rightmask << std::endl;
	os << "_turn: " << bs.turn() << std::endl;
	os << "_size: " << bs.size() << std::endl;
	os << "_sqrt: " << bs.sqrt() << std::endl;
	os << "_mystate: " << bs.mystate() << std::endl;
	os << "_otherstate: " << bs.otherstate() << std::endl;
	os << "_totalboard: " << bs.totalboard() << std::endl;
	BigInt freepos = bs.expanded_free();
	os << "expanded_free: " << freepos << std::endl;
	os << "possible moves: " << std::endl;
	for (size_t i = 0; i < freepos.size(); i++)
	{
		if (freepos.get_bit(i))
			std::cout << "\t" << i;
	}
	std::cout  << std::endl;
	return os;
}
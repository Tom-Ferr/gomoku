/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boardstate.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 14:43:15 by iwillens          #+#    #+#             */
/*   Updated: 2024/08/07 16:57:04 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BoardState.hpp"

BigInt BoardState::mask = BigInt("0");

BoardState::BoardState(int _sqrt)
:
	_turn(true),
	_size(_sqrt * _sqrt),
	_sqrt(_sqrt),
	_mystate(0),
	_otherstate(0),
	_totalboard(0)
{
	if (BoardState::mask == BigInt("0"))
		BoardState::mask = (BigInt("1") << _size) - 1;
}

BoardState::BoardState(const BoardState& other)
: _turn(other._turn), _size(other._size), _sqrt(other._sqrt),
_mystate(other._mystate), _otherstate(other._otherstate),
_totalboard(other._totalboard)
{ }

BoardState::BoardState(BoardState&& other) noexcept
: _turn(other._turn), _size(other._size), _sqrt(other._sqrt),
_mystate(std::move(other._mystate)),
_otherstate(std::move(other._otherstate)),
_totalboard(other._totalboard)
{ }

BoardState::BoardState(const BoardState& other, BigInt move)
: _turn(!other._turn), _size(other._size), _sqrt(other._sqrt),
_mystate(other._otherstate), _otherstate(other._mystate | move),
_totalboard(_mystate | _otherstate)
{ }

void BoardState::applymove(size_t pos, bool mystate)
{
	BigInt mv = BigInt(1) << pos;
	if (mystate)
		_mystate = _mystate | mv;
	else
		_otherstate = _otherstate | mv;
	_totalboard = _totalboard | mv;
}

BoardState::~BoardState() { }

BigInt const &BoardState::mystate() const
{
	return _mystate;
}

BigInt const &BoardState::otherstate() const
{
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

size_t const &BoardState::move() const
{
	return _move;
}

BoardState& BoardState::operator=(const BoardState& other)
{
	if (this != &other)
	{
		_turn = other._turn;
		_size = other._size;
		_sqrt = other._sqrt;
		_mystate = other._mystate;
		_otherstate = other._otherstate;
		_totalboard = other._totalboard;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const BoardState &bs)
{
	os << "Mask: " << BoardState::mask << std::endl;
	os << "_turn: " << bs.turn() << std::endl;
	os << "_size: " << bs.size() << std::endl;
	os << "_sqrt: " << bs.sqrt() << std::endl;
	os << "_mystate: " << bs.mystate() << std::endl;
	os << "_otherstate: " << bs.otherstate() << std::endl;
	os << "_totalboard: " << bs.totalboard() << std::endl;
	return os;
}
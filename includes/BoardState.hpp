/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boardstate.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:49:52 by iwillens          #+#    #+#             */
/*   Updated: 2024/08/07 23:15:11 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARDSTATE_HPP
# define BOARDSTATE_HPP

# include "gomoku.hpp"
# include <ostream>

class BoardState
{
	public:
		static BigInt mask;
		static BigInt leftmask;
		static BigInt rightmask;

		private:
			bool	_turn;
			int		_size;
			int 	_sqrt;
			size_t	_move;
			BigInt	_mystate;
			BigInt	_otherstate;
			BigInt	_totalboard;

		public:
			BoardState(int sqrt=19);
			BoardState(const BoardState& other);
			BoardState(BoardState&& other) noexcept;
			BoardState(const BoardState& other, BigInt move);
			~BoardState();
			BoardState& operator=(const BoardState& other);
			void applymove(size_t pos, bool mystate=true);
			void applymove(BigInt move, bool mystate);

			// getters
			bool	const &turn() const;
			int		const &size() const;
			int		const &sqrt() const;
			size_t 	const &move() const;
			BigInt 	const &mystate() const;
			BigInt 	const &otherstate() const;
			BigInt 	const &totalboard() const;
			BigInt 	expanded_free() const;
			void swap_states();
			void flip_turn();
			void print();

};

std::ostream &operator<<(std::ostream &os, const BoardState &bs);

#endif

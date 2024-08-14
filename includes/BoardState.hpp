#ifndef BOARDSTATE_HPP
# define BOARDSTATE_HPP

# include <gomoku.hpp>
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
			BigInt	_move;
			BigInt	_mystate;
			BigInt	_otherstate;
			BigInt	_inv_mystate;
			BigInt	_inv_otherstate;
			BigInt	_totalboard;

		public:
			bool 	_random;
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
			BigInt 	const &move() const;
			BigInt 	const &mystate(bool inverted=false) const;
			BigInt 	const &otherstate(bool inverted=false) const;
			BigInt 	const &totalboard() const;
			BigInt 	expanded_free() const;
			void swap_states();
			void flip_turn();
			void print();

};

std::ostream &operator<<(std::ostream &os, const BoardState &bs);

#endif

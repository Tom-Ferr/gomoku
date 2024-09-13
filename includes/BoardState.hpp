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
			bool	_maximizing;
			bool 	_capture_move = false;
			int		_size;
			int 	_sqrt;
			int 	_maxi_captures = 0;
			int 	_mini_captures = 0;
			BigInt	_move;
			BigInt	_mystate;
			BigInt	_otherstate;
			BigInt	_inv_mystate;
			BigInt	_inv_otherstate;
			BigInt	_totalboard;
			static Mask _masks;
			static Mask _capturable_masks;
			static bool _with_captures;
			const char _modes[4] = {HORIZONTAL, VERTICAL, CRESCENDO, DECRESCENDO};

		public:
			BoardState(int sqrt=19);
			BoardState(const BoardState& other);
			BoardState(BoardState&& other) noexcept;
			BoardState(const BoardState& other, BigInt move);
			~BoardState();
			BoardState& operator=(const BoardState& other);
			void applymove(size_t pos, bool mystate=true);
			void applymove(BigInt move, bool mystate=true);

			// getters
			bool	const &maximizing() const;
			bool 	const &is_capture() const;
			bool 	const &with_captures() const;
			int		const &size() const;
			int		const &sqrt() const;
			int		const &maxi_captures() const;
			int		const &mini_captures() const;
			BigInt 	const &move() const;
			BigInt 	const &mystate(bool inverted=false) const;
			BigInt 	const &otherstate(bool inverted=false) const;
			BigInt 	const &totalboard() const;
			BigInt 	expanded_free() const;
			void swap_states();
			void set_captures(bool);
			void print();
			void increment_captures(bool turn);
			void increment_captures(bool turn, size_t points);
			static void set_masks(int mask_size, int board_sqrt, bool mirrored);
			void check_capture(size_t pos, bool maximizing);
			int check_capture(const BigInt &self, const BigInt &rival, const size_t &pos, char orientation);
			size_t check_capture(const BigInt &target, bool maximizing);
		private:

};

std::ostream &operator<<(std::ostream &os, const BoardState &bs);

#endif

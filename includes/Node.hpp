/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Node.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:34:12 by iwillens          #+#    #+#             */
/*   Updated: 2024/08/07 14:42:14 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODE_HPP
# define NODE_HPP

# include "gomoku.hpp"
# include <vector>

class Node
{
	private:
		t_board				_board;
		int					_score;
		int					_alpha;
		int 				_beta;
		int					_depth;
		std::vector<Node> 	_children;

	public:
		Node(int depth, int alpha, int beta, t_board board)
		: _depth(depth), _alpha(alpha), _beta(beta), _board(board)
		{
			_score = 0;
		}

		Node(const Node& other)
		: _depth(other._depth), _alpha(other._alpha), _beta(other._beta), _board(other._board)
		{}


};

#endif

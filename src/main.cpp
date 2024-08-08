/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:00:02 by iwillens          #+#    #+#             */
/*   Updated: 2024/08/07 23:02:36 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gomoku.hpp"
#include "Node.hpp"
#include "Game.hpp"

int main (void)
{
	//BoardState bs(19);
	//bs.applymove(100, false);
	//std::cout << bs << std::endl;
	//BoardState b(bs, BigInt(1) << 101);
	//std::cout << b << std::endl;
	//Node n(3, INT_MIN, INT_MAX, b);
	//n.minimax();
	Game g;

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	g.step();
	while (g.step())
		continue;
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Time taken (TOTAL): " << duration.count() << " seconds" << std::endl;
	std::cout << "Node count: " << Node::node_count << std::endl;
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:00:02 by iwillens          #+#    #+#             */
/*   Updated: 2024/08/08 19:23:53 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gomoku.hpp"
#include "Node.hpp"
#include "Game.hpp"

int main (void)
{
//	Game g;
//
//	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
//	g.step();
//	while (g.step())
//		continue;
//	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
//	std::cout << "Time taken (TOTAL): " << duration.count() << " seconds" << std::endl;
//	std::cout << "Node count: " << Node::node_count << std::endl;
	Mask m = Mask(6, 19);
	m.print_mask(m['d']["edge"]);
	return (0);
}

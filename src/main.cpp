/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:00:02 by iwillens          #+#    #+#             */
/*   Updated: 2024/08/07 15:41:18 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gomoku.hpp"

int main (void)
{
	BoardState bs(19);
	bs.applymove(100, false);
	std::cout << bs << std::endl;
	return (0);
}

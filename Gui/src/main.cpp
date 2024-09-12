#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// #include <Board.hpp>
#include <Heuristics.hpp>
#include <Free_Three_Checker.hpp>
#include <Gui.hpp>
#include <Logger.hpp>

//static mlx_image_t* image;

// -----------------------------------------------------------------------------


int main()
{
	Logger::set_active(true);
	Logger::log_move(1, "human", 1, true);
	Gui gui;
	Logger::close_fds();
	return (EXIT_SUCCESS);
}
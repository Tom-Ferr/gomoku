#include <iostream>
#include <stdbool.h>
#include <gomoku.hpp>
#include <Gui.hpp>
#include <Logger.hpp>

static void usage(bool help=false)
{
	std::cout << "Usage: ./gomoku [--debug | --test | --help]" << std::endl;
	if (help)
	{
		std::cout << "  --debug: run the program with debug logs" << std::endl;
		std::cout << "  --test: run the program with debug logs and run tests" << std::endl;
		std::cout << "  --help: display this help message" << std::endl;
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}

/*
** returns true for running main, false for running tests
*/
static bool parse_args(int argc, char **argv)
{
	if (argc == 1)
		return true;
	if (argc == 2 && std::string(argv[1]) == "--help")
		usage(true);
	else if (argc == 2 && std::string(argv[1]) == "--debug")
	{
		Logger::set_active(true);
		return true;
	}
	else if (argc == 2 && std::string(argv[1]) == "--test")
	{
		Logger::set_active(true);
		return false;
	}
	else
		usage();
	return true;
}

int main(int argc, char **argv)
{
	Logger::set_active(false);
	if (!parse_args(argc, argv))
		test();
	else
		Gui gui;
	Logger::close_fds();
	return (EXIT_SUCCESS);
}

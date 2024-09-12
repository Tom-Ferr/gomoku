#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <iostream>
# include <unistd.h>
# include <fcntl.h>

# define LOGMOVES_FILENAME "moves.log"

class Logger
{
	private:
		static bool _active;
		static int _moves_fd;
		static bool _init(int &fd, const char *filename);

	public:
		Logger();
		Logger(Logger const &other);
		~Logger();
		Logger &operator=(Logger const &other);
		static void log_move(size_t current_move, std::string who, size_t move, bool turn);
		static void close_fds();
		static void set_active(bool active);
};

#endif
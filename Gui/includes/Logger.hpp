#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <iostream>
# include <unistd.h>
# include <fcntl.h>
# include <chrono>
# include <ctime>
# include <sys/stat.h>
# include <iostream>
# include <BoardState.hpp>

# define LOGMOVES_FILENAME "moves.log"
# define LOGSTATE_FILENAME "boardstate.log"
# define LOGDIR "logs"

class Logger
{
	private:
		static bool			_active;
		static int			_moves_fd;
		static std::string	_logdir;
		static bool _init(int &fd, const char *filename);
		static bool _create_logdir();
		static std::string _get_current_time();

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

#include <Logger.hpp>

int Logger::_moves_fd = -1;
bool Logger::_active = true;

Logger::Logger()
{ }

Logger::Logger(Logger const &other)
{
	*this = other;
}

Logger::~Logger()
{ }

Logger &Logger::operator=(Logger const &other)
{
	(void)other;
	return (*this);
}

bool Logger::_init(int &fd, const char *filename)
{
	if (!_active)
		return false;
	if (fd != -1)
		return true;
	fd = open (filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return false;
	return true;
};

void Logger::log_move(size_t current_move, std::string who, size_t move, bool turn)
{
	if (!_init(_moves_fd, LOGMOVES_FILENAME))
		return ;
	while (who.size() < 10)
		who += " ";
	std::string str = std::to_string(current_move) + "\t" + who + "\t\t" + std::to_string(move) + "\t"
		+ " ( x:" + std::to_string(move % 19) + " y:" + std::to_string(move / 10) + " )" + "\t"
		+ (turn ? "Black" : "White") + "\n";
	write(_moves_fd, str.c_str(), str.size());
}

void Logger::close_fds()
{
	if (_moves_fd != -1)
		close(_moves_fd);
}

void Logger::set_active(bool active)
{
	_active = active;
}

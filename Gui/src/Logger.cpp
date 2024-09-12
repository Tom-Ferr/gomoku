
#include <Logger.hpp>

int Logger::_moves_fd = -1;
bool Logger::_active = true;
std::string Logger::_logdir = "";

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

std::string Logger::_get_current_time()
{
	char buffer[100];
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_r(&now_c, &tm);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", &tm);
	return buffer;
}

bool Logger::_create_logdir()
{
	std::string logdir;

	if (access(LOGDIR, F_OK) == -1)
	{
		if (mkdir(LOGDIR, 0755) == -1)
			return false;
	}
	logdir = LOGDIR;
	logdir += "/" + _get_current_time();
	if (access(logdir.c_str(), F_OK) == -1)
	{
		if (mkdir(logdir.c_str(), 0755) == -1)
			return false;
	}
	_logdir = logdir;
	return true;
}

bool Logger::_init(int &fd, const char *filename)
{
	std::string path;

	_get_current_time();
	if (!_active)
		return false;
	if (fd != -1)
		return true;
	if (_logdir.size() == 0 && !_create_logdir())
		return false;
	path = _logdir + "/" + filename;
	fd = open (path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

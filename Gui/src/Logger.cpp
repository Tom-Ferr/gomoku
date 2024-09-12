
#include <Logger.hpp>
#include <Heuristics.hpp>

int Logger::_moves_fd = -1;
int Logger::_boardstate_fd = -1;
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

void Logger::_log_boardstate(BoardState &bs)
{
	std::ostringstream os;
	std::string str;

	if (!_init(_moves_fd, LOGSTATE_FILENAME))
		return ;

	BigInt mystate = bs.mystate();
	BigInt otherstate = bs.otherstate();
	os << "Full Board State:" << std::endl;
	for (int y = bs.sqrt() - 1; y >= 0; y--)
	{
		for (int x =  bs.sqrt() - 1; x >= 0; x--)
		{
			if (!mystate.get_bit(y * bs.sqrt() + x))
				os << "X ";
			else if (!otherstate.get_bit(y * bs.sqrt() + x))
				os << "0 ";
			else
				os << ". ";
			if (x == 0)
				os << std::endl;
		}
	}

	os << std::endl << "Black State:" << std::endl;
	for (int y = bs.sqrt() - 1; y >= 0; y--)
	{
		for (int x = bs.sqrt() - 1; x >= 0; x--)
		{
			if (!mystate.get_bit(y * bs.sqrt() + x))
				os << "1";
			else
				os << "0";
			if (x == 0)
				os << std::endl;
		}
	}

	os << std::endl << "White State:" << std::endl;
	for (int y = bs.sqrt() - 1; y >= 0; y--)
	{
		for (int x = bs.sqrt() - 1; x >= 0; x--)
		{
			if (!otherstate.get_bit(y * bs.sqrt() + x))
				os << "1";
			else
				os << "0";
			if (x == 0)
				os << std::endl;
		}
	}
	os << std::endl;

	os << "Heuristics:" << std::endl;
	std::streambuf* orig_cout = std::cout.rdbuf();
    std::cout.rdbuf(os.rdbuf());

	Heuristics h(bs);
	int result = h.run();
	os << "Value: " << result << std::endl;
	os << "Description:" << std::endl;
	h.describe_heuristic();

    std::cout.rdbuf(orig_cout);
	str = os.str();
	write(_boardstate_fd, str.c_str(), str.size());
}

void Logger::log_move(size_t total_moves, std::string who, size_t move, bool turn)
{
	if (!_init(_moves_fd, LOGMOVES_FILENAME))
		return ;
	while (who.size() < 10)
		who += " ";
	std::string str = std::to_string(total_moves) + "\t" + who + "\t\t" + std::to_string(move) + "\t"
		+ " ( x:" + std::to_string(move % 19) + " y:" + std::to_string(move / 10) + " )" + "\t"
		+ (turn ? "Black" : "White") + "\n";
	write(_moves_fd, str.c_str(), str.size());
}

void Logger::log_move(size_t total_moves, std::string who, size_t move, bool turn, BoardState &bs)
{
	log_move(total_moves, who, move, turn);
	if (!_init(_boardstate_fd, LOGSTATE_FILENAME))
		return ;
	while (who.size() < 10)
		who += " ";
	std::string str = std::to_string(total_moves) + "\t" + who + "\t\t" + std::to_string(move) + "\t"
		+ " ( x:" + std::to_string(move % 19) + " y:" + std::to_string(move / 10) + " )" + "\t"
		+ (turn ? "Black" : "White") + "\n";
	write(_boardstate_fd, str.c_str(), str.size());
	_log_boardstate(bs);
	write(_boardstate_fd, SEPARATOR, sizeof(SEPARATOR) - 1);
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

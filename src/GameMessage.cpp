#include <GameMessage.hpp>

GameMessage::GameMessage()
: _active(false), _player(false), _selection(false), _message(""), _mode("Swap")
{}

GameMessage::GameMessage(bool player, bool selection, std::string message, std::string mode)
: _active(true), _player(player), _selection(selection), _message(message), _mode(mode)
{}

GameMessage::GameMessage(const GameMessage& other)
: _active(other._active), _player(other._player), _selection(other._selection), _message(other._message), _mode(other._mode)
{}

GameMessage &GameMessage::operator=(const GameMessage& other)
{
	if (this != &other)
	{
		_active = other._active;
		_player = other._player;
		_selection = other._selection;
		_message = other._message;
		_mode = other._mode;
	}
	return *this;
}

GameMessage::operator bool()
{
	return _active;
}

GameMessage::~GameMessage() {}

void GameMessage::clear()
{
	_active = false;
	_player = false;
	_selection = false;
	_message = "";
	_mode = "Swap";
}

std::string const &GameMessage::text() const
{
	return _message;
}

std::string const &GameMessage::mode() const
{
	return _mode;
}

bool GameMessage::player() const
{
	return _player;
}

bool GameMessage::selection() const
{
	return _selection;
}

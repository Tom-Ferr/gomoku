#ifndef GAMEMESSAGE_HPP
# define GAMEMESSAGE_HPP

# include <iostream>

# define MSG_SWAP "Swap"
# define MSG_SWAP2 "Swap2"
# define MSG_P1_CHOOSES "Player 1 must choose"
# define MSG_P2_CHOOSES "Player 2 must choose"
# define MSG_P1_CHOSE_WHITE "AI chose to play as whites"
# define MSG_P1_CHOSE_BLACK "AI chose to play as whites"
# define MSG_P2_CHOSE_WHITE "AI chose to play as whites"
# define MSG_P2_CHOSE_BLACK "AI chose to play as whites"

class GameMessage
{
	private:
		bool _active;
		bool _player;
		bool _selection;
		std::string _message;
		std::string _mode;

	public:
		GameMessage();

		GameMessage(bool player, bool selection, std::string message="", std::string mode="Swap");

		GameMessage(const GameMessage& other);
		GameMessage &operator=(const GameMessage& other);

		operator bool();

		~GameMessage();
		void clear();
		std::string const &text() const;
		std::string const &mode() const;
		bool player() const;
		bool selection() const;
};

#endif

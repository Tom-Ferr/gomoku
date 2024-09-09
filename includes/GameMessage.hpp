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

# define MSG_PRO_INFO	"The starting player (black) puts the first stone to the middle        "\
					   	"intersection of the board, this move is compulsory. The second player "\
					   	"can put the second move anywhere on the board. Then, on black's turn, "\
					   	"the third move has to be outside a 5x5 square from the center of the  "\
					   	"board.                                                                "

# define MSG_LONGPRO_INFO	"The starting player (black) puts the first stone to the middle        "\
					   		"intersection of the board, this move is compulsory. The second player "\
					   		"can put the second move anywhere on the board. Then, on black's turn, "\
					   		"the third move has to be outside a 7x7 square from the center of the  "\
					   		"board.                                                                "

# define MSG_SWAP_INFO		"The starting player puts the first three stones anywhere on the board "\
							"(two black stones and a white one). The second player can decide      "\
							"whether s/he wants to keep white stones and put a fourth stone or s/he"\
							"can swap and control the black stones.                                "

# define MSG_SWAP2_INFO		"The first player puts three stones (two blacks and one white) on any  "\
							"intersections of the board. The second player has three options now:  "\
							"1. Choose to stay as whites (and keeps playing)                       "\
							"2. Choose to play as blacks (and it's the other player's turn)        "\
							"3. Place another white and another black stone on the board and defer "\
							"the choice to the opponent.                                           "

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

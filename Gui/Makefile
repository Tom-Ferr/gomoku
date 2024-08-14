FILES			=	BigInt.cpp BoardState.cpp Free_Three_Checker.cpp Game.cpp Heuristics.cpp Mask.cpp Node.cpp

GUI_FILES		=	Tile.cpp Board.cpp main.cpp
GUI_SRCDIR			= 	src/

SRCDIR			= 	../src/

SRCS			= 	$(addprefix $(SRCDIR), $(FILES)) $(addprefix $(GUI_SRCDIR), $(GUI_FILES))

OBJS			= 	${SRCS:.cpp=.o}

HEADS			=	BigInt.hpp BoardState.hpp Free_Three_Checker.hpp Game.hpp gomoku.hpp Heuristics.hpp Mask.hpp Node.hpp
GUI_HEADS		=	Tile.hpp Board.hpp

INC				= 	../includes/
GUI_INC			=	./includes/
DEPS			= 	$(addprefix ${INC}, $(HEADS)) $(addprefix ${GUI_INC}, $(GUI_HEADS))

NAME			= 	gui

CXX				=	clang++

CXXFLAGS		= 	 -std=c++11 #-Wall -Wextra -Werror


SFML_INC		=	-I/opt/homebrew/Cellar/sfml/2.6.1/include
SFML_LIB		=	-L/opt/homebrew/Cellar/sfml/2.6.1/lib
GMP_INC			=	-I/opt/homebrew/Cellar/gmp/6.3.0/include
GMP_LIB			=	-L/opt/homebrew/Cellar/gmp/6.3.0/lib

INCLUDE 		= 	-I${INC} -I${GUI_INC}

SANITIZE 		= 	-fsanitize=address -g

UNAME			=	$(shell uname)

LINUX			= -D _LINUX

LIBS 			= -lsfml-graphics -lsfml-window -lsfml-system -lgmp -lgmpxx

ifeq ($(UNAME),Linux)
    CXXFLAGS += $(LINUX)
else ifeq ($(shell uname -s), Darwin)
    INCLUDE += $(SFML_INC)
    LIBS += $(SFML_LIB)
	INCLUDE += $(GMP_INC)
    LIBS += $(GMP_LIB)
endif

all:			${NAME}

check-gmp:
ifeq ($(shell pkg-config --exists sfml-graphics && echo yes || echo no), no)
	@echo "sfml is not installed. Installing sfml..."
ifeq ($(shell uname -s), Linux)
	@sudo apt-get update && sudo apt-get install -y libsfml-dev
else ifeq ($(shell uname -s), Darwin)
	@brew install lsfm
else
	@echo "Unsupported OS. Please install LSFM manually."
	exit 1
endif
else
	@echo "sfml is already installed."
endif


$(NAME):		check-gmp ${OBJS} $(DEPS)
				${CXX} ${CXXFLAGS} ${SANITIZE} ${OBJS} ${INCLUDE} ${LIBS} -o ${NAME}

%.o: %.cpp
				${CXX} ${CXXFLAGS} ${SANITIZE} ${INCLUDE} -c $< -o $@

test:			all
				${NAME}

clean:
				rm -f ${OBJS}

fclean:			clean
				rm -f ${NAME}
re:				fclean all

.PHONY:			all clean fclean re
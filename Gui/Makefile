FILES			=	main.cpp Tile.cpp Board.cpp

SRCDIR			= 	src/

SRCS			= 	$(addprefix $(SRCDIR), $(FILES))

OBJS			= 	${SRCS:.cpp=.o}

HEADS			=

INC				= 	./includes/

DEPS			= 	$(addprefix ${INC}, $(HEADS))

NAME			= 	gui

CXX				=	clang++

CXXFLAGS		= 	 -Wall -Wextra -Werror -std=c++11


SFML_INC			=	-I/opt/homebrew/Cellar/sfml/2.6.1/include
SFML_LIB			=	-L/opt/homebrew/Cellar/sfml/2.6.1/lib

INCLUDE 		= 	-I${INC}

SANITIZE 		= 	-fsanitize=address -g

UNAME			=	$(shell uname)

LINUX			= -D _LINUX

LIBS 			= -lsfml-graphics -lsfml-window -lsfml-system

ifeq ($(UNAME),Linux)
    CXXFLAGS += $(LINUX)
else ifeq ($(shell uname -s), Darwin)
    INCLUDE += $(SFML_INC)
    LIBS += $(SFML_LIB)
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
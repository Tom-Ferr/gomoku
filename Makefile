FILES			=	BigInt.cpp BoardState.cpp Free_Three_Checker.cpp Game.cpp Heuristics.cpp main.cpp Mask.cpp Node.cpp

SRCDIR			= 	src/

SRCS			= 	$(addprefix $(SRCDIR), $(FILES))

OBJS			= 	${SRCS:.cpp=.o}

HEADS			=	BigInt.hpp BoardState.hpp Free_Three_Checker.hpp Game.hpp gomoku.hpp Heuristics.hpp Mask.hpp Node.hpp

INC				= 	./includes/

DEPS			= 	$(addprefix ${INC}, $(HEADS))

NAME			= 	gomoku

CXX				=	clang++

CXXFLAGS		= 	 -std=c++11#-Wall -Wextra -Werror


GMP_INC			=	-I/opt/homebrew/Cellar/gmp/6.3.0/include
GMP_LIB			=	-L/opt/homebrew/Cellar/gmp/6.3.0/lib

INCLUDE 		= 	-I${INC}

SANITIZE 		= 	-O3#-fsanitize=address

UNAME			=	$(shell uname)

LINUX			= 	-D _LINUX

GMP_INC			=	-I/opt/homebrew/Cellar/gmp/6.3.0/include

GMP_LIB			=	-L/opt/homebrew/Cellar/gmp/6.3.0/lib

LIBS 			= -lgmp -lgmpxx

ifeq ($(UNAME),Linux)
    CXXFLAGS += $(LINUX)
else ifeq ($(shell uname -s), Darwin)
    INCLUDE += $(GMP_INC)
    LIBS += $(GMP_LIB)
endif


%.o: %.cpp
				${CXX} ${CXXFLAGS} ${SANITIZE} ${INCLUDE} -c $< -o $@

$(NAME):		${OBJS} $(DEPS)
				${CXX} ${CXXFLAGS} ${SANITIZE} ${OBJS} ${GMP} ${INCLUDE} -o ${NAME}

all:			${NAME}

check-gmp:
ifeq ($(shell pkg-config --exists gmp && echo yes || echo no), no)
	@echo "GMP is not installed. Installing GMP..."
ifeq ($(shell uname -s), Linux)
	@sudo apt-get update && sudo apt-get install -y libgmp-dev
else ifeq ($(shell uname -s), Darwin)
	@brew install gmp
else
	@echo "Unsupported OS. Please install GMP manually."
	exit 1
endif
else
	@echo "GMP is already installed."
endif


$(NAME):		check-gmp ${OBJS} $(DEPS)
				${CXX} ${CXXFLAGS} ${SANITIZE} ${OBJS} ${INCLUDE} ${LIBS} -o ${NAME}

%.o: %.cpp
				${CXX} ${CXXFLAGS} ${INCLUDE} -c $< -o $@

test:			all
				${NAME}

clean:
				rm -f ${OBJS}

fclean:			clean
				rm -f ${NAME}
re:				fclean all

.PHONY:			all clean fclean re
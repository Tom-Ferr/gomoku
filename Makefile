FILES			=	

SRCDIR			= 	src/

SRCS			= 	$(addprefix $(SRCDIR), $(FILES)) main.cpp

OBJS			= 	${SRCS:.cpp=.o}

HEADS			=	

INC				= 	./includes/

DEPS			= 	$(addprefix ${INC}, $(HEADS))

NAME			= 	gomoku

CXX				=	clang++

CXXFLAGS		= 	-g -Wall -Wextra -Werror -lgmp -lgmpxx

INCLUDE 		= 	-I${INC}

SANITIZE 		= 	-fsanitize=address

UNAME			=	$(shell uname)

LINUX			= -D _LINUX

ifeq ($(UNAME),Linux)
	CXXFLAGS += $(LINUX)
endif

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

%.o: %.cpp		
				${CXX} ${CXXFLAGS} ${INCLUDE} -c $< -o $@

$(NAME):		check-gmp ${OBJS} $(DEPS)
				${CXX} ${CXXFLAGS} ${SANITIZE} ${OBJS} ${INCLUDE} -o ${NAME}

all:			${NAME}

test:			all
				${NAME}

clean:
				rm -f ${OBJS}

fclean:			clean
				rm -f ${NAME}
re:				fclean all

.PHONY:			all clean fclean re
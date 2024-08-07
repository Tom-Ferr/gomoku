FILES			=	

SRCDIR			= 	src/

SRCS			= 	$(addprefix $(SRCDIR), $(FILES))

OBJS			= 	${SRCS:.cpp=.o}

HEADS			=	

INC				= 	./includes/

DEPS			= 	$(addprefix ${INC}, $(HEADS))

MAIN			=	main.cpp

NAME			= 	gomoku

CXX				=	clang++

CXXFLAGS		= 	-g -Wall -Wextra -Werror

INCLUDE 		= 	-I${INC}

SANITIZE 		= 	-fsanitize=address

UNAME			=	$(shell uname)

LINUX			= -D _LINUX

ifeq ($(UNAME),Linux)
	CXXFLAGS += $(LINUX)
endif

%.o: %.cpp		
				${CXX} ${CXXFLAGS} ${INCLUDE} -c $< -o $@

$(NAME):		${OBJS} $(DEPS) ${MAIN}
				${CXX} ${CXXFLAGS} ${SANITIZE} ${OBJS} ${INCLUDE} ${MAIN} -o ${NAME}

all:			${NAME}

test:			all
				${NAME}

clean:
				rm -f ${OBJS}

fclean:			clean
				rm -f ${NAME}
re:				fclean all

.PHONY:			all clean fclean re
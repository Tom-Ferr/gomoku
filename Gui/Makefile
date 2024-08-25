FILES			=	BigInt.cpp BoardState.cpp Free_Three_Checker.cpp Game.cpp Heuristics.cpp Mask.cpp Node.cpp

GUI_FILES		=	main.cpp  Gui.cpp Board.cpp Rect.cpp Tile.cpp Color.cpp Menu.cpp Text.cpp Button.cpp ButtonGroup.cpp
GUI_SRCDIR			= 	src/

SRCDIR			= 	../src/

SRCS			= 	$(addprefix $(SRCDIR), $(FILES)) $(addprefix $(GUI_SRCDIR), $(GUI_FILES))

OBJS			= 	${SRCS:.cpp=.o}

HEADS			=	BigInt.hpp BoardState.hpp Free_Three_Checker.hpp Game.hpp gomoku.hpp Heuristics.hpp Mask.hpp Node.hpp
GUI_HEADS		=	Board.hpp Gui.hpp Rect.hpp Tile.hpp Color.hpp Menu.hpp Text.hpp Button.hpp ButtonGroup.hpp

INC				= 	../includes/
GUI_INC			=	./includes/
DEPS			= 	$(addprefix ${INC}, $(HEADS)) $(addprefix ${GUI_INC}, $(GUI_HEADS))

NAME			= 	gui

CXX				=	clang++

CXXFLAGS		= 	 -std=c++11 -Wall -Wextra -Werror


MLX_INC			=	-I./MLX42/include -I/opt/homebrew/Cellar/glfw/3.4/include
MLX_LIB			=	-L./MLX42/build -L/opt/homebrew/Cellar/glfw/3.4/lib
GMP_INC			=	-I/opt/homebrew/Cellar/gmp/6.3.0/include
GMP_LIB			=	-L/opt/homebrew/Cellar/gmp/6.3.0/lib

INCLUDE 		= 	-I${INC} -I${GUI_INC}

SANITIZE 		= 	-fsanitize=address -g

UNAME			=	$(shell uname)

LINUX			= -D _LINUX

LIBS 			=  -lmlx42 -lglfw -lgmp -lgmpxx -ldl -lglfw -pthread -lm#  -framework Cocoa -framework OpenGL -framework IOKit #-lsfml-graphics -lsfml-window -lsfml-system

INCLUDE += $(MLX_INC)
LIBS += $(MLX_LIB)
INCLUDE += $(GMP_INC)
LIBS += $(GMP_LIB)

all:			${NAME}


$(NAME):		${OBJS} $(DEPS)
				${CXX} ${CXXFLAGS} ${SANITIZE} ${OBJS} ${INCLUDE} -o ${NAME}  ${LIBS}

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

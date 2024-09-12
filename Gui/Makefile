# Directories
SRCDIR			= 	../src/
GUI_SRCDIR		= 	./src/
BUILD_DIR		= 	./build/
INC				= 	../includes/
GUI_INC			=	./includes/

# Files
FILES			=	BigInt.cpp BoardState.cpp Free_Three_Checker.cpp \
					Game.cpp Heuristics.cpp Mask.cpp Node.cpp GameMessage.cpp

GUI_FILES		=	main.cpp  Gui.cpp Board.cpp Rect.cpp Tile.cpp Color.cpp Menu.cpp \
					Text.cpp Button.cpp ButtonGroup.cpp Info.cpp BoardStatusBar.cpp \
					BoardEndGame.cpp BoardMode.cpp ABoardPopup.cpp Logger.cpp

# Full Paths
SRCS			= 	$(addprefix $(SRCDIR), $(FILES)) $(addprefix $(GUI_SRCDIR), $(GUI_FILES))
OBJS			= 	$(addprefix $(BUILD_DIR), $(notdir ${SRCS:.cpp=.o}))
DEPS 			=	$(patsubst %.o, %.d, $(OBJS))

# Compiler
NAME			=	gui
CXX				=	clang++
CXXFLAGS		=	-std=c++11 -Wall -Wextra -Werror
SANITIZE 		=	-g
INCLUDE 		=	-I${INC} -I${GUI_INC}
LIBS 			=	-lmlx42 -lglfw -lgmp -lgmpxx -ldl -pthread -lm

# MLX
MLX_INC			=	-I./MLX42/include -I/opt/homebrew/Cellar/glfw/3.4/include
MLX_LIB			=	-L./MLX42/build -L/opt/homebrew/Cellar/glfw/3.4/lib
GMP_INC			=	-I/opt/homebrew/Cellar/gmp/6.3.0/include
GMP_LIB			=	-L/opt/homebrew/Cellar/gmp/6.3.0/lib

UNAME			=	$(shell uname)
LINUX			= -D _LINUX

INCLUDE += $(MLX_INC)
LIBS += $(MLX_LIB)
INCLUDE += $(GMP_INC)
LIBS += $(GMP_LIB)


# Rules

all:		${NAME}

$(NAME):	${OBJS}
			${CXX} ${CXXFLAGS} ${SANITIZE} ${OBJS} ${INCLUDE} -o ${NAME}  ${LIBS}

$(BUILD_DIR)%.o:	$(GUI_SRCDIR)%.cpp
	@mkdir -p $(BUILD_DIR)
	${CXX} ${CXXFLAGS} -MMD ${SANITIZE} ${INCLUDE} -c $< -o $@

$(BUILD_DIR)%.o:	$(SRCDIR)%.cpp
	@mkdir -p $(BUILD_DIR)
	${CXX} ${CXXFLAGS} -MMD ${SANITIZE} ${INCLUDE} -c $< -o $@

clean:
				rm -rf ${BUILD_DIR}

fclean:			clean
				rm -f ${NAME}

re:				fclean all

.PHONY:			all clean fclean re

-include $(DEPS)
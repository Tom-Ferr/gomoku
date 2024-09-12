# Directories
SRCDIR			= 	./Gui/src/
GUI_SRCDIR		= 	./src/
BUILD_DIR		= 	./build/
INC				= 	./Gui//includes/
GUI_INC			=	./includes/

# Files
FILES			=	main.cpp BigInt.cpp BoardState.cpp Free_Three_Checker.cpp \
					Game.cpp Heuristics.cpp Mask.cpp Node.cpp GameMessage.cpp Logger.cpp

GUI_FILES		=	Rect.cpp

# Full Paths
SRCS			= 	$(addprefix $(SRCDIR), $(FILES)) $(addprefix $(GUI_SRCDIR), $(GUI_FILES))
OBJS			= 	$(addprefix $(BUILD_DIR), $(notdir ${SRCS:.cpp=.o}))
DEPS 			=	$(patsubst %.o, %.d, $(OBJS))

# Compiler
NAME			=	gomoku
CXX				=	clang++
CXXFLAGS		=	-std=c++11 -Wall -Wextra -Werror
SANITIZE 		=	-g
INCLUDE 		=	-I${INC} -I${GUI_INC}
LIBS 			=	-lgmp -lgmpxx -ldl

# MLX
MLX_INC			=	-I./MLX42/include -I/opt/homebrew/Cellar/glfw/3.4/include
MLX_LIB			=	-L./MLX42/build -L/opt/homebrew/Cellar/glfw/3.4/lib
GMP_INC			=	-I/opt/homebrew/Cellar/gmp/6.3.0/include
GMP_LIB			=	-L/opt/homebrew/Cellar/gmp/6.3.0/lib

UNAME			=	$(shell uname)
LINUX			= -D _LINUX

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
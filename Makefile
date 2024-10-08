# Directories
SRC_DIR			= 	./src
BUILD_DIR		= 	./build
INCLUDE_DIR		= 	./includes
ALGORITHM_DIR	= 	$(addprefix ${SRC_DIR}, /Algorithm)
GUI_DIR			= 	$(addprefix ${SRC_DIR}, /Gui)
SUB_DIRS		= 	${SRC_DIR} ${ALGORITHM_DIR} ${GUI_DIR}

# Files
FILES			=	main.cpp test.cpp

ALGORITHM_FILES	=	BigInt.cpp BoardState.cpp Free_Three_Checker.cpp \
					Game.cpp Heuristics.cpp Mask.cpp Node.cpp GameMessage.cpp

GUI_FILES		=	Gui.cpp Board.cpp Rect.cpp Tile.cpp Color.cpp Menu.cpp \
					Text.cpp Button.cpp ButtonGroup.cpp Info.cpp BoardStatusBar.cpp \
					BoardEndGame.cpp BoardMode.cpp ABoardPopup.cpp Logger.cpp

# Full Paths
SRCS			= 	$(addprefix $(SRC_DIR)/, $(FILES)) \
						$(addprefix $(ALGORITHM_DIR)/, $(ALGORITHM_FILES)) \
						$(addprefix $(GUI_DIR)/, $(GUI_FILES))
OBJS			= 	$(patsubst ${SRC_DIR}/%.cpp, ${BUILD_DIR}/%.o, ${SRCS})
DEPS 			=	$(patsubst ${BUILD_DIR}/%.o, ${BUILD_DIR}/%.d, ${OBJS})
INCLUDE			= 	$(patsubst $(SRC_DIR)%, -I$(INCLUDE_DIR)%, $(SUB_DIRS))



# Compiler
NAME			=	gomoku
CXX				=	clang++
CXXFLAGS		=	-std=c++11 -Wall -Wextra -Werror
SANITIZE 		=	-O3 #-fsanitize=address -g
LIBS 			=	-lmlx42 -lglfw -lgmp -lgmpxx -ldl -pthread -lm


# MLX
MLX_DIR			=	./MLX42
MLX				=	${MLX_DIR}/build/libmlx42.a
MLX_INC			=	-I${MLX_DIR}/include -I/opt/homebrew/Cellar/glfw/3.4/include
MLX_LIB			=	-L${MLX_DIR}/build -L/opt/homebrew/Cellar/glfw/3.4/lib

# GMP
GMP_INC			=	-I/opt/homebrew/Cellar/gmp/6.3.0/include
GMP_LIB			=	-L/opt/homebrew/Cellar/gmp/6.3.0/lib

INCLUDE += $(MLX_INC)
LIBS += $(MLX_LIB)
INCLUDE += $(GMP_INC)
LIBS += $(GMP_LIB)

# Rules

all:		${NAME}

${MLX}:
	@if [ ! -d $(MLX_DIR) ]; then \
		echo "Cloning MLX42..."; \
		git clone https://github.com/codam-coding-college/MLX42.git; \
	fi
	@cd $(MLX_DIR) && \
	if [ ! -d build ]; then \
		echo "Building MLX42..."; \
		cmake -B build; \
		cmake --build build -j4; \
	fi

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

check-glfw:
ifeq ($(shell pkg-config --exists glfw3 && echo yes || echo no), no)
	@echo "glfw3 is not installed. Installing glfw3..."
ifeq ($(shell uname -s), Linux)
	@sudo apt update && sudo sudo apt install -y libglfw3-dev
else ifeq ($(shell uname -s), Darwin)
	@brew install gmp
else
	@echo "Unsupported OS. Please install GMP manually."
	exit 1
endif
else
	@echo "glfw3 is already installed."
endif

check-cmake:
ifeq ($(shell pkg-config --exists glfw3 && echo yes || echo no), no)
	@echo "cmake is not installed. Installing cmake..."
ifeq ($(shell uname -s), Linux)
	@sudo apt update && sudo sudo apt install -y cmake
else ifeq ($(shell uname -s), Darwin)
	@brew install gmp
else
	@echo "Unsupported OS. Please install GMP manually."
	exit 1
endif
else
	@echo "cmake is already installed."
endif

$(NAME):	check-cmake check-glfw check-gmp ${MLX} ${OBJS}
	@${CXX} ${CXXFLAGS} ${SANITIZE} ${OBJS} ${INCLUDE} -o ${NAME}  ${LIBS}
	@echo "\033[96m${NAME} is built. \033[0m"

${BUILD_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p ${@D}
	@${CXX} ${CXXFLAGS} -MMD ${SANITIZE} ${INCLUDE} -c $< -o $@
	@echo "\033[0;32mCompiled $<\033[0m"

clean:
				@rm -rf ${BUILD_DIR}

fclean:			clean
				@rm -f ${NAME}

re:				fclean all

.PHONY:			all clean fclean re

-include $(DEPS)

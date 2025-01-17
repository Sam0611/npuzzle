
SRC = main.cpp Visualizer.cpp parsing.cpp Npuzzle.cpp errors.cpp

OBJ	= ${SRC:.cpp=.o}

NAME = visualizer

CXX = c++

CFLAGS = -Wall -Werror -Wextra

SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

RM = rm -rf

%.o : %.cpp
	${CXX} -c ${CFLAGS} -o $@ $<

all:	${NAME}

${NAME}:	${OBJ}
	${CXX} ${CFLAGS} ${OBJ} -o ${NAME} ${SFML_FLAGS}

clean:
	${RM} ${OBJ}

fclean:	clean
	${RM} ${NAME}

re: fclean ${NAME}

.PHONY:	all clean fclean re
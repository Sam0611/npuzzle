
SRC = main.cpp Npuzzle.cpp Database.cpp parsing.cpp errors.cpp

OBJ	= ${SRC:.cpp=.o}

NAME = npuzzle

DATABASE = database/*

CXX = c++

CFLAGS = -Wall -Werror -Wextra -g

RM = rm -rf

%.o : %.cpp
	${CXX} -c ${CFLAGS} -o $@ $<

all:	${NAME}

${NAME}:	${OBJ}
	${CXX} ${CFLAGS} ${OBJ} -o ${NAME}

clean:
	${RM} ${OBJ}

fclean:	clean
	${RM} ${NAME} ${DATABASE}

re: fclean ${NAME}

.PHONY:	all clean fclean re

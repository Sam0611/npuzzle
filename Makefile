
SRC = main.cpp

all:
	c++ ${SRC} -lsfml-graphics -lsfml-window -lsfml-system

fclean:
	rm -f a.out

re: fclean all
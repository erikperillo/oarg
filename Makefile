NAME = oarg

all: $(NAME).o

$(NAME).o: $(NAME).cpp $(NAME).hpp arg.tpp 
	g++ -c -o $(NAME).o $(NAME).cpp -O3

test: test.cpp $(NAME).o
	g++ -o test test.cpp $(NAME).o -O3

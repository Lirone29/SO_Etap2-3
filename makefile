Snake1: obj obj/main.o obj/fSnakeGame.o
	g++ -g -pthread -rdynamic -lncurses -fpermissive -Wall -pedantic -std=c++11 -o Snake1 obj/main.o obj/fSnakeGame.o

obj:
	mkdir obj

obj/main.o:
	g++ -g -c -pthread -Wall -pedantic -fpermissive -std=c++11 -o obj/main.o main.cpp

obj/fSnakeGame.o:
	g++ -g -c -pthread -Wall -pedantic -fpermissive -std=c++11 -o obj/fSnakeGame.o fSnakeGame.cpp

clean:
	rm -f obj/*.o Snake1
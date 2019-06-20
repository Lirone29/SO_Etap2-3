SO_22: obj obj/main.o obj/forks.o obj/phil.o obj/view.o
	g++ -g -pthread -rdynamic -lncurses -Wall -pedantic -std=c++11 -o SO_22 obj/main.o obj/forks.o obj/phil.o obj/view.o

obj:
	mkdir obj

obj/main.o:
	g++ -g -c -pthread -Wall -pedantic -std=c++11 -o obj/main.o main.cpp

obj/view.o:
	g++ -g -c -pthread -Wall -pedantic -std=c++11 -o obj/view.o view.cpp

obj/phil.o:
	g++ -g -c -pthread -Wall -pedantic -std=c++11 -o obj/phil.o phil.cpp

obj/forks.o:
	g++ -g -c -pthread -Wall -pedantic -std=c++11 -o obj/forks.o forks.cpp

clean:
	rm -f obj/*.o SO_22

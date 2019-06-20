#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <ncurses.h>
#include <signal.h>
#include "Philosopher.h"
using  namespace std;

#define width 80
#define height 24
#define maxWidth 237
#define maxHeight 64

int frequencyOfChanges = 3;


//Variables
int numberOfPhilosophers = 5;
int eatingTime = 7;
int thinkingTime = 5;
int deltaTime = 1;
int endTime = 120;

bool anwserEnd = false;

vector<thread> threads;
vector<Philosopher> philosophers;
char q = 'q';

void* resizedHandler(int sig)
{
    int newHeight, newWidth;

    getmaxyx(stdscr, newHeight, newWidth);

    if(newHeight<maxHeight){
        if(newWidth<maxWidth){
            //good size
        }else wresize(stdscr,newHeight,maxWidth);
    }else{
        if(newWidth<maxWidth){
            wresize(stdscr,maxHeight,newWidth);
        }else wresize(stdscr,maxHeight,maxWidth);
    }
}

int main(int argc, char *argv[]) {

    numberOfPhilosophers = std::stoi(argv[1]);

    cout << "This is philosophersProblem" << endl;
    cout << "Number of Philosophers" << endl;

    cout << numberOfPhilosophers << endl;
    initscr();

    if (has_colors() == false) {
        endwin();
        printf("Your ternimal doesn't support color \n");
        exit(1);
    }

    noecho();
    cbreak();
    wresize(stdscr, height, width);
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    //keypad(stdscr,TRUE);
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_WHITE);
    init_pair(2, COLOR_RED, COLOR_WHITE);
    init_pair(3, COLOR_YELLOW, COLOR_GREEN);
    signal(SIGWINCH, resizedHandler);
    attron(COLOR_PAIR(1));
    refresh();

    for (int i = 0; i < numberOfPhilosophers; i++) {
        philosophers.push_back(Philosopher(i, eatingTime, thinkingTime, endTime, &anwserEnd));
    }

    refresh();

    for (int i = 0; i < numberOfPhilosophers; i++) {
        threads.push_back(std::thread(philosophers[i]));
    }


    while (anwserEnd != true) {
        if (wgetch(stdscr) == q) {
            printw("\nProgram konczy prace");
            attroff(COLOR_PAIR(3));
            anwserEnd = true;
            refresh();
            getch();
            endwin();
        }
    }

    for (auto &t : threads)
        t.join();


    cout << "Program zakonczy dziaanie" << endl;
    return 0;
}
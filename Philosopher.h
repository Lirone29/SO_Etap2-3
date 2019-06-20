//
// Created by eliza on 3/30/19.
//

#ifndef SO_P_PHILOSOPHER_H
#define SO_P_PHILOSOPHER_H

#include <chrono>
#include <iostream>
#include <ncurses.h>
#include <string>

class Philosopher {

private:
    int id;
    int eatingTime;
    int thinkingTime;
    int endTime;
    bool *anwser;

public:
    char getID();
    int getIDINT();
    Philosopher(int id, int eatingTime, int thinkingTime,int endTime,bool *anwser);
    Philosopher();
    void operator()();

};


#endif //SO_P_PHILOSOPHER_H

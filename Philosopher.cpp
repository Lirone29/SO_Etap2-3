//
// Created by eliza on 3/30/19.

#include "Philosopher.h"
#include <iostream>
#include <thread>

using  namespace std;

char Philosopher::getID() {
    return (id+'0' );
}

//TO jest obiekt funkcyjny

Philosopher::Philosopher(int id, int eatingTime, int thinkingTime,int endTime,bool *anwser) {

    this->anwser = anwser;
    this->id = id;
    this->eatingTime = eatingTime;
    this->thinkingTime = thinkingTime;
    this->endTime = endTime;

}

void Philosopher::operator()() {

    std::chrono::time_point<std::chrono::system_clock> end;
    double timeDuration;


    std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

    while (*anwser!= true){
        printw("\nPhilosopher nr %d is THINKING",getIDINT());
        std::this_thread:: sleep_for(std::chrono::seconds(this->thinkingTime));
        printw("\nPhilosopher nr %d is EATING",getIDINT());
        std::this_thread:: sleep_for(std::chrono::seconds(this->eatingTime));
        end = std::chrono::system_clock::now();

        timeDuration = std::chrono::duration_cast<std::chrono::seconds>(end-begin).count();
        if(timeDuration>this->endTime) *anwser = true;
    }

}

Philosopher::Philosopher() {

}

int Philosopher::getIDINT() {
    return this->id;
}


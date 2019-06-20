#ifndef PHIL_H
#define PHIL_H
#include "forks.h"
#include <random>

enum phil_state {
    EAT, THINK, WAIT_L, WAIT_R, NOTHING
};

class phil {
    private:
    int id;
    int dinners_eaten;
    int thinkingTime;
    int eatingTime;
    phil_state state;
    forks *left_fork;
    forks *right_fork;
    void eat();
    void think();
    void request_forks();
    void put_down_forks();

    public:
    phil(int id, forks *_left_fork, forks *_right_fork, int tTime,int eTime);
    phil() {;}
    static bool is_stopped;
    static bool is_started;
    std::string get_state();
    int get_dinners_eaten();
    int get_id() { return id; }
    void dine();
};

#endif
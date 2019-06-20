#include <thread>
#include "phil.h"

bool phil::is_stopped = false;
bool phil::is_started = false;

phil::phil(int _index, forks*_left_fork, forks *_right_fork,int tTime, int eTime) {
    left_fork = _left_fork;
    right_fork = _right_fork;
    id = _index;
    dinners_eaten = 0;
    state = phil_state::THINK;
    thinkingTime = tTime;
    eatingTime = eTime;

}

void phil::dine() {
    while(!is_started) {
        ;
    }
    while(!is_stopped) {
        eat();
        think();
    }
}

void phil::eat() {
    state = WAIT_L;
    left_fork->take(id);
    state = WAIT_R;
    right_fork->take(id);
    state = phil_state::EAT;
    state = EAT;
    std::this_thread::sleep_for(std::chrono::seconds(eatingTime));
    ++dinners_eaten;
    right_fork->put();
    left_fork->put();
}

void phil::think() {
    state = THINK;
    state = phil_state::THINK;
    std::this_thread::sleep_for(std::chrono::seconds(thinkingTime));
}

int phil::get_dinners_eaten() {
    return dinners_eaten;
}

std::string phil::get_state() {
    switch(state) {
        case phil_state::EAT:
            return "eating        ";
        break;
        case phil_state::THINK:
            return "thinking      ";
        break;
        case phil_state::WAIT_L:
            return "wait left     ";
        break;
        case phil_state::WAIT_R:
            return "wait right    ";
            break;
        case phil_state::NOTHING:
            return "ending        ";
        default:
            return "ERROR";
    }
}
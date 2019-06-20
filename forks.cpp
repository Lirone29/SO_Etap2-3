#include "forks.h"

forks::forks(int idTmp) {
    userId = idTmp;
}

int forks::getId() {
    return userId;
}


std::string forks::getIsUsedString() {
    if(isUsed) {
        return "On use  ";
    } else {
        return "Free    ";
    }
}

void forks::take(int newUser) {
    mutex.lock();
    userId = newUser;
    isUsed = true;
}

void forks::put() {
    isUsed = false;
    mutex.unlock();
}
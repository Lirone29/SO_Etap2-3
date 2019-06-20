#ifndef FORK_T_H
#define FORK_T_H
#include <mutex>

class forks {
    private:
    std::mutex mutex;
    int userId;
    bool isUsed;

    public:
    forks(int idTmp);
    int getId();
    std::string getIsUsedString();
    void take(int newUser);
    void put();
};

#endif
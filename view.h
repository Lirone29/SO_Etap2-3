#include "phil.h"
#include "forks.h"
#include <ncurses.h>
#include <thread>
#include <vector>

class view {

    bool answerEnd = false;
    const int philNumber;
    std::thread* philsThreads;
    std::vector<phil> phils;
    std::vector<forks*> forksVector;

    std::thread* visualizerThread;
    WINDOW* philWindow;
    WINDOW* forksWindow;
    int max_x;
    int max_y;
    int margin_x;
    int margin_y;
    int separator;
    void updateForks();
    void updatePhils();
    void updateScreen();
    void initVisualization();
    void initThreads();

    int eatingTime;
    int thinkingTime;
    public:

    ~view();
    view(int philNumber,int tTime, int eTime);
    void endProblem();
};
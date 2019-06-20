#include <iostream>
#include "view.h"
#include <string>


using namespace std;

int main(int argc, char *argv[]) {


    int numberOfPhilosophers = 5;
    int eatingTime = 4;
    int thinkingTime = 5;

    char q = 'q';


    cout << "This is Dining Philosophers Problem" << std::endl;

    numberOfPhilosophers = std::stoi(argv[1]);

    cout << "Set eating time" << std::endl;
    cin>>eatingTime;

    cout<<"Set thinking time"<<endl;
    cin>>thinkingTime;

    view t = view(numberOfPhilosophers,thinkingTime,eatingTime);
    char ans = 'a';
    while (true) {
        ans = wgetch(stdscr);
        if (ans == q){
            t.endProblem();
            break;
        }
    }
    //t.endProblem();
}
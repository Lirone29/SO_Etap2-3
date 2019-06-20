#include "view.h"
#include <iostream>
#include <unistd.h>
#include "forks.h"
#include <thread>

view::view(int philN,int tTime, int eTime) :philNumber( philN) {
    this->thinkingTime = tTime;
    this->eatingTime = eTime;
    philsThreads= new std::thread[philN];
    initVisualization();
    initThreads();
}

void view::initThreads() {
    forksVector.reserve(philNumber);
    for(int i=0; i < philNumber; ++i) {
        forksVector.push_back(new forks(i));
    }

    phils.reserve(philNumber);
    for(int i=0; i < philNumber; ++i) {
        phils.push_back(phil(i, std::ref(forksVector[i]),std::ref((forksVector[(i+1)%philNumber])),this->thinkingTime, this->eatingTime));
        philsThreads[i] = std::thread(&phil::dine, std::ref(phils[i]));
    }
    visualizerThread = new std::thread(&view::updateScreen,this);
    phil::is_started = true;
}

void view::initVisualization() {
    initscr();
    noecho();
    box(stdscr,0,0);
    use_default_colors();
    start_color();
    
    getmaxyx(stdscr, max_y, max_x);
    margin_x = 0.07 * (float) max_x;
    margin_y = 0.13 * (float) max_y;
    separator = (float) (max_x) * 0.5;
    int column_width = (float)(separator-margin_x)/3;
    init_pair(9, COLOR_GREEN, -1);
    wattron(stdscr,COLOR_PAIR(9));
    mvwprintw(stdscr,max_y-margin_y/2,margin_x + separator- 18,"SO Project. Press q to end!");
    wattroff(stdscr,COLOR_PAIR(9));
    refresh();

    philWindow = derwin(stdscr,max_y-2*margin_y, separator-margin_x,margin_y,margin_x);
    box(philWindow,0,0);

    wprintw(philWindow,"Philosophs");

    mvwprintw(philWindow,2,3,"Id: ");
    mvwprintw(philWindow,2,column_width,"States: ");
    mvwprintw(philWindow,2,2*column_width+2,"Dinners: ");
    mvwhline(philWindow, 4, 1, 0 ,separator-margin_x-2);
    
    mvwvline(philWindow,1,column_width-2,0,max_y-2*margin_y-2);
    mvwvline(philWindow,1,2*1.1*column_width-2,0,max_y-2*margin_y-2);

    touchwin(philWindow);
    wrefresh(philWindow);

    column_width = (float)(max_x-separator-margin_x)/3;
    forksWindow = derwin(stdscr,max_y-2*margin_y, 3*column_width,margin_y,separator);
    box(forksWindow,0,0);
    wprintw(forksWindow,"Forks");

    mvwprintw(forksWindow,2,2,"Id");
    mvwprintw(forksWindow,2,column_width,"State: ");
    mvwprintw(forksWindow,2,2*column_width,"User: ");

    mvwhline(forksWindow, 4, 1, 0 ,max_x-separator-margin_x-4);
    mvwvline(forksWindow,1,column_width-2,0,max_y-2*margin_y-2);
    mvwvline(forksWindow,1,2*column_width-2,0,max_y-2*margin_y-2);

    touchwin(forksWindow);
    wrefresh(forksWindow);
}

void view::updatePhils(){
    int column_width = (float)(separator-margin_x)/3;
    char buf[16];
    const char* p;
    init_pair(1, COLOR_CYAN, -1);
    init_pair(2, COLOR_MAGENTA, -1);
    init_pair(3, COLOR_BLUE, -1);
    for(int i=0; i<philNumber; ++i) {
        sprintf(buf,"%d", phils[i].get_id());
        p = buf;
        mvwprintw(philWindow, 2*i+5,3,"%s",p);
        std::string state = phils[i].get_state();
        if(state == "eating        ") {
            wattron(philWindow,COLOR_PAIR(1));
            mvwprintw(philWindow, 2*i+5,column_width+1,"%s",state.c_str());
            wattroff(philWindow,COLOR_PAIR(1));
        } else if(state == "thinking      ") {
            wattron(philWindow,COLOR_PAIR(2));
            mvwprintw(philWindow, 2*i+5,column_width+1,"%s",state.c_str());
            wattroff(philWindow,COLOR_PAIR(2));
        } else {
            wattron(philWindow,COLOR_PAIR(3));
            mvwprintw(philWindow, 2*i+5,column_width+1,"%s",state.c_str());
            wattroff(philWindow,COLOR_PAIR(3));
        }
        sprintf(buf,"%d", phils[i].get_dinners_eaten());
        p = buf;
        mvwprintw(philWindow, 2*i+5,2*1.1*column_width+3,"%s",p);
    }
    touchwin(philWindow);
    wrefresh(philWindow);
}

void view::updateForks() {
    int column_width = (float)(max_x-separator-margin_x)/3;
    char buf[16];
    const char* p;

    init_pair(4, COLOR_YELLOW, -1);
    init_pair(10, COLOR_WHITE, -1);

    for(int i=0; i<philNumber; ++i) {
        sprintf(buf,"%d", forksVector[i]->getId());
        p = buf; 
        mvwprintw(forksWindow, 2*i+5,3,"%s",p);
        std::string state = forksVector[i]->getIsUsedString();
        if(state == "On use  ") {
            wattron(forksWindow,COLOR_PAIR(4));
            mvwprintw(forksWindow, 2*i+5,column_width+1,"%s",state.c_str());
            wattroff(forksWindow,COLOR_PAIR(4));
        }else
            if(state == "Free    "){
                wattron(forksWindow,COLOR_PAIR(10));
                mvwprintw(forksWindow, 2*i+5,column_width+1,"%s",state.c_str());
                wattroff(forksWindow,COLOR_PAIR(10));
            }
        sprintf(buf,"%d", forksVector[i]->getId());
        p = buf;
        mvwprintw(forksWindow, 2*i+5,2*column_width+3,"%s",p);
    }
    touchwin(forksWindow);
    wrefresh(forksWindow);
}

void view::updateScreen() {
    while(!phil::is_stopped) {
        updatePhils();
        updateForks();
        sleep(1);
        refresh();
    }
}

view::~view() {
   delwin(forksWindow);
   delwin(philWindow);
   endwin();
}


void view::endProblem() {

    mvwprintw(stdscr,max_y-margin_y/2 +2,margin_x + separator- 10, "Program konczy prace");
    phil::is_stopped = true;
    refresh();
    endwin();

    for(int i=0; i < philNumber; ++i) {
        philsThreads[i].join();

    }
}

#ifndef APP_H
#define APP_H

#include <vector>
#include <string>
#include <ncurses.h>

class Application {
private:
    std::vector<std::string> choices = {
        "Create Watch List",
        "Add a watch",
        "Remove a watch",
        "Watch Stats",
        "Exit"
    };
    int num_choices = choices.size();
    int highlight = 0;
    int ymax, xmax = 0;
    int choice = 0;
public:
    Application();
    ~Application();
    void Main_Menu();
};

#endif
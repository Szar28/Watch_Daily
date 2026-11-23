#ifndef DATABASE_H
#define DATABASE_H

#include "constants/control_constants.h"
#include <ncurses.h>
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include "settings.h"

class Database {
private: 
    std::vector<std::string> days = {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
    };
    char watchName[CHARACTER_LIMIT]= "";
public:
    Database();
    ~Database();
    void Add_Watch(WINDOW* window);
    void Remove_Watch(WINDOW* window);
    void Display_Database(WINDOW* window);
    void Create_List(WINDOW* window, Settings* settings);
    void View_List(WINDOW* window, Settings* settings);
};

#endif

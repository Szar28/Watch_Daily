#ifndef DATABASE_H
#define DATABASE_H

#include "constants/control_constants.h"
#include <ncurses.h>
#include <sqlite3.h>
#include <iostream>

class Database {
private: 
    char watchName[CHARACTER_LIMIT]= "";
    int ymax = 0;
    int xmax = 0;
public:
    Database();
    ~Database();
    void Add_Watch(WINDOW* window);
    void Remove_Watch();
    int Add_To_Database(char name[CHARACTER_LIMIT]);
    void Remove_Database();
};

#endif
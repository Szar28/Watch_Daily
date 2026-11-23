#ifndef SETTINGS_H
#define SETTINGS_H

#include <ncurses.h>
#include <fstream>
#include "constants/control_constants.h"
#include <unordered_map>
#include <iostream>

class Settings {
private:
    std::vector<std::string> choices = {
        "View Current Settings",
        "Change List Mode",
        "Change List Length",
        "Exit"
    };
    int highlight = 0;
    int choice = 0;
    int count = 0;
    enum Mode currentMode;
    char userChoice[CHARACTER_LIMIT]= "";
public:
    Settings();
    ~Settings();
    int Get_Count();
    enum Mode Get_Current_Mode();
    void Read_In_Settings();
    void Write_To_Settings_File();
    void Display_Setting(WINDOW* window, WINDOW* window2);
    void View_Current_Settings(WINDOW* window);
    void Change_Mode(WINDOW* window);
    void Change_List_Length(WINDOW* window);
};
#endif
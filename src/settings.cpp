#include "settings.h"

Settings::Settings() {};

Settings::~Settings() {};

int Settings::Get_Count() {
    return count;
}
enum Mode Settings::Get_Current_Mode() {
    return currentMode;
}

/*  First number will be the COUNT
    Second number will be the TYPE (Amount/Week)
*/
void Settings::Read_In_Settings() {

    std::unordered_map<std::string, Mode> modeMap {
        {"COUNT", Mode::COUNT},
        {"WEEK", Mode::WEEK}
    };

    int row = 0;
    std::string myText;
    std::ifstream fs("settings.txt");

    while (getline(fs, myText)) {
        if (row == 0) {
            count = std::stoi(myText);
        } else {
            auto it = modeMap.find(myText);
            currentMode = it->second;
        }
        row++;
    }
    fs.close();
}

void Settings::Write_To_Settings_File() {
    int row = 0;
    std::ofstream fs("settings.txt");

    while (row < MAX_SETTINGS) {
        if (row == 0) {
            fs << count << "\n";
        } else {
            if (currentMode == COUNT) {
                fs << "COUNT" << "\n"; 
            } else if (currentMode == WEEK) {
                fs << "WEEK" << "\n";
            } else {
                fs << "COUNT" << "\n"; 
            }
        }
        row++;
    }
    fs.close();
}

void Settings::View_Current_Settings(WINDOW* window) {
    werase(window);
    box(window, 0, 0);
    mvwprintw(window, 2, 3, "Count: %d", count);
    if (currentMode == COUNT) {
        mvwprintw(window, 3, 3, "Mode: Count");    
    } else {
        mvwprintw(window, 3, 3, "Mode: Week");
    }
    wrefresh(window);
}

void Settings::Display_Setting(WINDOW* window, WINDOW* window2) {
    werase(window);
    box(window, 0, 0);
    wrefresh(window);
    
    while (true) {
        wrefresh(window);
        for (int i = 0; i < choices.size(); i++) {
            if (i == highlight) {
                wattron(window, A_REVERSE);
            }
            mvwprintw(window, i + 2, 3, choices[i].c_str());
            wattroff(window, A_REVERSE);
	    }
        choice = wgetch(window);

        switch(choice) {
            case KEY_UP:
                if (highlight > 0) {
                    highlight--;
                }
                break;
            case KEY_DOWN:
                if (highlight < choices.size() - 1) {
                    highlight++;
                }
                break;
            default:
                break;
        }
        if (choice == CONTROL_CONSTANTS::ENTER_KEY) {
            if (highlight == 0) {
                View_Current_Settings(window2);
            } else if (highlight == 1) {
                werase(window2);
                box(window2, 0, 0);
                mvwprintw(window2, 2, 3, "Change the Mode");
                wrefresh(window2);
            } else if (highlight == 2) {
                Change_List_Length(window2);
            } else if (highlight == 3) {
                werase(window2);
                werase(window);
                box(window2, 0, 0);
                box(window, 0, 0);
                wrefresh(window2);
                wrefresh(window);
                break;
            }
        }
    }
}

/* Max Length of the list currently should be 15 before it starts
   getting wonky behavior.
*/
void Settings::Change_List_Length(WINDOW* window) {
    echo();
    werase(window);
    box(window, 0, 0);

    mvwprintw(window, 2, 3, "Enter the list Watch List Length: ");
    wrefresh(window);
    mvwgetstr(window, 2, 38, userChoice);

    try {
        count = std::stoi(userChoice);
        if (count > MAX_ROWS) {
            mvwprintw(window, 3, 3, "Valid greater than max row length of %d", MAX_ROWS);
            wrefresh(window);
            noecho();
            return;
        }
    } catch (const std::invalid_argument& e) {
        mvwprintw(window, 3, 3, "Input must be numeric.");
        wrefresh(window);
        noecho();
        return;
    }

    mvwprintw(window, 3, 3, "Watch list length was set to: %s", userChoice);
    wrefresh(window);
    noecho();
}

void Settings::Change_Mode(WINDOW* window) {

}

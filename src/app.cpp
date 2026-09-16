#include "app.h"
#include "database.h"

Application::Application() {};

Application::~Application() {};

void Application::Main_Menu() {

    Database theBase;

    initscr();
    noecho();
    cbreak();
    getmaxyx(stdscr, ymax, xmax);
    box(stdscr, 0, 0);

    WINDOW* menuWin = newwin(num_choices+4, xmax-2, ymax-(num_choices+4), 1);
    WINDOW* contentWin = newwin(ymax-(num_choices+4), xmax-2, 1, 1);
    box(menuWin, 0, 0);
    box(contentWin, 0, 0);
    refresh();
    wrefresh(menuWin);
    wrefresh(contentWin);
    keypad(menuWin, true);

    while (true) {
        wrefresh(menuWin);
        for (int i = 0; i < choices.size(); i++) {
            if (i == highlight) {
                wattron(menuWin, A_REVERSE);
            }
            mvwprintw(menuWin, i + 2, 3, choices[i].c_str());
            wattroff(menuWin, A_REVERSE);
	    }
        choice = wgetch(menuWin);

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
        
        /* TODO:
            Make it so that it will point to other parts of the program.
            We can start by creating a one that allows us to create the
            database with SQLite so were able to actually store the
            watches.
        */
        if (choice == CONTROL_CONSTANTS::ENTER_KEY) {
            if (highlight == 0) {
                theBase.Add_Watch(contentWin);
            } else if (highlight ==1) {
                werase(contentWin);
                box(contentWin, 0, 0);
                mvwprintw(contentWin, 2, 3, "You chose: %s", choices[highlight].c_str());
                wrefresh(contentWin);
            }else if (highlight == 4) {
                
                werase(contentWin);
                box(contentWin, 0, 0);
                mvwprintw(contentWin, 2, 3, "You chose: %s", choices[highlight].c_str());
                wrefresh(contentWin);
                break;
            }
        }
    }

    getch();
    endwin();
};
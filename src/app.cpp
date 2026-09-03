#include "app.h"

Application::Application() {};

Application:: ~Application() {};

void Application::Main_Menu() {

    initscr();
    noecho();
    cbreak();
    getmaxyx(stdscr, ymax, xmax);
    box(stdscr, 0, 0);

    WINDOW* menuwin = newwin(num_choices+4, xmax-2, ymax-(num_choices+4),1);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);
    keypad(menuwin, true);

    while (true) {
        for (int i = 0; i < choices.size(); i++) {
            if (i == highlight) {
                wattron(menuwin, A_REVERSE);
            }
            mvwprintw(menuwin, i + 2, 3, choices[i].c_str());
            wattroff(menuwin, A_REVERSE);
	    }
        choice = wgetch(menuwin);

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
            break;
        }
    }

    move(2, 3);
    printw("You chose: %s", choices[highlight].c_str());
    getch();
    endwin();
};
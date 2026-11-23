#include "database.h"

Database::Database() {};

Database::~Database() {};

void Database::Add_Watch(WINDOW* window) {

    echo();
    werase(window);
    box(window, 0, 0);

    mvwprintw(window, 2, 3, "Enter a watch name: ");
    wrefresh(window);
    mvwgetstr(window, 2, 23, watchName);

    mvwprintw(window, 3, 3, "The watch name: %s", watchName);
    wrefresh(window);
    noecho();

    sqlite3* db = nullptr;
    char* errMessage = nullptr;

    int rc = sqlite3_open("watch.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMessage);
    }

    const char* sqlCreate = "CREATE TABLE IF NOT EXISTS watches ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "name TEXT NOT NULL);";
    
    rc = sqlite3_exec(db, sqlCreate, nullptr, nullptr, &errMessage);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMessage);
    }

    const char* sqlInsert = "INSERT INTO watches (name) VALUES (?);";
    sqlite3_stmt* stmt = nullptr; 
    
    rc = sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMessage);
    }

    std::string str(watchName);
    sqlite3_bind_text(stmt, 1, str.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_free(errMessage);
    } else {
        mvwprintw(window, 4, 3, "%s was entered successfully!", watchName);
    }

    wrefresh(window);
    sqlite3_close(db);
}

void Database::Remove_Watch(WINDOW* window) {

    int id = 0;
    sqlite3* db = nullptr;
    char* errMessage = nullptr;
    int rc = sqlite3_open("watch.db", &db);

    echo();
    werase(window);
    box(window, 0, 0);

    sqlite3_stmt *stmt = nullptr;
    Display_Database(window);

    watchName[CHARACTER_LIMIT-1] = {0};
    mvwprintw(window, 2, 2, "Enter ID of watch to remove: ");
    wrefresh(window);
    mvwgetstr(window, 2, 31, watchName);
    noecho();

    const char* sqlDelete = "DELETE FROM watches WHERE id = (?)";
    rc = sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMessage);
    }

    //Bind the id to the statement
    try {
        id = std::stoi(watchName);
    } catch (const std::invalid_argument& e) {
        mvwprintw(window, 3, 2, "ID must be numeric.");
        wrefresh(window);
        sqlite3_close(db);
        return;
    } catch (const std::out_of_range& e) {
        mvwprintw(window, 3, 2, "ID out of range.");
        wrefresh(window);
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        mvwprintw(window, 3, 2, "There was an error");
    } else {
        mvwprintw(window, 3, 2, "Watch deleted successfully!");
    }
    wrefresh(window);
    sqlite3_close(db);
}

void Database::Display_Database(WINDOW* window) {
    sqlite3* db = nullptr;
    char* errMessage = nullptr;
    int rc = sqlite3_open("watch.db", &db);

    werase(window);
    box(window, 0, 0);

    mvwprintw(window, 2, 2, " Database entries ");
    mvwprintw(window, 3, 2, "  ID \t \t NAME ");
    mvwprintw(window, 4, 2, " ----\t \t------");

    sqlite3_stmt *stmt = nullptr;
    // Displays the rows for the from the database
    rc = sqlite3_prepare_v2(db, "SELECT id, name from watches ORDER BY id", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        int row = 5;
        while(row <= MAX_DISPLAY_ROWS && sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char *name = (const char *)sqlite3_column_text(stmt, 1);
            mvwprintw(window, row, 4, "%-6d\t%s", id, name ? name : NULL);
            wrefresh(window);
            row++;
        }
        sqlite3_finalize(stmt);
    } else {
        sqlite3_free(errMessage);
    }
    sqlite3_close(db);
}

void Database::Create_List(WINDOW* window, Settings* settings) {
    sqlite3* db = nullptr;
    char* errMessage = nullptr;
    int rc = sqlite3_open("watch.db", &db); 

    werase(window);
    box(window, 0, 0);
    
    std::ofstream outfile("list.txt");
    mvwprintw(window, 2, 2, "Creating list...");
    sqlite3_stmt *stmt = nullptr;

    //picks a watch at random
    for (int i = 0; i < settings->Get_Count(); i++) {
        rc = sqlite3_prepare_v2(db, "SELECT id, name FROM watches ORDER BY RANDOM() LIMIT 1", -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt);
            const char *name = (const char *)sqlite3_column_text(stmt, 1);
            outfile << name << std::endl;
        } else {
            sqlite3_free(errMessage);
        }
    }
    
    mvwprintw(window, 3, 2, "List was created");
    wrefresh(window);
    outfile.close();
}

void Database::View_List(WINDOW* window, Settings* settings) {
    int row = 0;
    std::string name;
    werase(window);
    box(window, 0, 0);

    std::ifstream outfile("list.txt");
    mvwprintw(window, 2, 2, "Current List");
    mvwprintw(window, 3, 2, "------------");

    if (settings->Get_Current_Mode() == WEEK) {
        for (std::string day: days) {
            getline(outfile, name);
            mvwprintw(window, row+4, 4, "%s: %s", day.c_str(), name.c_str());
            row++;
        }
    } else if (settings->Get_Current_Mode() == COUNT) {
        while (std::getline(outfile, name) && row < settings->Get_Count()) {
            mvwprintw(window, row+4, 4, "Day %d: %s", row+1, name.c_str());
            row++;
        }
    }

    wrefresh(window);
}
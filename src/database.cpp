#include "database.h"

Database::Database() {};

Database::~Database() {};

void Database::Add_Watch(WINDOW* window) {

    int code = 0;

    echo();
    werase(window);
    box(window, 0, 0);

    mvwprintw(window, 2, 3, "Enter a watch name: ");
    wrefresh(window);
    mvwgetstr(window, 2, 23, watchName);

    mvwprintw(window, 3, 3, "The watch name: %s", watchName);
    wrefresh(window);
    noecho();

    code = Add_To_Database(watchName);
    if (code == 0) {
        mvwprintw(window, 4, 3, "%s was entered successfully!", watchName);
    } else {
        mvwprintw(window, 4, 3, "%s was not successful.", watchName);
    }
    wrefresh(window);
}

int Database::Add_To_Database(char name[CHARACTER_LIMIT]) {
    sqlite3* db = nullptr;
    char* errMessage = nullptr;

    int rc = sqlite3_open("watch.db", &db);
    if (rc != SQLITE_OK) {
        return rc;
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

    std::string str(name);
    sqlite3_bind_text(stmt, 1, str.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        return -1;
    }
    return 0;
}
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

    sqlite3_close(db);
    return 0;
}

void Database::Remove_Watch(WINDOW* window) {

    int id = 0;
    sqlite3* db = nullptr;
    char* errMessage = nullptr;
    int rc = sqlite3_open("watch.db", &db);

    echo();
    werase(window);
    box(window, 0, 0);

    mvwprintw(window, 2, 2, " Database entries ");
    mvwprintw(window, 3, 2, "  ID \t \t NAME ");
    mvwprintw(window, 4, 2, " ----\t \t------");

    sqlite3_stmt *stmt = nullptr;
    // Displays the rows for the from the database
    if (sqlite3_prepare_v2(db, "SELECT id, name from watches ORDER BY id", -1, &stmt, NULL) == SQLITE_OK) {
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
        mvwprintw(window, MAX_DISPLAY_ROWS + 4, 4, "Error: %s", sqlite3_errmsg(db));
    }

    mvwprintw(window, MAX_DISPLAY_ROWS + 4, 2, "Enter ID of watch to remove: ");
    wrefresh(window);
    mvwgetstr(window, MAX_DISPLAY_ROWS + 4, 31, watchName);
    noecho();

    const char* sqlDelete = "DELETE FROM watches WHERE id = (?)";
    rc = sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMessage);
    }

    //Bind the id to the statement
    id = std::stoi(watchName);
    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        mvwprintw(window, MAX_DISPLAY_ROWS + 8, 2, "There was an error");
    } else {
        mvwprintw(window, MAX_DISPLAY_ROWS + 8, 2, "Watch deleted successfully!");
    }
    wrefresh(window);
    sqlite3_close(db);
}
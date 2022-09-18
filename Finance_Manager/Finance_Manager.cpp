#include <iostream>
#include <fstream>
#include <iomanip>
#include "sqlite/sqlite3.h"

constexpr auto filename_len = 100;

class user {
public:
    user() {
        std::cout << "User Database" << std::endl;
    }
};

class sqlite_func {
private:
    std::string filename;

public:
    typedef std::string sql_comm;
    typedef int ind;

    sqlite_func(std::string fname) {
        filename = "../Database/" + fname + ".db";
        CreateDB();
    }
    //Create Database
    void CreateDB() {
        std::fstream DbFile;
        DbFile.open(filename, std::ios_base::out);
        if (!DbFile.is_open()) {
            std::cerr << "ERR: File Not Created: " << filename << std::endl;
        }
        else {
            std::cout << "MSG: File Created: " << filename << std::endl;
        }
    }
    //Create Table 
    void CreateTable(sql_comm cTable) {
        sqlite3* db;
        sqlite3_stmt* stmt;
        ind connect, prepare;
        connect = sqlite3_open(filename.c_str(), &db);
        if (connect) {
            std::cout << "ERR: Database Not Connected: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
        }
        else {
            std::cout << "MSG: Database Connected" << std::endl;
            prepare = sqlite3_prepare_v2(db, cTable.c_str(), -1, &stmt, NULL);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            if (prepare != SQLITE_OK) {
                std::cout << "ERR: Table Not Created" << sqlite3_errmsg(db) << std::endl;
                sqlite3_close(db);
            }
            else {
                std::cout << "MSG: Table Created" << std::endl;
                sqlite3_close(db);
            }
        }
    }
    //Retrieve Data
    void ReadDB(sql_comm rTable) {
        sqlite3* db;
        sqlite3_stmt* stmt;
        ind connect, prepare;
        connect = sqlite3_open(filename.c_str(), &db);
        if (connect) {
            std::cout << "ERR: Database Not Connected: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
        }
        else {
            std::cout << "MSG: Database Connected" << std::endl;
            prepare = sqlite3_prepare_v2(db, rTable.c_str(), -1, &stmt, NULL);
            if (prepare != SQLITE_OK) {
                std::cout << "ERR: Records Not Retrieved" << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                sqlite3_close(db);
            }
            else {
                std::cout << "MSG: Records Retrieved Successfully" << std::endl;
                std::cout << std::left << std::setw(15) << "ID" << std::setw(25) << "Name" << std::setw(25) << "Password"
                    << std::setw(25) << "Email" << std::setw(25) << "Course" << std::setw(25) << "Section" << std::endl;
                while (true) {
                    prepare = sqlite3_step(stmt);
                    if (prepare == SQLITE_ROW) {
                        std::cout << std::left << std::setw(15) << sqlite3_column_text(stmt, 0)
                            << std::setw(25) << sqlite3_column_text(stmt, 1)
                            << std::setw(25) << sqlite3_column_text(stmt, 2)
                            << std::setw(25) << sqlite3_column_text(stmt, 3)
                            << std::setw(25) << sqlite3_column_text(stmt, 4)
                            << std::setw(25) << sqlite3_column_text(stmt, 5) << std::endl;
                    }
                    else {
                        break;
                    }
                }
                sqlite3_finalize(stmt);
                sqlite3_close(db);
            }
        }
    }
};

int main()
{
    sqlite_func user1("user_list");
    std::string sql = "CREATE TABLE Users(NAME TEXT NOT NULL, PASSWORD TEXT NOT NULL,EMAIL TEXT NOT NULL);";
    user1.CreateTable(sql);
    std::string sql = "SELECT rowid,* FROM  Student;";

    return 0;
}

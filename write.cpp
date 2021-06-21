#include <fstream>
#include <Windows.h>

void write(std::string text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text;
    file.close();
}

void write(char text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text;
    file.close();
}

void write(char *text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text;
    file.close();
}

void write(const char *text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text;
    file.close();
}

void write(LPWSTR text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text;
    file.close();
}

void write_endl(std::string text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text << std::endl;
    file.close();
}

void write_endl(char *text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text << std::endl;
    file.close();
}

void write_endl(const char *text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text << std::endl;
    file.close();
}

void write_endl(LPWSTR text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text << std::endl;
    file.close();
}

void write_pendl(std::string text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << std::endl << text;
    file.close();
}

void write_pendl(char text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text;
    file.close();
}

void write_pendl(char *text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text;
    file.close();
}

void write_pendl(const char *text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text;
    file.close();
}

void write_pendl(LPWSTR text, std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << text;
    file.close();
}

void wnewline(std::string filename){
    std::ofstream file(filename, std::ios::app);
    file << std::endl;
    file.close();
}
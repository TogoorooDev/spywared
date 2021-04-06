#include <fstream>

void write(std::string text){
    std::ofstream file("keys.txt", std::ios::app);
    file << text;
    file.close();
}

void write(char text){
    std::ofstream file("keys.txt", std::ios::app);
    file << text;
    file.close();
}

void write_endl(std::string text){
    std::ofstream file("keys.txt", std::ios::app);
    file << text << std::endl;
    file.close();
}

void wnetline(){
    std::ofstream file("keys.txt", std::ios::app);
    file << std::endl;
    file.close();
}
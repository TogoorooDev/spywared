#ifndef WRITE_H
#define WRITE_H

#include <iostream>

void write(std::string, std::string);
void write(char, std::string);
void write(char*, std::string);
void write(const char*, std::string);
void write(LPWSTR, std::string);

void write_endl(std::string, std::string);
void write_endl(char*, std::string);
void write_endl(const char*, std::string);
void write_endl(LPWSTR, std::string);

void write_pendl(std::string, std::string);
void write_pendl(char, std::string);
void write_pendl(char*, std::string);
void write_pendl(const char*, std::string);
void write_pendl(LPWSTR, std::string);

void wnewline(std::string);

#endif
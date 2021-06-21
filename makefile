CXXFLAGS = -Wall -Wextra -pedantic -std=c++17

#klog.exe: main.o write.o chrome.o capt.o sqlite3.o
#	g++ $(CXXFLAGS) main.o write.o chrome.o sqlite3.o -o klog.exe

klog.exe: main.o write.o capt.o sqlite3.o
	g++ $(CXXFLAGS) main.o write.o sqlite3.o -o klog.exe


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
	rm -f capt.o main.o chrome.o write.o sqlite3.o klog.exe

dellogs:
	rm -f keys.txt history.txt
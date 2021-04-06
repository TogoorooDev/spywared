CXXFLAGS = -Wall -Wextra -pedantic

klog.exe: main.o write.o
	g++ $(CXXFLAGS) main.o write.o -o klog.exe

write.o: write.cpp write.h
	g++ $(CXXFLAGS) -c write.cpp

main.o: main.cpp
	g++ $(CXXFLAGS) -c main.cpp
CC=g++
CCFLAGS=-std=c++11 -pedantic -Wall
CINCS=-I/usr/include/qt5/ -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtWidgets -I/usr/include/qt5/QtCore

MOC=/usr/lib64/qt5/bin/moc
UIC=/usr/lib64/qt5/bin/uic

LK=g++
LKFLAGS=
LIBS=-L/usr/lib64/ -lQt5Core -lQt5Gui -lQt5Widgets


OBJ=main.o lcdclock.o moc_lcdclock.o

lcdclock: $(OBJ)
	$(LK) $(LKFLAGS) $(OBJ) $(LIBS) -o lcdclock

all : clean lcdclock


main.o : main.cpp
	$(CC) -c $(CCFLAGS) $(CINCS) main.cpp -o main.o

lcdclock.o : lcdclock.cpp lcdclock.h
	$(CC) -c $(CCFLAGS) $(CINCS) lcdclock.cpp -o lcdclock.o

moc_lcdclock.o : lcdclock.h
	$(MOC) lcdclock.h -o moc_lcdclock.cpp
	$(CC) -c $(CCFLAGS) $(CINCS) moc_lcdclock.cpp -o moc_lcdclock.o

clean:
	rm -f *.o
	rm -f moc_lcdclock.*
	rm lcdclock

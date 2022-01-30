#include "lcdclock.h"
#include <QApplication>
#include <QDir>

#include <iostream>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

void allocConsole();

int main(int argc, char *argv[])
{
    //allocConsole();         // give us a console for debugging....fuck those idiots at microsoft.

    QString path = QDir::currentPath();
    if (path.isEmpty() && path.isNull())
        QCoreApplication::addLibraryPath("C:/Program Files/LCD CLock/plugins");
    else
    {
        QCoreApplication::addLibraryPath(path);
    }

    QApplication a(argc, argv);    
    lcdClock w;
    w.show();
    return a.exec();
}


// printing debugging information, status information is common and helpful...
// except if you are brain-dead idiots from Microsoft who disable this stuff
// So, lets fix this.  (taken from Adding Console I/O to a Win32 GUI App,
// Windows Developer Journal, Dec 1997; and updated from 
// https://smacdo.com/programming/redirecting-standard-output-to-console-in-windows/)
//
void allocConsole()
{
    // Create the console window and set the window title.
    if (AllocConsole() == 0)
    {
        // Handle error here. Use ::GetLastError() to get the error.
    }

    // Redirect CRT standard input, output and error handles to the console window.
    FILE * pNewStdout = nullptr;
    FILE * pNewStderr = nullptr;
    FILE * pNewStdin = nullptr;

    ::freopen_s(&pNewStdout, "CONOUT$", "w", stdout);
    ::freopen_s(&pNewStderr, "CONOUT$", "w", stderr);
    ::freopen_s(&pNewStdin, "CONIN$", "r", stdin);

    // Clear the error state for all of the C++ standard streams. Attempting to accessing the streams before they refer
    // to a valid target causes the stream to enter an error state. Clearing the error state will fix this problem,
    // which seems to occur in newer version of Visual Studio even when the console has not been read from or written
    // to yet.

    std::cout.clear();
    std::cerr.clear();
    std::cin.clear();

    std::wcout.clear();
    std::wcerr.clear();
    std::wcin.clear();
}




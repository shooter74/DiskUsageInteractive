#ifndef H_Display
#define H_Display

#include <iostream>
#include <string>

#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO

class Display
{
    public:
        Display();

        unsigned int Rows() const;
        unsigned int Cols() const;

        /// Returns a winsize structure with size.ws_row is the number of rows, size.ws_col is the number of columns.
        static winsize GetTerminalSize();

        /// Clears the whole screen.
        static void ClearScreen();

    protected:
        unsigned int screenRows;//<! Number of rows currently displayed in the screen
        unsigned int screenCols;//<! Number of columns currently displayed in the screen
};

/// Creates a progress bar
std::string GenerateProgressBar(unsigned int width, unsigned int current, unsigned int total, bool showPercentage = true, std::string const& fillChar = "\u25A0");

#endif

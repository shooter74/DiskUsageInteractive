#ifndef H_Display
#define H_Display

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO

#include <AnsiTerminal.hpp>

class Display
{
    public:
        Display();

        unsigned int Rows() const;
        unsigned int Cols() const;

        /// Returns a constant reference to a given line of the screen.
        std::string const& GetLine(unsigned int i) const;
        
        /// Sets the line i to str. /!\ Warning : the line will be truncated at its max length of Cols().
        void SetLine(unsigned int i, std::string const& str);

        /// Sets the line i to str and centers it horizontally on the screen. /!\ Warning : the line will be truncated at its max length of Cols().
        void SetLineCentered(unsigned int i, std::string const& str, char fillChar = ' ');

        /// Sets the line i to str and justifies it to the right of the screen. /!\ Warning : the line will be truncated at its max length of Cols().
        void SetLineRjustified(unsigned int i, std::string const& str, char fillChar = ' ');

        /// Get reference to "pixel" at row i, column j.
        char & GetPixel(unsigned int i, unsigned int j);

        /// Clears all the screen lines stored within the object and recreates new empty lines of the right size.
        void ClearScreenLines();

        /// Draws all the screen lines stored within the object. You must call ClearScreen() in order to draw in-place.
        void DrawScreenLines();

        /// Updates the screen size stored internally.
        void UpdateScreenSize();

        /// Highlights the line i by inverting its colors (black on white background).
        void HighlightLine(unsigned int i, bool highlight);

        /// Returns a winsize structure with size.ws_row is the number of rows, size.ws_col is the number of columns.
        static winsize GetTerminalSize();

        /// Clears the whole screen.
        static void ClearScreen();

    protected:
        unsigned int screenRows;//<! Number of rows currently displayed in the screen
        unsigned int screenCols;//<! Number of columns currently displayed in the screen

        std::vector<std::string> lines;//<! Stores the lines of the screen
};

/// Returns a left-justified string containing str, with fillChar used to fill the blank space : "bla", 7 -> "bla    ". If the string is longer than width, it is truncated.
std::string LeftJustify(std::string const& str, unsigned int width, char fillChar = ' ');

/// Returns a centered string containing str, with fillChar used to fill the blank space : "bla", 7 -> "  bla  ". If the string is longer than width, it is truncated.
std::string CenterString(std::string const& str, unsigned int width, char fillChar = ' ');

/// Returns a right-justified string containing str, with fillChar used to fill the blank space : "bla", 7 -> "    bla". If the string is longer than width, it is truncated.
std::string RightJustify(std::string const& str, unsigned int width, char fillChar = ' ');

/// Creates a progress bar
std::string GenerateProgressBar(unsigned int width, unsigned int current, unsigned int total, bool showPercentage = true, std::string const& fillChar = "\u25A0");

/// Converts a size in bytes to a human readable size (To, Go, Mo, ko, o).
/// If SI_units is false, the size will be displayed using powers of 1024 instead of 1000, producing Mio, kio etc.
/// The suffix can be specified. Default is 'o' for 'octet'. Use 'b' for bits or 'B' for bytes.
std::string Bytes2HumanReadable(uint64_t size, bool SI_units = true, const char* suffix = "o");

#endif

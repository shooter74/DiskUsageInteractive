#include <Display.hpp>

using std::cout;

Display::Display()
{
    winsize size = Display::GetTerminalSize();
    screenCols = size.ws_col;
    screenRows = size.ws_row;
}

unsigned int Display::Cols() const { return screenCols; }
unsigned int Display::Rows() const { return screenRows; }

winsize Display::GetTerminalSize()
{
    winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size;
}

void Display::ClearScreen() { cout << "\033[2J\033[1;1H"; }

std::string GenerateProgressBar(unsigned int width, unsigned int current, unsigned int total, bool showPercentage, std::string const& fillChar)
{
    std::string progressBar;
    unsigned int freeWidth = width-2;
    char buf[8];

    progressBar = '[';

    if(showPercentage)
    {
        double percentCurrent = 100.*((double)current/(double)total);
        freeWidth -= 7;
        sprintf(buf, "%5.1f%% ", percentCurrent);
        progressBar += buf;
    }
    
    unsigned int scaledCurrent = (current*freeWidth)/total;

    for(unsigned int i = 0 ; i < scaledCurrent ; i++)
        progressBar += fillChar;
    for(unsigned int i = scaledCurrent ; i < freeWidth ; i++)
        progressBar += ' ';

    progressBar += ']';

    return progressBar;
}

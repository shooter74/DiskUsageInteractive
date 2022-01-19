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

    if(current > total) // Protect against > 100 %
        current = total;
    
    if(total == 0)      // Protect against divisions by 0
        total = 1;
    
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

std::string Bytes2HumanReadable(uint64_t size, bool SI_units, const char* suffix)
{
    const uint64_t kilo = (SI_units) ? 1000ULL : 1024ULL;
    const uint64_t mega = kilo*kilo;
    const uint64_t giga = mega*kilo;
    const uint64_t tera = giga*kilo;
    const uint64_t peta = tera*kilo;
    const uint64_t exa = peta*kilo;
    //const uint64_t zetta = exa*kilo;
    //const uint64_t yotta = zetta*kilo;

    char buf[16];
    std::string fullSuffix = (SI_units) ? std::string(suffix) : (std::string("i") + suffix);

    //if(size >= yotta)
    //    sprintf(buf, "%5.1f Y%s", (double)size/yotta, fullSuffix.c_str());
    //else if(size >= zetta)
    //    sprintf(buf, "%5.1f Z%s", (double)size/zetta, fullSuffix.c_str());

    if(size >= exa)
        sprintf(buf, "%5.1f E%s", (double)size/exa, fullSuffix.c_str());
    else if(size >= peta)
        sprintf(buf, "%5.1f P%s", (double)size/peta, fullSuffix.c_str());
    else if(size >= tera)
        sprintf(buf, "%5.1f T%s", (double)size/tera, fullSuffix.c_str());
    else if(size >= giga)
        sprintf(buf, "%5.1f G%s", (double)size/giga, fullSuffix.c_str());
    else if(size >= mega)
        sprintf(buf, "%5.1f M%s", (double)size/mega, fullSuffix.c_str());
    else if(size >= kilo)
        sprintf(buf, "%5.1f k%s", (double)size/kilo, fullSuffix.c_str());
    else
        sprintf(buf, "%5.1f %s", (double)size, suffix);

    return std::string(buf);
}


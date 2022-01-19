#include <Display.hpp>

using std::cout;

Display::Display()
{
    UpdateScreenSize();
    ClearScreenLines();
}

unsigned int Display::Cols() const { return screenCols; }
unsigned int Display::Rows() const { return screenRows; }

std::string const& Display::GetLine(unsigned int i) const { return lines[i]; }

void Display::SetLine(unsigned int i, std::string const& str)
{
    if(str.size() <= Cols())
        lines[i] = str + std::string(Cols() - str.size(), ' ');
    else
        lines[i] = str.substr(0, Cols());
}

void Display::SetLineCentered(unsigned int i, std::string const& str, char fillChar) { lines[i] = CenterString(str, Cols(), fillChar); }
void Display::SetLineRjustified(unsigned int i, std::string const& str, char fillChar) { lines[i] = RightJustify(str, Cols(), fillChar); }

char & Display::GetPixel(unsigned int i, unsigned int j) { return lines[i][j]; }

void Display::ClearScreenLines()
{
    lines.clear();
    std::string emptyLine(screenCols, ' ');
    for(unsigned int i = 0 ; i < screenRows ; i++)
        lines.push_back(emptyLine);
}

void Display::DrawScreenLines()
{
    for(unsigned int i = 0 ; i < lines.size() ; i++)
        cout << lines[i] << "\n";
}

void Display::UpdateScreenSize()
{
    winsize size = Display::GetTerminalSize();
    screenCols = size.ws_col;
    screenRows = size.ws_row;
}

void Display::HighlightLine(unsigned int i, bool highlight)
{
    if(highlight)
    {
        if(lines[i].substr(0, 4) != C_INVERT_FG_BG)
            lines[i] = C_INVERT_FG_BG + lines[i] + C_RESET;
    }
    else
    {
        if(lines[i].substr(0, 4) == C_INVERT_FG_BG)
            lines[i] = lines[i].substr(4, lines[i].size()-4-4);
    }
}

winsize Display::GetTerminalSize()
{
    winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size;
}

void Display::ClearScreen() { cout << "\033[2J\033[1;1H"; }

std::string LeftJustify(std::string const& str, unsigned int width, char fillChar)
{
    if(str.size() <= width)
        return str + std::string(width - str.size(), fillChar);
    else
        return str.substr(0, width);
}

std::string CenterString(std::string const& str, unsigned int width, char fillChar)
{
    if(str.size() <= width)
    {
        unsigned int freeSpace = (width - str.size())/2;
        return std::string(freeSpace, fillChar) + str + std::string(freeSpace, fillChar);
    }
    else
        return str.substr(0, width);
}

std::string RightJustify(std::string const& str, unsigned int width, char fillChar)
{
    if(str.size() <= width)
        return std::string(width - str.size(), fillChar) + str;
    else
        return str.substr(0, width);
}

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


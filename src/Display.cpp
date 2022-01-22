#include <Display.hpp>
#include <TreeNodeDiskUsage.hpp>

// DEBUG
#ifndef PRINT_VAR
#define PRINT_VAR(x); std::cout << #x << "\t= " << (x) << "\n"; 
#endif

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

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
    if(str.size() <= screenCols)
        lines[i] = str + std::string(screenCols - str.size(), ' ');
    else
        lines[i] = str.substr(0, screenCols);
}

void Display::SetLineCentered(unsigned int i, std::string const& str, char fillChar) { lines[i] = CenterString(str, screenCols, fillChar); }
void Display::SetLineRjustified(unsigned int i, std::string const& str, char fillChar) { lines[i] = RightJustify(str, screenCols, fillChar); }

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

void Display::DisplayTreeNode(TreeNodeDiskUsage const& treeNode, size_t topLine, bool SI_units)
{
    ClearScreenLines();
    // Write header
    lines[0] = std::string(C_INVERT_FG_BG) + CenterString("DiskUsageInteractive ~ Use the arrow keys to navigate, press ? or 'h' for help", screenCols) + C_RESET;

    // Write currently displayed directory using its absolute path. If the path resolution does not succeed, the relative path is displayed
    char absoluteNodePathCstr[4096+1];
    char *ptr = realpath(treeNode.GetNodePath().c_str(), absoluteNodePathCstr);
    std::string absoluteNodePath = (ptr != NULL) ? absoluteNodePathCstr : treeNode.GetNodePath();
    lines[1] = "--- " + LeftJustify(absoluteNodePath + " ", screenCols-5, '-');

    size_t availableLines = screenRows - 3;// Number of available lines for the listing

    size_t fieldWidth1 = 10;
    size_t fieldWidth2 = screenCols/3;
    size_t fieldWidth3 = screenCols - fieldWidth1 - fieldWidth2;

    if(lines.size() < 3)// Not enough space !
        return;

    for(size_t i = 0 ; i < MIN(availableLines, treeNode.GetChildrenCount()-topLine) ; i++)
    {
        TreeNodeDiskUsage const& node = treeNode.GetChildren()[i+topLine];
        std::stringstream lineStream;
        lineStream << RightJustify(Bytes2HumanReadable(node.GetTotalSize(), SI_units), fieldWidth1);
        lineStream << GenerateProgressBar(fieldWidth2, node.GetTotalSize(), treeNode.GetTotalSize(), true, "#");
        lineStream << " " << LeftJustify(node.GetNodeName() + ((node.IsFolder()) ? "/" : " "), fieldWidth3-1);
        lines[i+2] = LeftJustify(lineStream.str(), screenCols);
    }

    // Write footer
    std::stringstream footerSstream; footerSstream << "Total disk usage:   " << Bytes2HumanReadable(treeNode.GetTotalSizeOnDisk(), SI_units)
                                                   << "   Apparent size:   " << Bytes2HumanReadable(treeNode.GetTotalSize(), SI_units)
                                                   << "   Items:   " << treeNode.GetTotalElements();
    lines[screenRows-1] = std::string(C_INVERT_FG_BG) + LeftJustify(footerSstream.str(), screenCols) + C_RESET;
}

/*
ncdu 1.14.1 ~ Use the arrow keys to navigate, press ? for help                                                    
--- /home/jerome/codeserver/config/workspace/DiskUsageInteractive ------------------------------------------------
  952.0 KiB [##########] /build                                                                                   
  536.0 KiB [#####     ] /.git
   24.0 KiB [          ] /src
   16.0 KiB [          ] /include
e   4.0 KiB [          ] /workspace
    4.0 KiB [          ]  Makefile
    4.0 KiB [          ]  .gitignore
@   0.0   B [          ]  DiskUsageInteractive
                           ┌───ncdu help─────────────────1:Keys───2:Format───3:About──┐
                           │                                                          │
                           │       up, k  Move cursor up                              │
                           │     down, j  Move cursor down                            │
                           │ right/enter  Open selected directory                     │
                           │  left, <, h  Open parent directory                       │
                           │           n  Sort by name (ascending/descending)         │
                           │           s  Sort by size (ascending/descending)         │
                           │           C  Sort by items (ascending/descending)        │
                           │           M  Sort by mtime (-e flag)                     │
                           │           d  Delete selected file or directory           │
                           │           t  Toggle dirs before files when sorting       │
                           │                        -- more --                        │
                           │                                         Press q to close │
                           └──────────────────────────────────────────────────────────┘








 Total disk usage:   1.5 MiB  Apparent size:   1.3 MiB  Items: 131              
*/

winsize Display::GetTerminalSize()
{
    winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size;
}

void Display::ClearScreen() { cout << "\033[2J\033[1;1H"; }

std::string LeftJustify(std::string str, unsigned int width, char fillChar)
{
    str.resize(width, fillChar);
    return str;
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

std::string GenerateProgressBar(size_t width, size_t current, size_t total, bool showPercentage, std::string const& fillChar)
{
    std::string progressBar;
    size_t freeWidth = width-2;
    char buf[8];

    if(total == 0)      // Protect against divisions by 0
        total = 1;

    if(current > total) // Protect against > 100 %
        current = total;
    
    if(showPercentage)
    {
        double percentCurrent = 100.*((double)current/(double)total);
        freeWidth -= 7;
        sprintf(buf, "%5.1f%% ", percentCurrent);
        progressBar += buf;
    }

    progressBar += '[';
    
    size_t scaledCurrent = (current*freeWidth)/total;

    for(size_t i = 0 ; i < scaledCurrent ; i++)
        progressBar += fillChar;
    progressBar += std::string(freeWidth - scaledCurrent, ' ');

    progressBar += ']';

    return progressBar;
}

std::string Bytes2HumanReadable(uint64_t size, bool SI_units, std::string const& suffix)
{
    const uint64_t kilo = (SI_units) ? 1000ULL : 1024ULL;
    const uint64_t mega = kilo*kilo;
    const uint64_t giga = mega*kilo;
    const uint64_t tera = giga*kilo;
    const uint64_t peta = tera*kilo;
    const uint64_t exa = peta*kilo;

    std::string fullSuffix = (SI_units) ? suffix : (std::string("i") + suffix);

    std::stringstream out;
    out.precision(1);
    out << std::fixed << std::setw(5) << std::right;

    if(size >= exa)
        out << (double)size/exa << " E" << fullSuffix;
    else if(size >= peta)
        out << (double)size/peta << " P" << fullSuffix;
    else if(size >= tera)
        out << (double)size/tera << " T" << fullSuffix;
    else if(size >= giga)
        out << (double)size/giga << " G" << fullSuffix;
    else if(size >= mega)
        out << (double)size/mega << " M" << fullSuffix;
    else if(size >= kilo)
        out << (double)size/kilo << " k" << fullSuffix;
    else
        out << (double)size << " " << suffix;
    return out.str();
}


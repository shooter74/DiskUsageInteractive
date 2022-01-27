#include <EventManager.hpp>

#define KEY_ARROW_UP 'A'
#define KEY_ARROW_DOWN 'B'
#define KEY_ARROW_RIGHT 'C'
#define KEY_ARROW_LEFT 'D'
#define KEY_BACKSPACE 127
#define KEY_DEL 126
#define KEY_HOME 'H'
#define KEY_ENTER 10

EventManager::EventManager(Display & display_, TreeNodeDiskUsage & rootNode_)
 : display(display_),
   rootNode(rootNode_),
   topLine(0),
   currentLine(0),
   sortType(0),
   SI_units(true)
{
    displayedNodes.push_back(&rootNode);
    SortNodeElements();
    UpdateScreen();
}

EventManager::~EventManager() {}

void EventManager::MainEventLoop()
{
    struct termios oldSettings, newSettings;

    tcgetattr(fileno(stdin), &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &newSettings);    

    bool readingSymbol = false;

    while(1)
    {
        fd_set set;
        struct timeval tv;

        tv.tv_sec = 10;
        tv.tv_usec = 0;

        FD_ZERO(&set);
        FD_SET(fileno(stdin), &set);

        int res = select(fileno(stdin)+1, &set, NULL, NULL, &tv);// res == 0 -> timeout reached

        if(res > 0)
        {
            char c;
            if(read(fileno(stdin), &c, 1) != 1) // read character from stdin
                break;
            // std::cout << (int)c << " = \"" << c << "\"\n";// DEBUG

            // Parse event
            if(c == 'q')        // Quit program
                break;
            else if(c == 27)    // when an arrow key is pressed, a string of 3 characters is produced : 27, '[', key identifier
            {
                readingSymbol = true;
                continue;
            }

            if(!readingSymbol)  // Regular key presses that fit in a single character
            {
                if(c == KEY_ENTER)
                    CallbackEnter();
                else if(c == KEY_BACKSPACE)
                    CallbackBackspace();
                else if(c == '?' || c == 'h' || c == 'H')
                    CallbackHelp();
                else if(c == 's' || c == 'S')
                    CallbackSort();
                else if(c == 'u' || c == 'U')
                    CallbackUnits();
            }
            else
            {
                if(c == '[')    // second character of the symbol
                    continue;
                
                if(c == KEY_ARROW_UP)
                    CallbackArrowUp();
                else if(c == KEY_ARROW_DOWN)
                    CallbackArrowDown();
                else if(c == KEY_ARROW_LEFT)
                    CallbackArrowLeft();
                else if(c == KEY_ARROW_RIGHT)
                    CallbackArrowRight();
                else if(c == KEY_HOME)
                    CallbackHome();

                //std::cout << "symbol read : " << (int)c << " = \"" << c << "\"\n";// DEBUG
                readingSymbol = false;
            }
        }
        else if(res < 0)
        {
            std::cerr << "Event select error.\n";
            break;
        }
    }

    tcsetattr(fileno(stdin), TCSANOW, &oldSettings);
}

void EventManager::CallbackArrowUp()
{
    if(displayedNodes.back() == NULL) return;
    if(displayedNodes.back()->GetChildrenCount())
        currentLine = ((currentLine - 1) < 0) ? (displayedNodes.back()->GetChildrenCount() - 1) : ((currentLine - 1) % displayedNodes.back()->GetChildrenCount());
    ScrollList();
    UpdateScreen();
}

void EventManager::CallbackArrowDown()
{
    if(displayedNodes.back() == NULL) return;
    if(displayedNodes.back()->GetChildrenCount())
        currentLine = (currentLine + 1) % displayedNodes.back()->GetChildrenCount();
    ScrollList();
    UpdateScreen();
}

void EventManager::CallbackArrowLeft()
{
    if(displayedNodes.back() == NULL) return;
    CallbackBackspace();
}

void EventManager::CallbackArrowRight()
{
    if(displayedNodes.back() == NULL) return;
    CallbackEnter();
}

void EventManager::CallbackEnter()
{
    if(displayedNodes.back() == NULL) return;
    if((size_t)currentLine < displayedNodes.back()->GetChildrenCount())
        if(displayedNodes.back()->GetChild(currentLine).IsFolder())
        {
            displayedNodes.push_back(&displayedNodes.back()->GetChild(currentLine));
            SortNodeElements();
            topLine = 0;
            currentLine = 0;
        }
    UpdateScreen();
}

void EventManager::CallbackBackspace()
{
    if(displayedNodes.back() == NULL) return;
    // Go up one level
    if(displayedNodes.size() > 1)
    {
        displayedNodes.pop_back();
        topLine = 0;
        currentLine = 0;
    }
    UpdateScreen();
}

void EventManager::CallbackHome()
{
    // Set the current node to be the root node (go back "home").
    displayedNodes.clear();
    displayedNodes.push_back(&rootNode);
    topLine = 0;
    currentLine = 0;
    UpdateScreen();
}

void EventManager::CallbackHelp()
{
    if(displayedNodes.back() == NULL) return;
    
    UpdateScreen();
}

void EventManager::CallbackSort()
{
    if(displayedNodes.back() == NULL) return;
    sortType = (sortType + 1) % 2;// Cycle through all the sort types.
    SortNodeElements();
    UpdateScreen();
}

void EventManager::CallbackUnits()
{
    if(displayedNodes.back() == NULL) return;
    SI_units ^= true;// Invert the SI_units flag.
    UpdateScreen();
}

void EventManager::ScrollList()
{
    /*if(displayedNodes.back() == NULL) return;
    if(displayedNodes.back()->GetChildrenCount())
    {
        // compute visibility of selected line and scroll if needed
        PRINT_VAR(2 + currentLine - topLine);
        PRINT_VAR(display.Rows()-1);
        if((2 + currentLine - topLine) >= (display.Rows()-1))
            topLine = currentLine - (display.Rows() - 2);// DOES NOT WORK. WORK IN PROGRESS.
    }//*/
}

void EventManager::SortNodeElements()
{
    if(displayedNodes.back() == NULL) return;
    if(sortType == 0)
        displayedNodes.back()->SortBySizeDesc();
    else if(sortType == 1)
        displayedNodes.back()->SortByNameAsc();
}

void EventManager::UpdateScreen()
{
    //DebugPrintState(); return;// DEBUG
    Display::ClearScreen();
    display.DisplayTreeNode(*displayedNodes.back(), topLine, SI_units);
    display.HighlightLine(2 + topLine + currentLine, true);
    display.DrawScreenLines();
}

void EventManager::DebugPrintState()
{
    PRINT_VAR(&rootNode);
    PRINT_VAR(topLine);
    PRINT_VAR(currentLine);
    PRINT_VAR(displayedNodes.back());
    PRINT_VAR(sortType);

    if((size_t)currentLine < displayedNodes.back()->GetChildrenCount())
    {
        PRINT_VAR(displayedNodes.back()->GetChild(currentLine).IsFolder());
        PRINT_VAR(displayedNodes.back()->GetChild(currentLine).GetNodePath());
    }
    std::cout << "-----------------------------------------------\n";
}

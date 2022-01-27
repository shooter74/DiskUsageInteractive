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
   sortType(0),
   SI_units(true)
{
    displayedNodes.push_back(DisplayedNode(&rootNode, 0, 0));
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
    if(displayedNodes.back().node == NULL) return;
    if(!displayedNodes.back().node->GetChildrenCount()) return;
    displayedNodes.back().currentLine = ((displayedNodes.back().currentLine - 1) < 0) ? (displayedNodes.back().node->GetChildrenCount() - 1) : ((displayedNodes.back().currentLine - 1) % displayedNodes.back().node->GetChildrenCount());
    ScrollList();
    UpdateScreen();
}

void EventManager::CallbackArrowDown()
{
    if(displayedNodes.back().node == NULL) return;
    if(!displayedNodes.back().node->GetChildrenCount()) return;
    displayedNodes.back().currentLine = (displayedNodes.back().currentLine + 1) % displayedNodes.back().node->GetChildrenCount();
    ScrollList();
    UpdateScreen();
}

void EventManager::CallbackArrowLeft()
{
    if(displayedNodes.back().node == NULL) return;
    CallbackBackspace();
}

void EventManager::CallbackArrowRight()
{
    if(displayedNodes.back().node == NULL) return;
    CallbackEnter();
}

void EventManager::CallbackEnter()
{
    if(displayedNodes.back().node == NULL) return;
    if((size_t)displayedNodes.back().currentLine < displayedNodes.back().node->GetChildrenCount())
        if(displayedNodes.back().node->GetChild(displayedNodes.back().currentLine).IsFolder())
        {
            displayedNodes.push_back(DisplayedNode(&displayedNodes.back().node->GetChild(displayedNodes.back().currentLine), 0, 0));
            SortNodeElements();
        }
    UpdateScreen();
}

void EventManager::CallbackBackspace()
{
    if(displayedNodes.back().node == NULL) return;
    // Go up one level
    if(displayedNodes.size() > 1)
        displayedNodes.pop_back();
    UpdateScreen();
}

void EventManager::CallbackHome()
{
    // Set the current node to be the root node (go back "home").
    displayedNodes.clear();
    displayedNodes.push_back(DisplayedNode(&rootNode, 0, 0));
    UpdateScreen();
}

void EventManager::CallbackHelp()
{
    if(displayedNodes.back().node == NULL) return;
    // TODO
    UpdateScreen();
}

void EventManager::CallbackSort()
{
    if(displayedNodes.back().node == NULL) return;
    sortType = (sortType + 1) % 2;// Cycle through all the sort types.
    SortNodeElements();
    UpdateScreen();
}

void EventManager::CallbackUnits()
{
    if(displayedNodes.back().node == NULL) return;
    SI_units ^= true;// Invert the SI_units flag.
    UpdateScreen();
}

void EventManager::ScrollList()
{
    /*if(displayedNodes.back().node == NULL) return;
    if(displayedNodes.back().node->GetChildrenCount())
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
    if(displayedNodes.back().node == NULL) return;
    if(sortType == 0)
        displayedNodes.back().node->SortBySizeDesc();
    else if(sortType == 1)
        displayedNodes.back().node->SortByNameAsc();
}

void EventManager::UpdateScreen()
{
    //DebugPrintState(); return;// DEBUG
    Display::ClearScreen();
    display.DisplayTreeNode(*displayedNodes.back().node, displayedNodes.back().topLine, SI_units);
    display.HighlightLine(2 + displayedNodes.back().topLine + displayedNodes.back().currentLine, true);
    display.DrawScreenLines();
}

void EventManager::DebugPrintState()
{
    PRINT_VAR(&rootNode);
    PRINT_VAR(displayedNodes.back().topLine);
    PRINT_VAR(displayedNodes.back().currentLine);
    PRINT_VAR(displayedNodes.back().node);
    PRINT_VAR(sortType);

    if((size_t)displayedNodes.back().currentLine < displayedNodes.back().node->GetChildrenCount())
    {
        PRINT_VAR(displayedNodes.back().node->GetChild(displayedNodes.back().currentLine).IsFolder());
        PRINT_VAR(displayedNodes.back().node->GetChild(displayedNodes.back().currentLine).GetNodePath());
    }
    std::cout << "-----------------------------------------------\n";
}

DisplayedNode::DisplayedNode(TreeNodeDiskUsage *node_, int topLine_, int currentLine_) : node(node_), topLine(topLine_), currentLine(currentLine_) {}

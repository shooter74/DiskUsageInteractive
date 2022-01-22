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
   currentNode(&rootNode),
   sortType(0)
{}

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
    std::cout << "CallbackArrowUp\n";// DEBUG
    if(currentNode == NULL) return;
    if(currentNode->GetChildrenCount())
        currentLine = (currentLine - 1) % currentNode->GetChildrenCount();
    DebugPrintState();
}

void EventManager::CallbackArrowDown()
{
    std::cout << "CallbackArrowDown\n";// DEBUG
    if(currentNode == NULL) return;
    if(currentNode->GetChildrenCount())
        currentLine = (currentLine + 1) % currentNode->GetChildrenCount();
    DebugPrintState();
}

void EventManager::CallbackArrowLeft()
{
    std::cout << "CallbackArrowLeft\n";// DEBUG
    if(currentNode == NULL) return;
    CallbackBackspace();
}

void EventManager::CallbackArrowRight()
{
    std::cout << "CallbackArrowRight\n";// DEBUG
    if(currentNode == NULL) return;
    CallbackEnter();
}

void EventManager::CallbackEnter()
{
    std::cout << "CallbackEnter\n";// DEBUG
    if(currentNode == NULL) return;
    if((size_t)currentLine < currentNode->GetChildrenCount())
        if(currentNode->GetChild(currentLine).IsFolder())
        {
            currentNode = &currentNode->GetChild(currentLine);
            topLine = 0;
            currentLine = 0;
        }
    DebugPrintState();
}

void EventManager::CallbackBackspace()
{
    std::cout << "CallbackBackspace\n";// DEBUG
    if(currentNode == NULL) return;
    // Go up one level
    if(currentNode->GetParent() != NULL)
    {
        currentNode = currentNode->GetParent();
        topLine = 0;
        currentLine = 0;
    }
    DebugPrintState();
}

void EventManager::CallbackHome()
{
    std::cout << "CallbackHome\n";// DEBUG
    if(currentNode == NULL) return;
    // Set the current node to be the root node (go back "home").
    currentNode = &rootNode;
    topLine = 0;
    currentLine = 0;
    DebugPrintState();
}

void EventManager::CallbackHelp()
{
    std::cout << "CallbackHelp\n";// DEBUG
    if(currentNode == NULL) return;

    DebugPrintState();
}

void EventManager::CallbackSort()
{
    std::cout << "CallbackSort\n";// DEBUG
    if(currentNode == NULL) return;
    sortType = (sortType + 1) % 2;// Cycle through all the sort types.
    if(sortType == 0)
        currentNode->SortBySizeDesc();
    else if(sortType == 1)
        currentNode->SortByNameAsc();
    DebugPrintState();
}

void EventManager::DebugPrintState()
{
    PRINT_VAR(&rootNode);
    PRINT_VAR(topLine);
    PRINT_VAR(currentLine);
    PRINT_VAR(currentNode);
    PRINT_VAR(currentNode->GetParent());
    PRINT_VAR(sortType);

    if((size_t)currentLine < currentNode->GetChildrenCount())
    {
        PRINT_VAR(currentNode->GetChild(currentLine).IsFolder());
        PRINT_VAR(currentNode->GetChild(currentLine).GetNodePath());
    }
    std::cout << "-----------------------------------------------\n";
}

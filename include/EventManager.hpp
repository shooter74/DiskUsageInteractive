#ifndef H_EventManager
#define H_EventManager

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <Display.hpp>
#include <TreeNodeDiskUsage.hpp>

/// This class manages the keyboard events and triggers actions.
class EventManager
{
    public:
        EventManager(Display & display_, TreeNodeDiskUsage & rootNode_);
        ~EventManager();

        /// Starts the main event loop.
        void MainEventLoop();

        // Callback functions that are triggered when the corresponding key is pressed.
        void CallbackArrowUp();
        void CallbackArrowDown();
        void CallbackArrowLeft();
        void CallbackArrowRight();
        void CallbackEnter();
        void CallbackBackspace();
        void CallbackHome();
        void CallbackHelp();
        void CallbackSort();

        void DebugPrintState();
    
    protected:
        Display & display;
        TreeNodeDiskUsage & rootNode;

        int topLine;                    //<! Index of top displayed line (for scrolling)
        int currentLine;                //<! Currently selected line
        TreeNodeDiskUsage *currentNode; //<! Pointer to the currently displayed node.
        int sortType;                   //<! Entry sort type : 0 -> size descending, 1 -> name ascending
};

#endif

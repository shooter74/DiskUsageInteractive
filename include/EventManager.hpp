#ifndef H_EventManager
#define H_EventManager

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <list>

#include <Display.hpp>
#include <TreeNodeDiskUsage.hpp>

/// This structure represents a displayed node, pointing to a tree node
struct DisplayedNode
{
    TreeNodeDiskUsage *node;    //<! Pointer to the tree node.
    int topLine;                //<! Index of top displayed line (for scrolling).
    int currentLine;            //<! Currently selected line.

    DisplayedNode(TreeNodeDiskUsage *node_ = NULL, int topLine_ = 0, int currentLine_ = 0);
};

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
        void CallbackUnits();

        /// Computes the visibility of the selected line and scrolls the element list if needed.
        void ScrollList();

        /// Sorts the elements of the current node according to the sorting method currently selected.
        void SortNodeElements();

        /// Updates the screen based on the current state.
        void UpdateScreen();

        void DebugPrintState();
    
    protected:
        Display & display;
        TreeNodeDiskUsage & rootNode;

        
        std::list<DisplayedNode> displayedNodes;   //<! List of pointers to the successively displayed nodes.
        int sortType;                              //<! Entry sort type : 0 -> size descending, 1 -> name ascending.
        bool SI_units;                             //<! Indicates whether SI units should be used.
};

#endif

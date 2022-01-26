# DiskUsageInteractive
Interactive disk usage utility for linux terminals.

This small and *simple* tool is meant to have as few dependencies as possible. It is freely inspired from ncdu.
It can be compiled using old versions of GCC (such as GCC 4), as it does not use any C++11 features. This is by design.
It is meant to be used on console-only environments, such as head-less servers, or very old computers with few graphical abilities.

## Basic use
By just running the program with no arguments, a list of all the contents of the current working directory will be established, and the total size of the folder both apparent and on disk will be displayed.
The program is by default in interactive mode : that means that the arrow keys can be used to navigate the tree structure. The relative size of every element in a folder is displayed as a percentage and as a visual line.
It is possible to sort the contents of the folders by name, in ascending order, or by size, in descending order.

The following keys can be used to trigger various actions :
- "s" :           swap the sort mode between name ascending and size descending
- "q" :           leave the program
- "u" :           swap between SI units (o, ko, Mo, Go, To, etc) and units based on powers of 1024 (o, kio, Mio, Gio, Tio, etc)
- "h" :           displays a small help message reminding the user what keys have what effect
- "enter" :       if the currently highlighed line corresponds to a folder, the program "descends" into it and displays its contents
- "right arrow" : same as "enter"
- "backspace" :   goes up one level (to the parent folder)
- "left arrow" :  same as "backspace"
- "home" :        goes back to the root folder
- "up arrow" :    highlights the line *above* the currently highlighed line
- "down arrow" :  highlights the line *below* the currently highlighed line

## Static use
By running the program with the option -s, a "static" list is built and printed to the standard output. The option -r, or --recursion-limit, allows to specify up to how many levels the tree should be displayed.
Note that there is no recursion limit when building the graph, only when it's displayed.

## Disk cleanup philosophy
When cleaning up a disk to recover some free space, it is best to first target the largest files and folders, before deleting the usually more numerous, but smaller files. It happens fairly often to spend hours trying to decide what can be deleted or archived on another platform, all to realise after the fact that a mere 2 % of the space has actually been freed.
By analysing the actual size of various folders and files on the drive, it is possible to free 10 %, 20 %, or even more of the drive by deleting a few folders only.

A simple "rule of thumb" that applies to many things in life is the "80-20" rule. It states that in a lot of environments, we find that a large quantity of something, such as money earned by a company, is usually brought in by relatively small proportion of the clients.
Take for example Airbus, with Qatar Airways and Emirates, *only* two airlines, purchasing more than 70 % of the A380 aircrafts.

This "rule" tends to apply to disk space as well :
- a few large files (videos, game files, simulation results, etc), and
- a multitude of smaller files (images, text documents, etc)

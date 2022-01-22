#include <iostream>
#include <TreeNodeDiskUsage.hpp>
#include <Display.hpp>
#include <AnsiTerminal.hpp>
#include <EventManager.hpp>

using std::cout;
using std::cerr;
using std::endl;

#define D_UNIT_TESTS

#include <test.hpp>

int main(int argc, char *argv[])
{
	#ifdef D_UNIT_TESTS
		runTests(argc, argv);
		return 0;
	#endif

	// Main program variables and parameters
	std::string rootpath = ".";
	bool staticMode = false;
	unsigned int maxDepth = 0xFFFFFFFF;// used for static mode

	// Parse arguments
	if(argc > 1)
	{
		bool pathFound = false;
		for(int i = 1 ; i < argc ; i++)
		{
			std::string arg = argv[i];
			if(arg == "-r" || arg == "--recursion-limit")
			{
				if(i+1 < argc)
				{
					maxDepth = atoi(argv[++i]);
					continue;
				}
				else
				{
					cerr << "Error : option \"-r\" must be followed by the recursion limit !\n";
					exit(1);
				}
			}
			else if(arg == "-s" || arg == "--static")
				staticMode = true;
			else if(arg == "-h" || arg == "--help")
			{
				cout << "usage : DiskUsageInteractive [-s] [-r recursion_limit] path\n"
				        "    -s                 --static                             Prints the file tree to the standard output.\n"
				        "    -r recursion_limit --recursion-limit recursion_limit    Sets the recursion limit for the printing. Level 0 is the root node.\n"
						"    -h                 --help                               Prints this help message.\n";
				exit(0);
			}
			else if(!pathFound)	// lonely parameter, it must be the root path, right ?
			{
				rootpath = arg;
				pathFound = true;
			}
		}
	}
	
	Display display;

	// Scan root path
	TreeNodeDiskUsage tree(rootpath);
	if(staticMode)
		tree.BuildTree(false, display.Cols());
	else
	{
		cout << "Scanning folder \"" << rootpath << "\" ...\n";
		tree.BuildTree(true, display.Cols());
	}
	cout << endl;

	tree.SortBySizeDesc(true);

	// Static mode : the tree is displayed in the standard output.
	if(staticMode)
		tree.PrintTree(maxDepth, true);
	
	// Interactive mode : the user can navigate through the tree structure.
	// else

	return 0;
}

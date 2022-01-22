#include <test.hpp>

void runTests(int argc, char *argv[])
{
	std::string rootpath = (argc > 1) ? argv[1] : ".";
	
	if(0)
	{// stat test
		cout << "Reading '" << rootpath << "'\n";
		struct stat s;
		PRINT_VAR(stat(rootpath.c_str(), &s));
		PRINT_VAR(s.st_mode);
		PRINT_VAR(s.st_mode & S_IFMT);
		PRINT_VAR((s.st_mode & S_IFMT) == S_IFLNK);// symbolic link
		PRINT_VAR((s.st_mode & S_IFMT) == S_IFREG);// regular file
		PRINT_VAR((s.st_mode & S_IFMT) == S_IFDIR);// folder
		PRINT_VAR(s.st_size);
		PRINT_VAR(s.st_blksize);
		PRINT_VAR(s.st_blocks);
		PRINT_VAR(s.st_blocks*512);// Size on disk, in bytes
	}
	if(0)
	{// test GetNodeName()
		PRINT_VAR(TreeNodeDiskUsage("/some/path/to/victory.tar.gz").GetNodeName());
		PRINT_VAR(TreeNodeDiskUsage("/some/path/to/a/folder").GetNodeName());
		PRINT_VAR(TreeNodeDiskUsage("/some/path/to/a/folder/").GetNodeName());
		PRINT_VAR(TreeNodeDiskUsage("/").GetNodeName());
		PRINT_VAR(TreeNodeDiskUsage(".").GetNodeName());
		PRINT_VAR(TreeNodeDiskUsage("..").GetNodeName());
		PRINT_VAR(TreeNodeDiskUsage("/a").GetNodeName());
		PRINT_VAR(TreeNodeDiskUsage("/a/b/c//////////").GetNodeName());
		PRINT_VAR(TreeNodeDiskUsage("/a///b//c/").GetNodeName());
	}
	if(0)
	{// test GetNodePath()
		PRINT_VAR(TreeNodeDiskUsage("/a/b/c//////////").GetNodePath());
		PRINT_VAR(TreeNodeDiskUsage("/a///b//c/").GetNodePath());
		PRINT_VAR(TreeNodeDiskUsage("////a///b//c/").GetNodePath());
		PRINT_VAR(TreeNodeDiskUsage("   /a/b/c      ").GetNodePath());
		PRINT_VAR(TreeNodeDiskUsage("   /a/b/c///      ").GetNodePath());
		PRINT_VAR(TreeNodeDiskUsage("   \\this\\\\is/sparta///      ").GetNodePath());
		PRINT_VAR(TreeNodeDiskUsage("   \\this\\\\is/sparta/with spaces//      ").GetNodePath());
	}
	if(0)
	{// basic tree building and printing test
		Display display;
		TreeNodeDiskUsage tree(rootpath);
		cout << "Scanning folder \"" << rootpath << "\" ...\n";
		tree.BuildTree(true, display.Cols());
		cout << endl;

		tree.PrintTree();
		cout << "\n\n";
		PRINT_VAR(tree.GetTotalSize());
		PRINT_VAR(tree.GetTotalSizeOnDisk());
		cout << "\n\n";
		tree.PrintTree(0);
		tree.PrintTree(1);
		tree.PrintTree(2);

		cout << "Effects of sorting --------------------\n";
		tree.SortBySizeDesc(); tree.PrintTree(1);
		tree.SortByNameAsc(); tree.PrintTree(1);
		tree.PrintTree(1, false);

		cout << "Recursive sorting --------------------\n";
		tree.SortBySizeDesc();
		tree.PrintTree(2);
		tree.SortBySizeDesc(true);
		tree.PrintTree(2);
	}
	if(0)
	{// terminal display functions test
		Display d;
		PRINT_VAR(d.Rows());
		PRINT_VAR(d.Cols());

		d.ClearScreen();
		cout << "coucou !\n";
	}
	if(0)
	{// progress bar test
		// unsigned int width, unsigned int current, unsigned int total, char fillChar = 219, bool showPercentage = true
		PRINT_VAR(GenerateProgressBar(20, 0, 100, false));
		PRINT_VAR(GenerateProgressBar(20, 25, 100,false));
		PRINT_VAR(GenerateProgressBar(20, 50, 100, false));
		PRINT_VAR(GenerateProgressBar(20, 75, 100, false));
		PRINT_VAR(GenerateProgressBar(20, 100, 100, false));
		PRINT_VAR(GenerateProgressBar(50, 666, 1000, false));

		PRINT_VAR(GenerateProgressBar(20, 0, 100, true));
		PRINT_VAR(GenerateProgressBar(20, 25, 100, true));
		PRINT_VAR(GenerateProgressBar(20, 50, 100, true));
		PRINT_VAR(GenerateProgressBar(20, 75, 100, true));
		PRINT_VAR(GenerateProgressBar(20, 100, 100, true));
		PRINT_VAR(GenerateProgressBar(50, 666, 1000, true));
		PRINT_VAR(GenerateProgressBar(50, 1666, 1000, true));
		PRINT_VAR(GenerateProgressBar(50, 42, 0, true));
		PRINT_VAR(GenerateProgressBar(50, 0, 0, true));

		PRINT_VAR(GenerateProgressBar(50, 333, 1000, true, "#"));
	}
	if(0)
	{// test output
		PRINT_VAR(Bytes2HumanReadable(123ULL, true));
		PRINT_VAR(Bytes2HumanReadable(1234ULL, true));
		PRINT_VAR(Bytes2HumanReadable(1234567ULL, true));
		PRINT_VAR(Bytes2HumanReadable(1234567890ULL, true));
		PRINT_VAR(Bytes2HumanReadable(1234567890123ULL, true));
		PRINT_VAR(Bytes2HumanReadable(1234567890123456ULL, true));
		PRINT_VAR(Bytes2HumanReadable(1234567890123456789ULL, true));
		cout << "\n";
		PRINT_VAR(Bytes2HumanReadable(123ULL, false));
		PRINT_VAR(Bytes2HumanReadable(1234ULL, false));
		PRINT_VAR(Bytes2HumanReadable(1234567ULL, false));
		PRINT_VAR(Bytes2HumanReadable(1234567890ULL, false));
		PRINT_VAR(Bytes2HumanReadable(1234567890123ULL, false));
		PRINT_VAR(Bytes2HumanReadable(1234567890123456ULL, false));
		PRINT_VAR(Bytes2HumanReadable(1234567890123456789ULL, false));
		cout << "\n";
		PRINT_VAR(Bytes2HumanReadable(1073741824ULL, true));
		PRINT_VAR(Bytes2HumanReadable(1073741824ULL, false));
		PRINT_VAR(Bytes2HumanReadable(1000000000ULL, true));
		PRINT_VAR(Bytes2HumanReadable(1000000000ULL, false));
		
	}
	if(0)
	{// test screen functions
		Display d;
		PRINT_VAR(d.Rows());
		PRINT_VAR(d.Cols());
		PRINT_VAR(d.GetLine(0));
		PRINT_VAR(d.GetLine(0).size());
		
		d.GetPixel(5,5) = '#';
		d.GetPixel(6,6) = '#';
		d.GetPixel(7,7) = '#';
		d.GetPixel(20,50) = '#';

		d.SetLine(4, "Coucou !");
		d.SetLine(10, "------------------------- Coucou ! ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
		d.SetLine(11, "------------------------- Inverted -------------------------");
		d.HighlightLine(11, true);
		d.SetLine(12, "        That's normal, bro.                               Really !");
		d.SetLineCentered(13, "Bonjour tout le monde.");
		d.SetLineCentered(14, "Tralala c'est moi la pute.", '-');
		d.SetLineRjustified(15, "Tralala c'est moi la pute, mais a droite de l'ecran.", '-');
		
		Display::ClearScreen();
		d.DrawScreenLines();

		PRINT_VAR(d.GetLine(12).size());
		d.HighlightLine(12, true);
		PRINT_VAR(d.GetLine(12).size());
		d.HighlightLine(12, true);
		PRINT_VAR(d.GetLine(12).size());
		d.HighlightLine(12, false);
		PRINT_VAR(d.GetLine(12).size());
		PRINT_VAR(d.GetLine(12));
		PRINT_VAR(std::string(C_INVERT_FG_BG).size());
	}
	if(0)
	{// test terminal colors
		cout << C_BLUE_F << "blue" << "\n";
		cout << C_CYAN_F << "cyan" << "\n";
		cout << C_YELLOW_F << "yellow" << "\n";
		cout << C_WHITE_F << "white" << "\n";
		cout << C_RED_F << "red" << "\n";
		cout << C_GREEN_F << "green" << "\n";
		cout << C_BLACK_F << "black" << "\n";
		cout << C_RESET << "Normal" << "\n";
		cout << C_BOLD << "Bold text." << C_RESET << " Normal text." << "\n";

		cout << C_BLUE_B << C_YELLOW_F << "Hard on the eyes..." << C_RESET << "\n";
		cout << C_UNDERLINE << "underlined" << C_RESET << "\n";
		cout << C_INVERT_FG_BG << "inverted" << C_RESET << "\n";

		cout << "\n\n";

		{// Nice color table
			int i, j, n;
			for (i = 0; i < 11; i++) {
				for (j = 0; j < 10; j++) {
					n = 10 * i + j;
					if (n > 107) break;
					printf("\033[%dm %3d\033[m", n, n);
				}
				printf("\n");
			}
		}
	}
	//if(0)
	{// test EventManager
		Display display;
		TreeNodeDiskUsage tree(rootpath);
		tree.BuildTree(false);
		EventManager eventManager(display, tree);
		eventManager.MainEventLoop();
	}
}

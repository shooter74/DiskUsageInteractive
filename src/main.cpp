#include <iostream>
#include <TreeNodeDiskUsage.hpp>
#include <Display.hpp>

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		if(0)
		{// stat test
			cout << "Reading '" << argv[1] << "'\n";
			struct stat s;
			PRINT_VAR(stat(argv[1], &s));
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
			TreeNodeDiskUsage tree(argv[1]);
			tree.BuildTree();
			tree.PrintTree();
			cout << "\n\n";
			PRINT_VAR(tree.GetTotalSize());
			PRINT_VAR(tree.GetTotalSizeOnDisk());
			cout << "\n\n";
			tree.PrintTree(0);
			tree.PrintTree(1);
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
		//if(0)
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
		//if(0)
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
	}
	else
		cerr << "Error : No path given !\n";
	
	return 0;
}

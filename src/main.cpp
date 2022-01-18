#include <iostream>
#include <TreeNodeDiskUsage.hpp>

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
		// if(0)
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
		// if(0)
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
		{
			TreeNodeDiskUsage tree(argv[1]);
			tree.BuildTree();
			tree.PrintTree();
			PRINT_VAR(tree.GetTotalSize());
			PRINT_VAR(tree.GetTotalSizeOnDisk());
			tree.PrintTree(0);
			tree.PrintTree(1);
			tree.PrintTree(2);
		}
	}
	else
		cerr << "Error : No path given !\n";
	
	return 0;
}

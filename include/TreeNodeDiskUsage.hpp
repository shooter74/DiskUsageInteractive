#ifndef H_TreeNodeDiskUsage
#define H_TreeNodeDiskUsage

#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>

#define PRINT_VAR(x); std::cout << #x << "\t= " << (x) << "\n"; 

/// \struct PathFilters Stores the filters on paths.
struct PathFilters
{
	std::vector<std::string> const& pathFiltersInclude;
	std::vector<std::string> const& pathFiltersExclude;
};

/// \class TreeNodeDiskUsage Implements a node of a tree of the files and folders on a system.
class TreeNodeDiskUsage
{
	public:
		TreeNodeDiskUsage(std::string const& path_, PathFilters const* pathFilters_ = NULL);
		~TreeNodeDiskUsage();
		
		/// Returns true if the path of the node matches the filters given to it. If no filters have been defined, all paths are valid.
		bool MatchesFilter() const;
		
		/// Builds the whole tree
		void BuildTree();
		
		/// Prints the tree to the standard output only including the names of the nodes
		void PrintTree(unsigned int maxDepth = 0xFFFFFFFF, unsigned int depth = 0) const;
		
		/// Returns the number of children of the node.
		size_t GetChildrenCount() const;
		
		/// Returns the total size of the node, including all its children
		size_t GetTotalSize() const;
		
		/// Returns the total size of the node on disk, including all its children
		size_t GetTotalSizeOnDisk() const;
		
		/// Returns the name of the node : /path/to/node.bla -> node.bla
		std::string GetNodeName() const;

		/// Returns the path of the node : /path/to/node.bla
		std::string GetNodePath() const;
	
	protected:
		/// Cleans the path to remove double slashes, trailing slashes and leading and trailing whitespaces.
		void SanitizePath();
		
		/// Explores the whole tree to compute every node's size, including all their children.
		void ComputeTotalSize();

		/// Removes the leading character 'c'.
		static std::string RemoveLeadingCharacter(std::string const& str, char c);

		/// Removes the trailing character 'c'.
		static std::string RemoveTrailingCharacter(std::string const& str, char c);

		/// Removes contiguous occurrences of the character 'c'.
		static std::string RemoveDoublesCharacter(std::string const& str, char c);

		/// Replaces all occurrences of c1 by c2 in str.
		static std::string ReplaceCharacterInStr(std::string str, char c1, char c2);
	
	protected:
		PathFilters const* pathFilters;					//<! Pointer to filters applied to the paths.
		
		std::string path;								//<! Path of the node
		std::vector<TreeNodeDiskUsage> children;		//<! Stores all the children contained within the folder.
		bool isFolder;									//<! Indicates whether the node is a folder or a file.
		size_t totalSize;								//<! Total size of the node, taking all children into account.
		size_t totalSizeOnDisk;							//<! Total size of the node on the disk, taking all children into account.
};

#endif

#ifndef H_TreeNodeDiskUsage
#define H_TreeNodeDiskUsage

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>

#include <Display.hpp>

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
		void BuildTree(bool verbose = false, unsigned int screenWidth = 80);
		
		/// Prints the tree to the standard output only including the names of the nodes
		void PrintTree(unsigned int maxDepth = 0xFFFFFFFF, bool humanReadableSizes = true, size_t parentSize = 0, unsigned int depth = 0) const;
		
		/// Returns the number of children of the node.
		size_t GetChildrenCount() const;
		
		/// Returns the total size of the node, including all its children
		size_t GetTotalSize() const;
		
		/// Returns the total size of the node on disk, including all its children
		size_t GetTotalSizeOnDisk() const;

		/// Returns the total number of elements contained within the node, including all its children
		size_t GetTotalElements() const;
		
		/// Returns the name of the node : /path/to/node.bla -> node.bla
		std::string GetNodeName() const;

		/// Returns the path of the node : /path/to/node.bla
		std::string GetNodePath() const;

		/// Returns a pointer to the node's parent. Warning : this pointer can be NULL !
		TreeNodeDiskUsage * GetParent() const;

		/// Returns true if the node corresponds to a folder.
		bool IsFolder() const;

		/// Returns a constant reference to the list of children of the node.
		std::vector<TreeNodeDiskUsage> const& GetChildren() const;

		/// Returns a constant reference to the the child i.
		TreeNodeDiskUsage const& GetChild(unsigned int i) const;

		/// Returns a reference to the the child i.
		TreeNodeDiskUsage & GetChild(unsigned int i);

		/// Builds the parent links. Must be called after the tree has been built.
		void BuildParentLinks();

		/// Sorts the children of the node by size in descending order.
		void SortBySizeDesc(bool recursive = false);

		/// Sorts the children of the node by name in ascending order.
		void SortByNameAsc(bool recursive = false);

		/// Operator used to sort the nodes by size in descending order. Returns true when a.totalSize > b.totalSize.
		static bool SortOperatorSizeDesc(TreeNodeDiskUsage const& a, TreeNodeDiskUsage const& b);
		/// Operator used to sort the nodes by size in descending order. Returns true when a.totalSize > b.totalSize.
		static bool SortOperatorNameAsc(TreeNodeDiskUsage const& a, TreeNodeDiskUsage const& b);
	
	protected:
		/// Cleans the path to remove double slashes, trailing slashes and leading and trailing whitespaces.
		void SanitizePath();

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
		TreeNodeDiskUsage * parentNode;					//<! Pointer to the parent of the node.
		bool isFolder;									//<! Indicates whether the node is a folder or a file.
		size_t totalSize;								//<! Total size of the node, taking all children into account.
		size_t totalSizeOnDisk;							//<! Total size of the node on the disk, taking all children into account.
		size_t totalElements;							//<! Total number of elements within the node, taking all children into account.
};

#endif

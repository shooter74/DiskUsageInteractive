#include <TreeNodeDiskUsage.hpp>

TreeNodeDiskUsage::TreeNodeDiskUsage(std::string const& path_, PathFilters const* pathFilters_)
: pathFilters(pathFilters_),
  path(path_),
  isFolder(false),
  totalSize(0),
  totalSizeOnDisk(0)
{
	SanitizePath();
}

TreeNodeDiskUsage::~TreeNodeDiskUsage()
{
	
}

bool TreeNodeDiskUsage::MatchesFilter() const
{
	if(pathFilters == NULL)
		return true;
	
	if(!pathFilters->pathFiltersInclude.size() && !pathFilters->pathFiltersExclude.size())
		return true;
	/*
	bool included = false;
	for(unsigned int i = 0 ; i < pathFilters.pathFiltersInclude.size() ; i++)
		//*/
	return true;// Temporary
}

void TreeNodeDiskUsage::BuildTree()
{
	// Build the entire tree starting from the current node
	
	// Check that the node is included in the current search
	if(!MatchesFilter())
		return;
	
	// Acquire stats on the current path
	struct stat s;
	
	// Check that path exists
	if(stat(path.c_str(), &s) != 0)
		return;
	
	std::cout << "BuildTree:path = " << path << "\n";// DEBUG
	
	isFolder = (s.st_mode & S_IFMT) == S_IFDIR;// True if the path corresponds to a folder
	//PRINT_VAR((s.st_mode & S_IFMT) == S_IFLNK);// symbolic link
	//PRINT_VAR((s.st_mode & S_IFMT) == S_IFREG);// regular file
	
	if(!isFolder)
	{
		totalSize = s.st_size;
		totalSizeOnDisk = s.st_blocks*512;// st_blocks is given in number of blocks of 512 bytes
	}
	else
	{
		totalSize = 0;
		totalSizeOnDisk = 0;
		
		// List the contents of the folder
		DIR *d;
		dirent *dir;
		d = opendir(path.c_str());
		if(d)
		{
			while ((dir = readdir(d)) != NULL)
			{
				std::string elementname = std::string(dir->d_name);
				if(elementname != "." && elementname != "..")
				{
					// Recursively build the sub-tree
					TreeNodeDiskUsage node(path + "/" + elementname, pathFilters);
					node.BuildTree();
					totalSize += node.totalSize;
					totalSizeOnDisk += node.totalSizeOnDisk;
					children.push_back(node);
				}
				//printf("%s\n", dir->d_name);
			}
			closedir(d);
		}
	}
}

void TreeNodeDiskUsage::PrintTree(unsigned int maxDepth, unsigned int depth) const
{
	if(depth > maxDepth)
		return;
	
	for(unsigned int i = 0 ; i < depth ; i++)
		std::cout << "    ";
	//std::cout << path << "\n";
	std::cout << path << "\t" << totalSize << "\t" << totalSizeOnDisk << "\n";
	if(isFolder)
		for(unsigned int i = 0 ; i < children.size() ; i++)
			children[i].PrintTree(maxDepth, depth+1);
}

size_t TreeNodeDiskUsage::GetChildrenCount() const { return children.size(); }
size_t TreeNodeDiskUsage::GetTotalSize() const { return totalSize; }
size_t TreeNodeDiskUsage::GetTotalSizeOnDisk() const { return totalSizeOnDisk; }
std::string TreeNodeDiskUsage::GetNodePath() const{ return path; }

std::string TreeNodeDiskUsage::GetNodeName() const
{
	if(path.size() <= 1)    // covers "." and "/"
		return path;
	
	if(path == "..")
		return path;
	
	// Find last "/" in path
	unsigned int lastSepPos = 0;
	for(int i = (int)path.size() ; i >= 0 ; i--)
		if(path[i] == '/')
		{
			lastSepPos = i;
			break;
		}
	
	return path.substr(lastSepPos+1, path.size()-lastSepPos-1);
}

void TreeNodeDiskUsage::SanitizePath()
{
	// Special cases handling
	if(path == "/" || path == "\\" || path == "." || path == "..")
		return;

	path = TreeNodeDiskUsage::ReplaceCharacterInStr(path, '\\', '/');
	path = TreeNodeDiskUsage::ReplaceCharacterInStr(path, '\t', ' ');

	path = TreeNodeDiskUsage::RemoveTrailingCharacter(path, ' ');	// Remove trailing white spaces
	path = TreeNodeDiskUsage::RemoveTrailingCharacter(path, '/');	// Remove trailing slashes

	path = TreeNodeDiskUsage::RemoveLeadingCharacter(path, ' ');	// Remove leading white spaces
	
	// Remove multiple occurrences of slashes
	path = TreeNodeDiskUsage::RemoveDoublesCharacter(path, '/');
}

std::string TreeNodeDiskUsage::RemoveLeadingCharacter(std::string const& str, char c)
{
	unsigned int slashCounter = 0;
	for(unsigned int i = 0 ; i < str.size() ; i++)
	{
		if(str[i] == c)
			slashCounter++;
		else
			break;
	}
	if(slashCounter)
		return str.substr(slashCounter, str.size());
	return str;
}

std::string TreeNodeDiskUsage::RemoveTrailingCharacter(std::string const& str, char c)
{
	unsigned int slashCounter = 0;
	for(int i = str.size()-1 ; i >= 0 ; i--)
	{
		if(str[i] == c)
			slashCounter++;
		else
			break;
	}
	if(slashCounter)
		return str.substr(0, str.size()-slashCounter);
	return str;
}

std::string TreeNodeDiskUsage::RemoveDoublesCharacter(std::string const& str, char c)
{
	std::string newStr;
	unsigned int slashCounter = 0;
	for(unsigned int i = 0 ; i < str.size() ; i++)
	{
		if(str[i] == c)
			slashCounter++;
		else
			slashCounter = 0;
		
		if(slashCounter < 2)
			newStr += str[i];
	}
	return newStr;
}

std::string TreeNodeDiskUsage::ReplaceCharacterInStr(std::string str, char c1, char c2)
{
	for(unsigned int i = 0 ; i < str.size() ; i++)
		if(str[i] == c1)
			str[i] = c2;
	return str;
}

void TreeNodeDiskUsage::ComputeTotalSize()
{
	// Recursively add all sizes to the current node's size
	//if()
}

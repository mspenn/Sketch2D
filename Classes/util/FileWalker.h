#ifndef __FILE_WALKER_H__
#define __FILE_WALKER_H__

#include <vector>
#include <string>

// Sub directories
#define MSK_SUB_DIR 0x0001
// All directories, . and .. is included
#define MSK_ALL_DIR 0x0010

// Directories, MSK_SUB_DIR | MSK_ALL_DIR
#define MSK_DIR		0x0011
// Files
#define MSK_FILE	0x0100

/**
 * list all files in directory
 @param path	directory path
 @param res		a vector to store files/sub dirs in specific path
 @param walk_mask walk mask, default parameter is walk files/sub dirs
 @see MSK_SUB_DIR	walk sub directories
 @see MSK_FILE		walk files

 examples: 
	int main(int argc, char* argv[])
	{
		std::vector<std::string> res;
		dir(argv[1], res, false);
		for(std::vector<std::string>::iterator iter = res.begin(); iter!=res.end(); iter++)
		{
		std::cout << *iter << std::endl;
		}
		return 0;
	}
 */
void dir(const char* path, std::vector<std::string>& res, int walk_mask = MSK_SUB_DIR | MSK_FILE);

#endif /* __FILE_WALKER_H__ */
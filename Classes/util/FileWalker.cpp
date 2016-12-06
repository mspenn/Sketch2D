#include "FileWalker.h"
#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <io.h>
#include <Windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif
#ifdef _DEBUG
#include <iostream>
#endif

/**
 * Walk dir and save files/sub directories in a vector container
 */
void dir(const char* path, std::vector<std::string>& res, int walk_mask)
{
#ifdef WIN32
	struct _finddata_t data;
	// find the first files when its name matched, return handle
	long hnd = _findfirst(path, &data);
	if (hnd < 0)
	{
		perror(path);
	}
	int  nRet = (hnd < 0) ? -1 : 1;
	while (nRet >= 0)
	{
		// if it is a directory
		if ((walk_mask & MSK_DIR) && data.attrib == _A_SUBDIR)
		{
			if ((walk_mask & MSK_ALL_DIR)
				|| ((walk_mask & MSK_SUB_DIR)
				&& (strcmp(data.name, ".")
				&& strcmp(data.name, ".."))))
			{
				res.push_back(data.name);
			}

		}

		if ((walk_mask & MSK_FILE) && data.attrib != _A_SUBDIR)
		{
			res.push_back(data.name);
		}

		nRet = _findnext(hnd, &data);
	}
	// close handle
	_findclose(hnd);
#else
	struct dirent* ent = NULL;
	DIR *pDir = opendir(path);

	while (NULL != (ent = readdir(pDir)))
	{
		// d_type: 4 -> dir, 8 -> normal_file
		// printf("reclen=%d, type=%d\n", ent->d_reclen, ent->d_type);

		if ((subdir_only && ent->d_type == 4 
			&& (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")))
			|| (!subdir_only 
			&& (ent->d_type == 8 ||ent->d_type == 4)))
		{
			res.push_back(ent->d_name);
		}
	}
	closedir(pDir);
#endif
}
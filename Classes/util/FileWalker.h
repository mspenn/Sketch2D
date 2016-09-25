#ifndef __FILE_WALKER_H__
#define __FILE_WALKER_H__

#include <vector>
#include <string>

#define MSK_SUB_DIR 0x0001
#define MSK_ALL_DIR 0x0010

#define MSK_DIR		0x0011
#define MSK_FILE	0x0100

void dir(const char* path, std::vector<std::string>& res, int walk_mask = MSK_SUB_DIR | MSK_FILE);

#endif
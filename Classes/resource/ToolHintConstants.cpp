#include "ToolHintConstants.h"
#include "cocos2d.h"
#include <string>

const char* TOOL_HINT_RECOGNIZE_SUCC[] = 
{
	TOOL_HINT_RECOGNIZE_SUCC_1,
	TOOL_HINT_RECOGNIZE_SUCC_2,
	TOOL_HINT_RECOGNIZE_SUCC_3
};

const char* TOOL_HINT_RECOGNIZE_FAILED[] =
{
	TOOL_HINT_RECOGNIZE_FAILED_1,
	TOOL_HINT_RECOGNIZE_FAILED_2,
	TOOL_HINT_RECOGNIZE_FAILED_3
};

const char* GetToolHintRecognizeSucc(char* bufDst, const char* name, float score)
{
	// format name and score with Successful Tool Hint Template randomly
	int idx = (int)(CCRANDOM_0_1() * (sizeof(TOOL_HINT_RECOGNIZE_SUCC) / sizeof(char*)));
	sprintf(bufDst, TOOL_HINT_RECOGNIZE_SUCC[idx], name, (int)(score * 100));
	return bufDst;
}

const char* GetToolHintRecognizeFailed(char* bufDst, const char* name, float score)
{
	// format name and score with Falied Tool Hint Template randomly
	int idx = (int)(CCRANDOM_0_1() * (sizeof(TOOL_HINT_RECOGNIZE_FAILED) / sizeof(char*)));
	sprintf(bufDst, TOOL_HINT_RECOGNIZE_FAILED[idx], name, (int)(score * 100));
	return bufDst;
}
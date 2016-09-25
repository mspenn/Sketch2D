#ifndef __TOOL_HINT_CONST_H__
#define __TOOL_HINT_CONST_H__


#define TOOL_HINT_WELCOME "Hey guys, welcome! Draw something below ~"

#define TOOL_HINT_RECOGNIZE_SUCC_1 "OK, it's a %s, similarity to %d%% ~"
#define TOOL_HINT_RECOGNIZE_SUCC_2 "Yes Sir! This's a %s, %d%% aha ~"
#define TOOL_HINT_RECOGNIZE_SUCC_3 "Got it! A %s which is up to %d%% similarity~"

#define TOOL_HINT_RECOGNIZE_FAILED_1 "Ooops, maybe it's a %s, similarity to %d%% ~"
#define TOOL_HINT_RECOGNIZE_FAILED_2 "Sorry Sir, is it a %s? Just %d%% similarity ~"
#define TOOL_HINT_RECOGNIZE_FAILED_3 "I cannot follow~ It look like a %s which is low to %d%% similarity~"

const char* GetToolHintRecognizeSucc(char* bufDst, const char* name, float score);
const char* GetToolHintRecognizeFailed(char* bufDst, const char* name, float score);

#endif
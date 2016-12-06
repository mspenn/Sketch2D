#ifndef __TOOL_HINT_CONST_H__
#define __TOOL_HINT_CONST_H__

// Welcome Tool Hint String
#define TOOL_HINT_WELCOME "Hey guys, welcome! Draw something below ~"

// Tool Hint when reconginze successfully
#define TOOL_HINT_RECOGNIZE_SUCC_1		"OK, it's a %s, similarity to %d%% ~"
#define TOOL_HINT_RECOGNIZE_SUCC_2		"Yes Sir! This's a %s, %d%% aha ~"
#define TOOL_HINT_RECOGNIZE_SUCC_3		"Got it! A %s which is up to %d%% similarity~"

// Tool Hint when reconginze failed
#define TOOL_HINT_RECOGNIZE_FAILED_1	"Ooops, maybe it's a %s, similarity to %d%% ~"
#define TOOL_HINT_RECOGNIZE_FAILED_2	"Sorry Sir, is it a %s? Just %d%% similarity ~"
#define TOOL_HINT_RECOGNIZE_FAILED_3	"I cannot follow~ It look like a %s which is low to %d%% similarity~"

#define TOOL_HINT_LOADING_PROGRESS		"Loading Templates, %d%%..."
#define TOOL_HINT_LOADING_ACTIVATING	"Activating Templates..."
#define TOOL_HINT_LOADING_COMPLETED		"Load Templates, Done!"
/**
 * Get Success Tool Hint
 * @param bufDst	destination to store formatted hint
 * @param name		recongnized gesture name
 * @param score		recongnized score
 * @return			char array pointer, hint content
 */
const char* GetToolHintRecognizeSucc(char* bufDst, const char* name, float score);

/**
 * Get Failed Tool Hint
 * @param bufDst	destination to store formatted hint
 * @param name		recongnized gesture name
 * @param score		recongnized score
 * @return			char array pointer, hint content
 */
const char* GetToolHintRecognizeFailed(char* bufDst, const char* name, float score);

#endif
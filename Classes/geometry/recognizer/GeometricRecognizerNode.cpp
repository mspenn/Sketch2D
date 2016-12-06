#include "GeometricRecognizerNode.h"
#include "util/SampleFileIOHelper.h"
#include "util/FileWalker.h"
#include "resource/Resources.h"
#include "resource/ToolHintConstants.h"

USING_NS_CC;
using namespace DollarRecognizer;

float loadingProgress = 0;

bool GeometricRecognizerNode::init()
{
	// call super init, Node::init
	if (!Node::init())
	{
		return false;
	}

	// start a thread to loading template
	std::thread tLoadingTemplate([&](){
		loadingProgress = 0;
		MultiStrokeGesture multiStrokeGesture;
		vector<string> mgestureList;

		// template type names
		string templateNames[] = 
		{ 
			// it seems "Line" symbol cannot be recognized by $N algorithm together with the following symbols
			//"Line",			
			"Circle", 
			"Triangle",
			"Rectangle", 
			"Pawn",
			"Pistal", 
			"Static", 
			"Delete", 
			"Check", 
			"Groove",
			"Spring" 
		};

		// scan each template type name
		for (auto& item : templateNames)
		{
			scanGestureSamples(item, multiStrokeGesture, mgestureList);
		}

		// feedback loading start
		loadProgressActiveFeedBack();

		// active templates
		_geometricRecognizer.activateMultiStrokesTemplates(mgestureList);
		
		// feedback loading completed
		loadProgressDoneFeedBack();
	});

	// thread start
	tLoadingTemplate.detach();

	return true;
}

MultiStrokeGesture& GeometricRecognizerNode::loadTemplate(
	std::string name, 
	const char* path, 
	DollarRecognizer::MultiStrokeGesture& multiStrokeGesture)
{
	// register stroke gestrure resources with detail names to recognizer 
	// if loadSample called successfully
	if (SampleFileIOHelper::loadSample(name, path, multiStrokeGesture))
	{
		_geometricRecognizer.addMultiStrokesTemplate(name, multiStrokeGesture);
	}
	return multiStrokeGesture;
}

void GeometricRecognizerNode::storeAsTemplate(
	const DollarRecognizer::MultiStrokeGesture& multiStrokeGesture, 
	const char* path)
{
	SampleFileIOHelper::storeSample(multiStrokeGesture, path);
}

void GeometricRecognizerNode::scanGestureSamples(
	const string& gestureName, 
	DollarRecognizer::MultiStrokeGesture& multiStrokeGesture,
	vector<string>& mgestureList)
{
	vector<string> gestureSampleList;
	char sampleFileNamePattern[100];
	sprintf(sampleFileNamePattern, PAT_GESTURE_FILTER, gestureName.c_str());
	// list all avaible files, end with ges
	dir(sampleFileNamePattern, gestureSampleList);

	// load gesture sample list, and register to recognizer with detail names
	for (unsigned int i = 0; i < gestureSampleList.size(); i++)
	{
		auto& gestureSampleName = gestureSampleList[i];
		
		char gestureSamplePath[MAX_PATH];
		sprintf(gestureSamplePath, PAT_GESTURE_PATH, gestureName.c_str(), gestureSampleName.c_str());
		sprintf(sampleFileNamePattern, "%s%s%d", gestureName.c_str(), GEOMETRY_SAMPLE_SEPERATOR, i);
		string sampleName(sampleFileNamePattern);
		loadTemplate(sampleName, gestureSamplePath, multiStrokeGesture);
		mgestureList.push_back(sampleName);
	}
	loadProgressFeedBack();
}

void GeometricRecognizerNode::loadProgressFeedBack()
{
	// feedback to ui, need to switch from current thread to main UI thread
	// otherwise you may get concurrent exceptions
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&](){
		// construct event arguments
		EventCustom e(EVENT_LOADING_TEMPLATE);
		loadingProgress += 5.0f;
		char buf[100];
		sprintf(buf, TOOL_HINT_LOADING_PROGRESS, int(loadingProgress));
		LoadTemplateData ltd(loadingProgress, string(buf));
		e.setUserData((void*)&ltd);
		// send event to notify all subscribers/observers
		_eventDispatcher->dispatchEvent(&e);
	});
}
void GeometricRecognizerNode::loadProgressActiveFeedBack()
{
	// feedback to ui, need to switch from current thread to main UI thread
	// otherwise you may get concurrent exceptions
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&](){
		// construct event arguments
		EventCustom e(EVENT_LOADING_TEMPLATE);
		LoadTemplateData ltd(loadingProgress, string(TOOL_HINT_LOADING_ACTIVATING));
		e.setUserData((void*)&ltd);
		// send event to notify all subscribers/observers
		_eventDispatcher->dispatchEvent(&e);
	});
}

void GeometricRecognizerNode::loadProgressDoneFeedBack()
{
	// feedback to ui, need to switch from current thread to main UI thread
	// otherwise you may get concurrent exceptions
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&](){
		cocos2d::log(TOOL_HINT_LOADING_COMPLETED);
		EventCustom e(EVENT_LOADED_TEMPLATE);
		LoadTemplateData ltd(100, TOOL_HINT_LOADING_COMPLETED);
		e.setUserData((void*)&ltd);
		// send event to notify all subscribers/observers
		_eventDispatcher->dispatchEvent(&e);
	});
}
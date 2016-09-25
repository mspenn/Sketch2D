#include "GeometricRecognizerNode.h"
#include "util/SampleFileIOHelper.h"
#include "util/FileWalker.h"
#include "resource/Resources.h"

USING_NS_CC;
using namespace DollarRecognizer;

float loadingProgress = 0;

bool GeometricRecognizerNode::init()
{
	if (!Node::init())
	{
		return false;
	}

	//_geometricRecognizer.loadMultistrokeTemplates();
	std::thread tAi([&](){
		loadingProgress = 0;
		MultiStrokeGesture multiStrokeGesture;
		vector<string> mgestureList;
		scanGestureSamples("Line", multiStrokeGesture, mgestureList);
		scanGestureSamples("Circle", multiStrokeGesture, mgestureList);
		scanGestureSamples("Triangle", multiStrokeGesture, mgestureList);
		scanGestureSamples("Rectangle", multiStrokeGesture, mgestureList);
		scanGestureSamples("Pawn", multiStrokeGesture, mgestureList);
		scanGestureSamples("Pistal", multiStrokeGesture, mgestureList);
		scanGestureSamples("Static", multiStrokeGesture, mgestureList);
		scanGestureSamples("Delete", multiStrokeGesture, mgestureList);
		scanGestureSamples("Check", multiStrokeGesture, mgestureList);

		loadProgressActiveFeedBack();
		_geometricRecognizer.activateMultiStrokesTemplates(mgestureList);
		
		loadProgressDoneFeedBack();
	});
	tAi.detach();




	/*loadTemplate("Line", RES_GESTURE_MESH(line), multiStrokeGesture);
	loadTemplate("Circle", RES_GESTURE_MESH(circle), multiStrokeGesture);
	loadTemplate("Triangle", RES_GESTURE_MESH(triangle), multiStrokeGesture);
	loadTemplate("Rectangle", RES_GESTURE_MESH(rectangle), multiStrokeGesture);
	loadTemplate("Pawn", RES_GESTURE_MESH(pawn), multiStrokeGesture);
	loadTemplate("Pistal", RES_GESTURE_MESH(pistal), multiStrokeGesture);
	loadTemplate("StaticMark", RES_GESTURE_MARKER(static), multiStrokeGesture);
	loadTemplate("DeleteMark", RES_GESTURE_MARKER(delete), multiStrokeGesture);
	loadTemplate("Spring", RES_GESTURE_MESH(spring), multiStrokeGesture);
	loadTemplate("CheckMark", RES_GESTURE_MARKER(check), multiStrokeGesture);*/
	//loadTemplate("Groove", "res/groove.ges", multiStrokeGesture);

	/*MgestureList.push_back("T");
	MgestureList.push_back("X");
	MgestureList.push_back("P");
	MgestureList.push_back("D");
	MgestureList.push_back("I");
	MgestureList.push_back("N");*/

	/*mgestureList.push_back("Line");
	mgestureList.push_back("Circle");
	mgestureList.push_back("Triangle");
	mgestureList.push_back("Rectangle");
	mgestureList.push_back("Pawn");
	mgestureList.push_back("Pistal");
	mgestureList.push_back("StaticMark");
	mgestureList.push_back("DeleteMark");
	mgestureList.push_back("Spring");
	mgestureList.push_back("CheckMark");*/
	//mgestureList.push_back("Groove");
	

	
	return true;
}

MultiStrokeGesture& GeometricRecognizerNode::loadTemplate(std::string name, const char* path, DollarRecognizer::MultiStrokeGesture& multiStrokeGesture)
{
	if (SampleFileIOHelper::loadSample(name, path, multiStrokeGesture))
	{
		_geometricRecognizer.addMultiStrokesTemplate(name, multiStrokeGesture);
	}
	return multiStrokeGesture;
}

void GeometricRecognizerNode::storeAsTemplate(const DollarRecognizer::MultiStrokeGesture& multiStrokeGesture, const char* path)
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
	sprintf(sampleFileNamePattern, "gestures/%s/*.ges", gestureName.c_str());
	dir(sampleFileNamePattern, gestureSampleList);
	

	// load gesture sample list
	for (unsigned int i = 0; i < gestureSampleList.size(); i++)
	{
		auto& gestureSampleName = gestureSampleList[i];
		
		string gestureSamplePath = "gestures/" + gestureName + "/" + gestureSampleName;
		sprintf(sampleFileNamePattern, "%s%s%d", gestureName.c_str(), GEOMETRY_SAMPLE_SEPERATOR, i);
		string sampleName(sampleFileNamePattern);
		loadTemplate(sampleName, gestureSamplePath.c_str(), multiStrokeGesture);
		mgestureList.push_back(sampleName);
	}
	loadProgressFeedBack();
}

void GeometricRecognizerNode::loadProgressFeedBack()
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&](){
		EventCustom e(EVENT_LOADING_TEMPLATE);
		loadingProgress += 5.0f;
		char buf[100];
		sprintf(buf, "Loading Templates, %d%%...", int(loadingProgress));
		LoadTemplateData ltd(loadingProgress, string(buf));
		e.setUserData((void*)&ltd);
		_eventDispatcher->dispatchEvent(&e);
	});
}
void GeometricRecognizerNode::loadProgressActiveFeedBack()
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&](){
		EventCustom e(EVENT_LOADING_TEMPLATE);
		LoadTemplateData ltd(loadingProgress, string("Activating Templates..."));
		e.setUserData((void*)&ltd);
		_eventDispatcher->dispatchEvent(&e);
	});
}

void GeometricRecognizerNode::loadProgressDoneFeedBack()
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&](){
		// execute code on main thread
		cocos2d::log("Load Templates Done!");
		EventCustom e(EVENT_LOADED_TEMPLATE);
		LoadTemplateData ltd(100, "Load Templates, Done!");
		e.setUserData((void*)&ltd);
		_eventDispatcher->dispatchEvent(&e);
	});
}
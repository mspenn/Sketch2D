#ifndef __GEOMETRIC_RECOGNIZER_NODE_H__
#define __GEOMETRIC_RECOGNIZER_NODE_H__

#include "cocos2d.h"
#include "gesture\GeometricRecognizer.h"

#define EVENT_LOADING_TEMPLATE			"onLoadingTemplate"
#define EVENT_LOADED_TEMPLATE			"onLoadedTemplate"
struct LoadTemplateData
{
	LoadTemplateData(int progress, const std::string& hint)
	{
		_progress = progress;
		_hint = hint;
	}
	int _progress;
	std::string _hint;
};

class GeometricRecognizerNode :public cocos2d::Node
{
public:
	virtual bool init();

	DollarRecognizer::MultiStrokeGesture& loadTemplate(std::string name, const char* path, DollarRecognizer::MultiStrokeGesture& multiStrokeGesture);
	void storeAsTemplate(const DollarRecognizer::MultiStrokeGesture& multiStrokeGesture, const char* path);
	DollarRecognizer::GeometricRecognizer* getGeometricRecognizer(){ return &_geometricRecognizer; }

	void scanGestureSamples(const string& gestureName, DollarRecognizer::MultiStrokeGesture& multiStrokeGesture, vector<string>& mgestureList);
	void loadProgressFeedBack();
	void loadProgressActiveFeedBack();
	void loadProgressDoneFeedBack();
	CREATE_FUNC(GeometricRecognizerNode);
private:
	DollarRecognizer::GeometricRecognizer _geometricRecognizer;
};




#endif
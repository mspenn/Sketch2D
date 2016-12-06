#ifndef __GEOMETRIC_RECOGNIZER_NODE_H__
#define __GEOMETRIC_RECOGNIZER_NODE_H__

#include "cocos2d.h"
#include "gesture\GeometricRecognizer.h"

// event
#define EVENT_LOADING_TEMPLATE			"onLoadingTemplate"
#define EVENT_LOADED_TEMPLATE			"onLoadedTemplate"

/**
 * Load Template Data for Update Feedback Event
 */
struct LoadTemplateData
{
	// Constructor
	LoadTemplateData(int progress, const std::string& hint)
	{
		_progress = progress;
		_hint = hint;
	}

	int _progress;		// current progress volunm
	std::string _hint;	// hint content/message
};

/**
 * Geometric Recognizer Node
 * It can be attached to a game scene, and used everywhere in the scene 
 * @see cocos2d::Node
 */
class GeometricRecognizerNode :public cocos2d::Node
{
public:

	/**
	 * Call when Geometric Recognizer Node is initialized, override
	 * @see cocos2d::Node::init
	 * @return true if initialize successfully, false otherwise
	 */
	virtual bool init();

	/**
	 * Load template as multiply stroke
	 * @param name					template name
	 * @param path					template path in file system
	 * @param multiStrokeGesture	a empty MultiStrokeGesture object
	 *								will be fill in template data after call
	 * @return reference to a existing MultiStrokeGesture object
	 */
	DollarRecognizer::MultiStrokeGesture& loadTemplate(
		std::string name, 
		const char* path, 
		DollarRecognizer::MultiStrokeGesture& multiStrokeGesture);

	/**
	 * Store multiply stroke as	template file
	 * @param multiStrokeGesture	a reference to existing MultiStrokeGesture object
	 * @param path					template path in file system
	 */
	void storeAsTemplate(
		const DollarRecognizer::MultiStrokeGesture& multiStrokeGesture,
		const char* path);

	/**
	 * Get GeometricRecognizer object
	 * @return pointer to GeometricRecognizer instance
	 */
	inline DollarRecognizer::GeometricRecognizer* getGeometricRecognizer()
	{
		return &_geometricRecognizer;
	}

	/**
	* Scan Gesture Samples by specified gesture name
	* @param gestureName		specified gesture name, such as Cycle, Rectangle, etc
	* @param multiStrokeGesture a empty MultiStrokeGesture object to store data
	* @param mgestureList		sample file detail names, such as Cycle::1, Cycle::2, etc
	*/
	void scanGestureSamples(
		const string& gestureName, 
		DollarRecognizer::MultiStrokeGesture& multiStrokeGesture,
		vector<string>& mgestureList);
	
	/**
	 * Feedback loading template progress
	 * send EVENT_LOADING_TEMPLATE event
	 */
	void loadProgressFeedBack();
	
	/**
	 * Feedback when loading progress is active
	 * send EVENT_LOADING_TEMPLATE event
	 */
	void loadProgressActiveFeedBack();
	
	/**
	 * Feedback when loading progress is done
	 * send EVENT_LOADED_TEMPLATE event
	 */
	void loadProgressDoneFeedBack();

	/**
	 * Static factory to create GeometricRecognizerNode object
	 */
	CREATE_FUNC(GeometricRecognizerNode);

private:
	DollarRecognizer::GeometricRecognizer _geometricRecognizer;	// dollar recognizer instance
};

#endif	/* __GEOMETRIC_RECOGNIZER_NODE_H__ */
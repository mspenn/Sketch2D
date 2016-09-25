#ifndef __TRAINING_SCENE_H__
#define __TRAINING_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

// events
#define EVENT_ON_LOAD_MESH		"onLoadMesh"
#define EVENT_ON_UNLOAD_MESH	"onUnloadMesh"
#define EVENT_ON_LOAD_MARKER	"onLoadMarker"
#define EVENT_ON_UNLOAD_MARKER	"onUnloadMarker"

class TrainingScene : public cocos2d::Scene
{
public:
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	CREATE_FUNC(TrainingScene);
private:

};

class CategoryLayer:public cocos2d::Layer
{
public:
	virtual bool init();
	virtual void onEnter();
//	virtual void onExit();
	void setActiveMeshCategory(bool isActive);
	void setActiveMarkerCategory(bool isActive);
	void onLoadMesh(cocos2d::EventCustom* event);
	void onUnloadMesh(cocos2d::EventCustom* event);
	void onLoadMarker(cocos2d::EventCustom* event);
	void onUnloadMarker(cocos2d::EventCustom* event);

	CREATE_FUNC(CategoryLayer);
private:
	cocos2d::ui::ScrollView* _scrollViewCategory;
	cocos2d::ui::CheckBox* _checkBoxMesh;
	cocos2d::ui::CheckBox* _checkBoxMarker;

	cocos2d::EventListenerCustom* _loadMeshListener;
	cocos2d::EventListenerCustom* _unloadMeshListener;
	cocos2d::EventListenerCustom* _loadMarkerListener;
	cocos2d::EventListenerCustom* _unloadMarkerListener;
};

class PreviewLayer :public cocos2d::Layer
{
public:
	virtual bool init();
	virtual void onEnter();
	void createNewSample();
	void deleteSample();
private:
	cocos2d::Label* _labelType;
	cocos2d::Label* _labelName;
	cocos2d::Label* _labelFileName;

	cocos2d::EventListenerCustom* _previewSampleListener;
};

/*class TrainingCanvasLayer :public cocos2d::Layer
{
public:
	virtual bool init();
	virtual void onEnter();
private:


	//cocos2d::EventListenerCustom* _previewItemListener;
};*/

#endif
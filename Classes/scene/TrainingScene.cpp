#include "TrainingScene.h"
#include "resource/Resources.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool TrainingScene::init()
{
	if (!Scene::init()){ return false; }
	
	auto category = CategoryLayer::create();
	this->addChild(category);

	return true;
}

void TrainingScene::onEnter()
{
	Scene::onEnter();
}

void TrainingScene::onExit()
{
	Scene::onExit();
}

bool CategoryLayer::init()
{
	if (!Layer::init()){ return false; }
	Size widgetSize = Size(200, 800);

	Text* alert = Text::create("ScrollView vertical", "fonts/Marker Felt.ttf", 30);
	alert->setColor(Color3B(159, 168, 176));
	alert->setPosition(Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
	this->addChild(alert);

	// Create the checkbox
	_checkBoxMesh = CheckBox::create("cocosui/check_box_normal.png",
		"cocosui/check_box_normal_press.png",
		"cocosui/check_box_active.png",
		"cocosui/check_box_normal_disable.png",
		"cocosui/check_box_active_disable.png");
	_checkBoxMesh->setPosition(Vec2(200, 650));
	_checkBoxMesh->addEventListener([&](Ref* pSender, CheckBox::EventType type){
		switch (type)
		{
		case CheckBox::EventType::SELECTED:
		{
											  EventCustom event(EVENT_ON_LOAD_MESH);
											  _eventDispatcher->dispatchEvent(&event);
											  break;
		}
		case CheckBox::EventType::UNSELECTED:
		{
												EventCustom event(EVENT_ON_UNLOAD_MESH);
												_eventDispatcher->dispatchEvent(&event);
												break;
		}
		default:
			break;
		}
	});

	this->addChild(_checkBoxMesh);
	_checkBoxMarker = CheckBox::create("cocosui/check_box_normal.png",
		"cocosui/check_box_normal_press.png",
		"cocosui/check_box_active.png",
		"cocosui/check_box_normal_disable.png",
		"cocosui/check_box_active_disable.png");
	_checkBoxMarker->setPosition(Vec2(200, 600));
	_checkBoxMarker->addEventListener([&](Ref* pSender, CheckBox::EventType type){
		switch (type)
		{
		case CheckBox::EventType::SELECTED:
		{
											  EventCustom event(EVENT_ON_LOAD_MARKER);;
											  _eventDispatcher->dispatchEvent(&event);
											  break;
		}
		case CheckBox::EventType::UNSELECTED:
		{
												EventCustom event(EVENT_ON_UNLOAD_MARKER);
												_eventDispatcher->dispatchEvent(&event);
												break;
		}
		default:
			break;
		}
	});
	this->addChild(_checkBoxMarker);

	//_uiLayer->addChild(_checkBox);


	_scrollViewCategory = ui::ScrollView::create();
	_scrollViewCategory->setContentSize(Size(280.0f, 100.0f));
	Size backgroundSize = Size(200, 800);// background->getContentSize();
	_scrollViewCategory->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
		(backgroundSize.width - _scrollViewCategory->getContentSize().width) / 2.0f,
		(widgetSize.height - backgroundSize.height) / 2.0f +
		(backgroundSize.height - _scrollViewCategory->getContentSize().height) / 2.0f));
	_scrollViewCategory->setScrollBarWidth(4);
	_scrollViewCategory->setScrollBarPositionFromCorner(Vec2(2, 2));
	_scrollViewCategory->setScrollBarColor(Color3B::WHITE);
	this->addChild(_scrollViewCategory);

	return true;
}

void CategoryLayer::onEnter()
{
	Layer::onEnter();

	_loadMeshListener = EventListenerCustom::create(EVENT_ON_LOAD_MESH, CC_CALLBACK_1(CategoryLayer::onLoadMesh, this));
	_unloadMeshListener = EventListenerCustom::create(EVENT_ON_UNLOAD_MESH, CC_CALLBACK_1(CategoryLayer::onUnloadMesh, this));
	_loadMarkerListener = EventListenerCustom::create(EVENT_ON_LOAD_MARKER, CC_CALLBACK_1(CategoryLayer::onLoadMarker, this));
	_unloadMarkerListener = EventListenerCustom::create(EVENT_ON_UNLOAD_MARKER, CC_CALLBACK_1(CategoryLayer::onUnloadMarker, this));

	_eventDispatcher->addEventListenerWithFixedPriority(_loadMeshListener, 2);
	_eventDispatcher->addEventListenerWithFixedPriority(_unloadMeshListener, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_loadMarkerListener, 2);
	_eventDispatcher->addEventListenerWithFixedPriority(_unloadMarkerListener, 1);

	setActiveMeshCategory(true);
}

void CategoryLayer::setActiveMeshCategory(bool isActive)
{
	_checkBoxMesh->setSelected(isActive);
	EventCustom event(isActive ? EVENT_ON_LOAD_MESH : EVENT_ON_UNLOAD_MESH);
	_eventDispatcher->dispatchEvent(&event);
}

void CategoryLayer::setActiveMarkerCategory(bool isActive)
{
	_checkBoxMarker->setSelected(isActive);
	EventCustom event(isActive ? EVENT_ON_LOAD_MARKER : EVENT_ON_UNLOAD_MARKER);
	_eventDispatcher->dispatchEvent(&event);
}

void CategoryLayer::onLoadMesh(cocos2d::EventCustom* event)
{
	// load circle
	ImageView* imageView = ImageView::create(RES_GESTURE_THUMB(triangle));
	//ImageView* imageView = ImageView::create("cocosui/ccicon.png");

	float innerWidth = _scrollViewCategory->getContentSize().width;
	float innerHeight = _scrollViewCategory->getContentSize().height + imageView->getContentSize().height;

	_scrollViewCategory->setInnerContainerSize(Size(innerWidth, innerHeight));

	Button* button = Button::create("cocosui/animationbuttonnormal.png", "cocosui/animationbuttonpressed.png");
	button->setPosition(Vec2(innerWidth / 2.0f, _scrollViewCategory->getInnerContainerSize().height - button->getContentSize().height / 2.0f));
	_scrollViewCategory->addChild(button);

	Button* titleButton = Button::create("cocosui/backtotopnormal.png", "cocosui/backtotoppressed.png");
	titleButton->setTitleText("Title Button");
	titleButton->setPosition(Vec2(innerWidth / 2.0f, button->getBottomBoundary() - button->getContentSize().height));
	_scrollViewCategory->addChild(titleButton);

	Button* button_scale9 = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
	button_scale9->setScale9Enabled(true);
	button_scale9->setContentSize(Size(100.0f, button_scale9->getVirtualRendererSize().height));
	button_scale9->setPosition(Vec2(innerWidth / 2.0f, titleButton->getBottomBoundary() - titleButton->getContentSize().height));
	_scrollViewCategory->addChild(button_scale9);

	imageView->setPosition(Vec2(innerWidth / 2.0f, imageView->getContentSize().height / 2.0f));
	_scrollViewCategory->addChild(imageView);
	_scrollViewCategory->scrollToBottom(2.0, true);
}

void CategoryLayer::onUnloadMesh(cocos2d::EventCustom* event)
{
	_scrollViewCategory->removeAllChildrenWithCleanup(true);
}

void CategoryLayer::onLoadMarker(cocos2d::EventCustom* event){}
void CategoryLayer::onUnloadMarker(cocos2d::EventCustom* event){}
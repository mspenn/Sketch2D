#include "scene/MenuScene.h"
#include "scene/SceneManager.h"
#include "util/VisibleRect.h"
#include "resource/Resources.h"

USING_NS_CC;

bool MenuScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();  
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	// set default fonts
	TTFConfig ttfConfig(RES_FONT(Marker Felt.ttf), 48);

	// training label
	_labelTraining = Label::createWithTTF(ttfConfig, "Train");
	auto trainItem = MenuItemLabel::create(_labelTraining, CC_CALLBACK_1(MenuScene::onTraining, this));
	trainItem->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 50));

	// gaming label
	_labelGaming = Label::createWithTTF(ttfConfig, "Game");
	auto gameItem = MenuItemLabel::create(_labelGaming, CC_CALLBACK_1(MenuScene::onGaming, this));
	gameItem->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 50));

	// add to menu
	auto menu = Menu::create(trainItem, gameItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void MenuScene::onTraining(Ref* sender)
{
	// switch to training scene
	auto scene = SceneManager::GetTrainingScene();
	Director::getInstance()->replaceScene(scene);
}

void MenuScene::onGaming(Ref* sender)
{
	// switch to gaming scene, 
	// also called a canvas scene
	// because we want to draw things
	auto scene = SceneManager::GetCanvasScene();
	Director::getInstance()->replaceScene(scene);
}
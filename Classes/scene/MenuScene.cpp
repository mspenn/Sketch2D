#include "scene/MenuScene.h"
#include "scene/SceneManager.h"
#include "util/VisibleRect.h"

USING_NS_CC;

bool MenuScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();  
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	TTFConfig ttfConfig("fonts/Marker Felt.ttf", 48);
	_labelTraining = Label::createWithTTF(ttfConfig, "Train");
	//Label::create("Train", "fonts/Marker Felt.ttf", 48, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	auto trainItem = MenuItemLabel::create(_labelTraining, CC_CALLBACK_1(MenuScene::onTraining, this));
	trainItem->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 50));

	_labelGaming = Label::createWithTTF(ttfConfig, "Game");
	auto gameItem = MenuItemLabel::create(_labelGaming, CC_CALLBACK_1(MenuScene::onGaming, this));
	gameItem->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 50));

	auto menu = Menu::create(trainItem, gameItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	return true;
}

void MenuScene::onTraining(Ref* sender)
{
	auto scene = SceneManager::GetTrainingScene();
	Director::getInstance()->replaceScene(scene);
}

void MenuScene::onGaming(Ref* sender)
{
	auto scene = SceneManager::GetCanvasScene();
	Director::getInstance()->replaceScene(scene);
}
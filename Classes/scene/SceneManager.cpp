#include "SceneManager.h"
#include "CanvasScene.h"
#include "HelloWorldScene.h"
#include "LogoScene.h"
#include "TrainingScene.h"
#include "ItemManageScene.h"
#include "MenuScene.h"

USING_NS_CC;

Scene* SceneManager::GetCanvasScene(){ return CanvasScene::create(); }
Scene* SceneManager::GetLogoScene(){ return LogoScene::create(); }
Scene* SceneManager::GetGameScene(){ return CanvasScene::create(); }
Scene* SceneManager::GetTrainingScene(){ return ItemManageScene::createScene(); }
Scene* SceneManager::GetTestScene(){ return HelloWorld::create()->getScene(); }
Scene* SceneManager::GetMenuScene(){ return MenuScene::create(); }
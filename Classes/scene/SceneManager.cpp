#include "SceneManager.h"
#include "CanvasScene.h"
#include "TrainingScene.h"
#include "ItemManageScene.h"
#include "MenuScene.h"

USING_NS_CC;

Scene* SceneManager::GetCanvasScene(){ return CanvasScene::create(); }
Scene* SceneManager::GetGameScene(){ return CanvasScene::create(); }
Scene* SceneManager::GetTrainingScene(){ return ItemManageScene::createScene(); }
Scene* SceneManager::GetMenuScene(){ return MenuScene::create(); }
#include "CanvasScene.h"
#include "SceneManager.h"
#include "controller/PawnController.h"

#include "resource/Resources.h"
#include "resource/ToolHintConstants.h"

USING_NS_CC;
using namespace DollarRecognizer;

Joints joints;
JointsList jointsList;

namespace
{
	const int DRAG_BODYS_TAG = 0x80;
	const Vec2 GRAVITY(0, -100);
}

Sprite* createBall(const Point& point, float radius, PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);
Sprite* createBox(const Point& point, Size size, PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);
Sprite* createTriangle(const Point& point, Size size, PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);

Sprite* createBall(const Point& point, float radius, PhysicsMaterial material)
{
	Sprite* ball = Sprite::create(RES_IMAGE(ball.png));
	ball->setScale(radius / 8);

	auto body = PhysicsBody::createCircle(radius, material);
	ball->setPhysicsBody(body);
	ball->setPosition(point);
	body->setTag(DRAG_BODYS_TAG);

	return ball;
}

Sprite* createBox(const Point& point, Size size, PhysicsMaterial material)
{
	Sprite* box = Sprite::create(RES_IMAGE(YellowSquare.png));
	box->setScale(size.width / 100.0f, size.height / 100.0f);

	auto body = PhysicsBody::createBox(size);
	box->setPhysicsBody(body);
	box->setPosition(point);
	box->setRotation(CCRANDOM_0_1() * 360);
	body->setTag(DRAG_BODYS_TAG);

	return box;
}

Sprite* createTriangle(const Point& point, Size size, PhysicsMaterial material)
{
	Sprite* triangle = Sprite::create();

	Point verts[] =
	{
		Point(0, size.height / 2),
		Point(size.width / 2, -size.height / 2),
		Point(-size.width / 2, -size.height / 2)
	};

	auto body = PhysicsBody::createPolygon(verts, 3, material);
	triangle->setPhysicsBody(body);
	triangle->setPosition(point);
	triangle->setRotation(CCRANDOM_0_1() * 360);
	triangle->setTag(DRAG_BODYS_TAG);

	return triangle;
}


CanvasScene::CanvasScene()
:_debugDraw(false)
{
}

bool CanvasScene::toggleDebugDraw()
{
#if CC_USE_PHYSICS
	_debugDraw = !_debugDraw;
	_physicsWorld->setDebugDrawMask(_debugDraw ? PhysicsWorld::DEBUGDRAW_ALL : PhysicsWorld::DEBUGDRAW_NONE);
#endif
	_toolLayer->toggleDebugMode(_debugDraw);
	return _debugDraw;
}

bool CanvasScene::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	_canvasLayer = GameCanvasLayer::create();
	this->addChild(_canvasLayer);

	_toolLayer = ToolLayer::create();
	_toolLayer->setParentScene(this);
	this->addChild(_toolLayer);

	return true;
}

void CanvasScene::onEnter()
{
	Scene::onEnter();
	_physicsWorld->setGravity(GRAVITY);


	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event * event){
		log("KeyPress:%d", keyCode);
		if (EventKeyboard::KeyCode::KEY_ENTER == keyCode)
		{
			this->_canvasLayer->save();
		}
		else if (EventKeyboard::KeyCode::KEY_R == keyCode)
		{
			this->_canvasLayer->recognize();
		}
		else if (EventKeyboard::KeyCode::KEY_D == keyCode)
		{
			this->_canvasLayer->redrawCurrentNode();
		}
		else if (EventKeyboard::KeyCode::KEY_T == keyCode)
		{
			this->_canvasLayer->train();
		}
		else if (EventKeyboard::KeyCode::KEY_C == keyCode)
		{
			if (!jointsList.empty())jointsList.pop_back();
		}
	};
	_keyboardListener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event * event){
		log("KeyRelease:%d", keyCode);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	/*Vec2 offset(200, 200);
	auto sp1 = makeTriangle(offset, Size(200,200));
	auto sp1PhysicsBody = sp1->getPhysicsBody();
	//sp1PhysicsBody->setTag(DRAG_BODYS_TAG);
	this->addChild(sp1);

	Vec2 offset(200, 200);
	auto sp1 = makeBall(offset - Vec2(30, 0), 10);
	auto sp1PhysicsBody = sp1->getPhysicsBody();
	sp1PhysicsBody->setTag(DRAG_BODYS_TAG);

	auto sp2 = makeBall(offset + Vec2(30, 0), 10);
	auto sp2PhysicsBody = sp2->getPhysicsBody();
	sp2PhysicsBody->setTag(DRAG_BODYS_TAG);

	PhysicsJointPin* joint = PhysicsJointPin::construct(sp1PhysicsBody, sp2PhysicsBody, offset);
	auto pw = getPhysicsWorld();
	pw->addJoint(joint);

	this->addChild(sp1);
	this->addChild(sp2);*/

}

void CanvasScene::onExit()
{
	this->stopAllActions();
	_eventDispatcher->removeEventListenersForTarget(this);
	Scene::onExit();
}

void CanvasScene::startSimulate()
{
	_gameLayer = _canvasLayer->createGameLayer();
	_gameLayer->setVisible(true);
	_canvasLayer->startGameSimulation();
	_canvasLayer->setVisible(false);
	_canvasLayer->stopAllActions();
	if (this->_debugDraw){ _physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); }
	_gameLayer->setParent(nullptr);
	this->addChild(_gameLayer);
	//this->_keyboardListener->setEnabled(false);
}

void CanvasScene::stopSimulate()
{
	_gameLayer->setVisible(false);
	_canvasLayer->setVisible(true);
	_gameLayer->stopAllActions();
	if (this->_debugDraw){ _physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE); }
	this->removeChild(_gameLayer);
	//this->_keyboardListener->setEnabled(true);
	_canvasLayer->stopGameSimulation();
}

bool GameCanvasLayer::init()
{
	if (!CanvasLayer::init())
	{
		return false;
	}

	auto _loadedTemplateListener = EventListenerCustom::create(EVENT_LOADED_TEMPLATE, [&](EventCustom* event){
		_keyboardListener->setEnabled(true);
		_mouseListener->setEnabled(true);
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_loadedTemplateListener, this);

	_geoRecognizer = GeometricRecognizerNode::create();
	this->addChild(_geoRecognizer);

	_preCmdHandlers.init();
	

	return true;
}

void GameCanvasLayer::onEnter()
{
	CanvasLayer::onEnter();
	_currentDrawNode = switchToNewDrawNode();
	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event * event){
		if (EventKeyboard::KeyCode::KEY_F5 == keyCode)
		{
			((CanvasScene*)(this->getScene()))->toggleDebugDraw();
		}
		else if (EventKeyboard::KeyCode::KEY_CTRL == keyCode)
		{
			this->_jointMode = true;
			joints.clear();
		}
		else if (EventKeyboard::KeyCode::KEY_ESCAPE == keyCode)
		{
			auto scene = SceneManager::GetMenuScene();
			Director::getInstance()->replaceScene(scene);
		}
	};
	_keyboardListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event * event){
		if (EventKeyboard::KeyCode::KEY_CTRL == keyCode)
		{
			this->_jointMode = false;
		}
	};
	_keyboardListener->setEnabled(false);
	_mouseListener->setEnabled(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	((CanvasScene*)(this->getScene()))->toggleDebugDraw();
	auto _world = this->getScene()->getPhysicsWorld();
/*
	Sprite* box = nullptr;
//	PhysicsJointDistance* chainJoint_distance = nullptr;
	int chainLen = 5;
	Vector<Sprite*> boxes;
	for (int i = 0; i < chainLen; i++) {
		box = createBox(Point(500 + 40 * i, 600), Size(20, 10));
		this->addChild(box);
		boxes.pushBack(box);
		if (i == 0)
		{
			box->getPhysicsBody()->setDynamic(false);
		}
		if (i > 0)
		{
			PhysicsJointDistance* chainJoint_distance = PhysicsJointDistance::construct(boxes.at(i - 1)->getPhysicsBody(), boxes.at(i)->getPhysicsBody(), Point(10, 0), Point(-10, 0));
			chainJoint_distance->createConstraints();
			log("DIS: %d\n", chainJoint_distance->getDistance());
			chainJoint_distance->setDistance(-10.f);
			_world->addJoint(chainJoint_distance);
		}
	}*/
	/*
#define ARROUND_BALLS_NUM 16
#define ARROUND_BALLS_RADIUS 0.1f
#define CENTER_BALL_POS Point(200, 200)
#define SOFT_BODY_RADIUS 40.0f
	{
		auto box1 = createBox(Point(400, 200), Size(100, 100));
		auto box2 = createBox(Point(300, 300), Size(40, 40));
		this->addChild(box1);
		this->addChild(box2);
		
		box1->getPhysicsBody()->getShape(0)->setFriction(0.0f);
		box2->getPhysicsBody()->getShape(0)->setFriction(0.0f);
		box1->getPhysicsBody()->setDynamic(false);
		schedule([box1](float d){
			box1->setRotation(20);
		},"test");
		PhysicsJointSpring* joint = PhysicsJointSpring::construct(box1->getPhysicsBody(), box2->getPhysicsBody(), Point::ANCHOR_MIDDLE, Point::ANCHOR_MIDDLE, 1000.0f, 0.8f);
		_world->addJoint(joint);
	}*/

	/*
	// to be finished 
	{
		auto centerBall = createBall(CENTER_BALL_POS, SOFT_BODY_RADIUS);
		centerBall->getPhysicsBody()->setCategoryBitmask(0x02);
		centerBall->getPhysicsBody()->setContactTestBitmask(0x01);
		centerBall->getPhysicsBody()->setMass(1.0f / (float)ARROUND_BALLS_NUM);
		this->addChild(centerBall);

		auto test = 2 * SOFT_BODY_RADIUS*sinf(2 * M_PI / (float)ARROUND_BALLS_NUM);
		Vector<Sprite*> ballsArround;
		for (int i = 0; i < ARROUND_BALLS_NUM; i++)
		{
			auto angle = 2 * M_PI / ARROUND_BALLS_NUM * i;
			Point pos = CENTER_BALL_POS + Point::forAngle(angle) * SOFT_BODY_RADIUS;
			auto ball = createBall(pos, test / 2);
			ball->getPhysicsBody()->setMoment(PHYSICS_INFINITY);
			ball->getPhysicsBody()->setMass(1.0f / (float)ARROUND_BALLS_NUM);
			ball->getPhysicsBody()->setCategoryBitmask(0x02);
			ball->getPhysicsBody()->setContactTestBitmask(0x01);
			ball->getPhysicsBody()->getShape(0)->setFriction(1.0f);
			ball->getPhysicsBody()->getShape(0)->setFriction(0.5f);
			this->addChild(ball);
			ballsArround.pushBack(ball);

			auto body = ballsArround.at(i)->getPhysicsBody();
			PhysicsJointLimit* limitJoint = PhysicsJointLimit::construct(centerBall->getPhysicsBody(), body, Point::forAngle(angle) * SOFT_BODY_RADIUS, Point::ZERO, 0, SOFT_BODY_RADIUS*0.1);

			PhysicsJointSpring* springJoint = PhysicsJointSpring::construct(centerBall->getPhysicsBody(), body, Point::forAngle(angle) * (SOFT_BODY_RADIUS + test / 2 + 4), Point::ZERO, 60, 0.75);
			springJoint->createConstraints();
			springJoint->setRestLength(20);
			_world->addJoint(limitJoint);
			_world->addJoint(springJoint);
		}

		for (int i = 0; i < ARROUND_BALLS_NUM; i++)
		{
			auto body = ballsArround.at(i)->getPhysicsBody();
			auto nextArroundBody = ballsArround.at((i + 1) % ARROUND_BALLS_NUM)->getPhysicsBody();

			PhysicsJointLimit* arroundEachLimitJoint = PhysicsJointLimit::construct(body, nextArroundBody, Point::ZERO, Point::ZERO, 0, test);
			_world->addJoint(arroundEachLimitJoint);
		}

		centerBall->getPhysicsBody()->applyImpulse(Point(400, 400));
	}*/
}

void GameCanvasLayer::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	Layer::onExit();
}

void GameCanvasLayer::onMouseDown(cocos2d::EventMouse* event)
{
	if (_jointMode)
	{
		for (auto p = _drawNodeResultMap.begin(); p != _drawNodeResultMap.end(); p++)
		{
			auto& rs = p->second;
			if (rs._priority == 5)
			{
				if (rs._drawNode->containsPoint(event->getLocationInView()))
				{
					joints.push_back(rs._drawNode);
					break;
				}
			}
		}
		//joints.push_back(event->getLocationInView());
		return;
	}
	CanvasLayer::onMouseDown(event);
}

DrawableSprite* GameCanvasLayer::switchToNewDrawNode()
{
	auto drawNode = DrawableSprite::create();
	drawNode->setGeoRecognizer(this->_geoRecognizer->getGeometricRecognizer());
	this->addChild(drawNode, 10);
	this->_drawNodeList.push_back(drawNode);
	return drawNode;
}

void GameCanvasLayer::startGameSimulation()
{
	_mouseListener->setEnabled(false);
}
void GameCanvasLayer::stopGameSimulation()
{
	_mouseListener->setEnabled(true);
}
void GameCanvasLayer::removeUnrecognizedSprite(DrawableSprite* target)
{
	auto actionBlink = Blink::create(1.5f, 3); 
	auto actionDone = CallFuncN::create([&](Node* node)
	{
		this->removeChild(node);
	});
	Sequence* sequence = Sequence::create(actionBlink, actionDone, NULL);
	target->runAction(sequence);
}

void GameCanvasLayer::save()
{
	if (_jointMode)
	{
		//joints.push_back(joints[0]);
		jointsList.push_back(joints);
		joints.clear();
		return;
	}
	RecognitionResult result = _currentDrawNode->recognize();
	RecognizedSprite rs(result, _currentDrawNode);
	if (result.score < 0.75f)
	{
		log("Geometric Recognize Failed. Guess: %s, Score: %f", result.name.c_str(), result.score);
		EventCustom event(EVENT_RECOGNIZE_FAILED);
		event.setUserData((void*)&rs);
		_eventDispatcher->dispatchEvent(&event);
		removeUnrecognizedSprite(_currentDrawNode);
	}
	else
	{
		CommandHandler cmdh = _preCmdHandlers.getCommandHandler(rs.getGeometricType());
		if (!cmdh._Empty())cmdh(rs, _drawNodeList, this, &_drawNodeResultMap);

		// dispatch success event
		EventCustom event(EVENT_RECOGNIZE_SUCCESS);
		event.setUserData((void*)&rs);
		_eventDispatcher->dispatchEvent(&event);
	}

	_currentDrawNode = switchToNewDrawNode();
}

GameLayer* GameCanvasLayer::createGameLayer()
{
	return GameLayer::create(this->_drawNodeList, this->_drawNodeResultMap, this->getScene());
}

void GameCanvasLayer::recognize()
{
	_currentDrawNode->recognize();
}

void GameCanvasLayer::redrawCurrentNode()
{
	_currentDrawNode->redraw();
}

void GameCanvasLayer::train()
{
	MultiStrokeGesture multiStrokes;
	_currentDrawNode->getMultiStrokeGesture(multiStrokes);
	_geoRecognizer->storeAsTemplate(multiStrokes, "res/pawn.ges");

	//MultiStrokeGesture msg;
	//_geoRecognizer->loadTemplate("", "res/line.ges", msg);
}

bool ToolLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

#define GAP 20.0f 
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_spriteAssistant = Sprite::create("images/assistant.png");
	_spriteAssistant->setScale(0.5f);
	_spriteAssistant->setPosition(Vec2(origin.x + _spriteAssistant->getContentSize().width / 4 + GAP,
		origin.y + visibleSize.height - _spriteAssistant->getContentSize().height / 4 - GAP));
	//log("%f,%f", _spriteAssistant->getContentSize().width / 4, _spriteAssistant->getContentSize().height / 4);
	this->addChild(_spriteAssistant);

	_labelHint = Label::create(TOOL_HINT_WELCOME, "fonts/Marker Felt.ttf", 24, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	_labelHint->setAnchorPoint(Vec2(0, 0));
	_labelHint->setPosition(Vec2(origin.x + _spriteAssistant->getContentSize().width / 2 + 4 * GAP,
		origin.y + visibleSize.height - _spriteAssistant->getContentSize().height / 4 - GAP));
	//_labelHint->setOpacity(200);
	this->addChild(_labelHint);

	// add a "start simulate/stop simulate" icon to exit the progress. it's an autorelease object
	_menuStartSimulate = MenuItemImage::create(
		RES_IMAGE(StartSimulate.png),
		RES_IMAGE(StartSimulateHover.png),
		CC_CALLBACK_1(ToolLayer::startSimulateCallback, this));
	_menuStartSimulate->setScale(0.5f);
	_menuStartSimulate->setRotation(90.0f);
	_menuStartSimulate->setPosition(Vec2(origin.x + visibleSize.width - _menuStartSimulate->getContentSize().width / 2,
		origin.y + visibleSize.height - _menuStartSimulate->getContentSize().height / 2));
	_menuStartSimulate->setVisible(true);

	_menuStopSimulate = MenuItemImage::create(
		RES_IMAGE(StopSimulate.png),
		RES_IMAGE(StopSimulateHover.png),
		CC_CALLBACK_1(ToolLayer::stopSimulateCallback, this));
	_menuStopSimulate->setScale(0.5f);
	_menuStopSimulate->setPosition(Vec2(origin.x + visibleSize.width - _menuStopSimulate->getContentSize().width / 2,
		origin.y + visibleSize.height - _menuStopSimulate->getContentSize().height / 2));
	_menuStopSimulate->setVisible(false);

	/*_menuStartJoint = MenuItemImage::create(
		RES_IMAGE(StartJointMode.png),
		RES_IMAGE(StartJointModeHover.png),
		CC_CALLBACK_1(ToolLayer::startJointModeCallback, this));
	_menuStartJoint->setScale(0.5f);
	_menuStartJoint->setPosition(Vec2(origin.x + visibleSize.width - _menuStartSimulate->getContentSize().width - _menuStartJoint->getContentSize().width / 2,
		origin.y + visibleSize.height - _menuStartJoint->getContentSize().height / 2));
	_menuStartJoint->setVisible(true);
	
	_menuStopJoint = MenuItemImage::create(
		RES_IMAGE(StopJointMode.png),
		RES_IMAGE(StopJointModeHover.png),
		CC_CALLBACK_1(ToolLayer::stopJointModeCallback, this));
	_menuStopJoint->setScale(0.5f);
	_menuStopJoint->setPosition(Vec2(origin.x + visibleSize.width - _menuStartSimulate->getContentSize().width - _menuStopJoint->getContentSize().width / 2,
		origin.y + visibleSize.height - _menuStopJoint->getContentSize().height / 2));
	_menuStopJoint->setVisible(false);*/
	
	_menuStartDebug = MenuItemImage::create(
		RES_IMAGE(DebugMode.png),
		RES_IMAGE(DebugMode.png));
	_menuStartDebug->setScale(0.5f);
	_menuStartDebug->setPosition(Vec2(origin.x + visibleSize.width - _menuStartDebug->getContentSize().width / 2,
		origin.y + visibleSize.height - _menuStartDebug->getContentSize().height / 2));
	_menuStartDebug->setVisible(false);

	// create menu, it's an autorelease object
	auto menu = Menu::create(_menuStartSimulate, _menuStopSimulate, _menuStartDebug, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void ToolLayer::onRecognizeSuccess(EventCustom* event)
{
	RecognizedSprite* result = static_cast<RecognizedSprite*>(event->getUserData());
	char buf[100];
	GetToolHintRecognizeSucc(buf, result->getGeometricType().c_str(), result->_result.score);
	this->_labelHint->setString(buf);
}

void ToolLayer::onRecognizeFailed(EventCustom* event)
{
	RecognizedSprite* result = static_cast<RecognizedSprite*>(event->getUserData());
	char buf[100];
	GetToolHintRecognizeFailed(buf, result->getGeometricType().c_str(), result->_result.score);
	this->_labelHint->setString(buf);
}

void ToolLayer::onEnter()
{
	Layer::onEnter();

	_recognizeSuccessListener = EventListenerCustom::create(EVENT_RECOGNIZE_SUCCESS, CC_CALLBACK_1(ToolLayer::onRecognizeSuccess, this));
	_recognizeFailedListener = EventListenerCustom::create(EVENT_RECOGNIZE_FAILED, CC_CALLBACK_1(ToolLayer::onRecognizeFailed, this));

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_recognizeSuccessListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_recognizeFailedListener, this);

	auto _loadTemplateListener = EventListenerCustom::create(EVENT_LOADING_TEMPLATE, [&](EventCustom* event){
		LoadTemplateData* ltd = static_cast<LoadTemplateData*>(event->getUserData());
		this->_labelHint->setString(ltd->_hint.c_str());
	});
	auto _loadedTemplateListener = EventListenerCustom::create(EVENT_LOADED_TEMPLATE, [&](EventCustom* event){
		LoadTemplateData* ltd = static_cast<LoadTemplateData*>(event->getUserData());
		this->_labelHint->setString(ltd->_hint.c_str());
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_loadTemplateListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_loadedTemplateListener, this);
}

void ToolLayer::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	Layer::onExit();
}

void ToolLayer::startSimulateCallback(cocos2d::Ref* pSender)
{
	_menuStartSimulate->setVisible(false);
	_menuStopSimulate->setVisible(true);
	_canvasScene->startSimulate();
}


void ToolLayer::stopSimulateCallback(cocos2d::Ref* pSender)
{
	_menuStartSimulate->setVisible(true);
	_menuStopSimulate->setVisible(false);
	_canvasScene->stopSimulate();
}

void ToolLayer::startJointModeCallback(cocos2d::Ref* pSender)
{
	_menuStartJoint->setVisible(false);
	_menuStopJoint->setVisible(true);
	/*_canvasScene->stopSimulate();*/
}

void ToolLayer::stopJointModeCallback(cocos2d::Ref* pSender)
{
	_menuStartJoint->setVisible(true);
	_menuStopJoint->setVisible(false);
	//_canvasScene->stopSimulate();
}

void ToolLayer::toggleDebugMode(bool isDebug)
{
	_menuStartDebug->setVisible(isDebug);
	//_canvasScene->stopSimulate();
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	log("game layer init");
	_postCmdHandlers.init();
	
	//GenSpriteResultMap& genSpriteMap = _genSpriteResultMap;
	// iterate to retrieve recognized sprite
	InitGeometricPhysicsMask();


	priority_queue<RecognizedSprite*, vector<RecognizedSprite*>, RecognizedSpriteLessCmp> lazyQueue;
	for (auto p = _drawNodeResultMap.begin(); p != _drawNodeResultMap.end(); p++)
	{
		auto ds = p->first;
		auto rs = &(p->second);
		if (!ds->empty() && rs->_priority >= 0){ lazyQueue.push(rs); }
	}

	while (!lazyQueue.empty())
	{
		auto rs = lazyQueue.top();
		lazyQueue.pop();
		auto cmdh = _postCmdHandlers.getCommandHandler(rs->getGeometricType());
		if (!cmdh._Empty()) cmdh(*rs, _drawNodeList, this, &_genSpriteResultMap);
	}

	return true;
}

GameLayer *GameLayer::create(list<DrawableSprite*>& drawNodeList, DrawSpriteResultMap& drawNodeResultMap, Scene* scene)
{
	GameLayer *ret = new (std::nothrow) GameLayer(drawNodeList, drawNodeResultMap);
	ret->setParent(scene);
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

GameLayer::GameLayer(list<DrawableSprite*>& drawNodeList, DrawSpriteResultMap& drawNodeResultMap)
:_drawNodeList(drawNodeList)
, _drawNodeResultMap(drawNodeResultMap)
{}

void GameLayer::onEnter()
{
	Layer::onEnter();
	log("game layer on enter");

	_postCmdHandlers.makeJoints(this->getScene()->getPhysicsWorld(), jointsList, _genSpriteResultMap);
}

void GameLayer::onExit()
{
	this->stopAllActions();
	_eventDispatcher->removeEventListenersForTarget(this);
	Layer::onExit();
}
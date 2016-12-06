#include "CommandHandler.h"
#include "gesture/GeometricRecognizer.h"
#include "geometry/GeomtryType.h"
#include "geometry/GeometricPhysics.h"
#include "geometry/Weapon.h"
#include "geometry/GeometricMath.h"
#include "controller/PawnController.h"

USING_NS_CC;
using namespace DollarRecognizer;

void handleDefaultWithPhysics(
	RecognizedSprite& recSprite,
	list<DrawableSprite*>& drawNodeList,
	Node* owner,
	void* udata,
	PhysicsBody*(*pfMakePhysicsBody)(DrawableSprite*));

void CommandHandlerFactory::registerCommandHandler(const string& name, CommandHandler handler)
{
	auto p = _handlerMap.find(name);
	CCASSERT(p == _handlerMap.end(), "Argument must be non-nil");
	_handlerMap.insert(pair<string, CommandHandler>(name, handler));
}

CommandHandler CommandHandlerFactory::getCommandHandler(const string& name)
{
	auto p = _handlerMap.find(name);
	if (p != _handlerMap.end())return p->second;
	else return nullptr;
}

// new callbacks based on C++11
#define CC_CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)

bool PreCommandHandlerFactory::init()
{
	// circle
	this->registerCommandHandler(GT_MESH_CIRCLE, CC_CALLBACK_4(PreCommandHandlerFactory::handleDefault, this));

	// triangle
	this->registerCommandHandler(GT_MESH_TRIANGLE, CC_CALLBACK_4(PreCommandHandlerFactory::handleDefault, this));

	// rectangle
	this->registerCommandHandler(GT_MESH_RECTANGLE, CC_CALLBACK_4(PreCommandHandlerFactory::handleDefault, this));

	// pawn
	this->registerCommandHandler(GT_MESH_PAWN, CC_CALLBACK_4(PreCommandHandlerFactory::handleDefault, this));

	// groove
	this->registerCommandHandler(GT_MESH_GROOVE, CC_CALLBACK_4(PreCommandHandlerFactory::handleDefault, this));

	// spring, @see CommandHandler
	this->registerCommandHandler(GT_MESH_SPRING, [](
		RecognizedSprite& recSprite, 
		list<DrawableSprite*>& drawNodeList, 
		Node* owner, 
		void* udata)
	{
		DrawSpriteResultMap* rmap = static_cast<DrawSpriteResultMap*>(udata);
		if (rmap)
		{
			// defualt priority is 5
			// set a lower priority so that Spring can attach to other
			// existing object drawn after it
			recSprite._priority = 4;
			rmap->insert(pair<DrawableSprite*, RecognizedSprite>(recSprite._drawNode, recSprite));
		}
	});

	// pistal, @see CommandHandler
	this->registerCommandHandler(GT_MESH_PISTAL, [](
		RecognizedSprite& recSprite, 
		list<DrawableSprite*>& drawNodeList,
		Node* owner, 
		void* udata)
	{
		DrawSpriteResultMap* rmap = static_cast<DrawSpriteResultMap*>(udata);
		if (rmap)
		{
			// defualt priority is 5
			// set a lower priority so that Pistal can attach to other 
			// existing object drawn after it, such as a gunman
			recSprite._priority = 4;
			rmap->insert(pair<DrawableSprite*, RecognizedSprite>(recSprite._drawNode, recSprite));
		}
	});

	// static, @see CommandHandler
	this->registerCommandHandler(GT_MARK_STATIC, [](
		RecognizedSprite& recSprite,
		list<DrawableSprite*>& drawNodeList,
		Node* owner,
		void* udata)
	{
		DrawSpriteResultMap* rmap = static_cast<DrawSpriteResultMap*>(udata);
		if (rmap)
		{
			recSprite._priority = 1;
			rmap->insert(pair<DrawableSprite*, RecognizedSprite>(recSprite._drawNode, recSprite));
		}
	});

	// control/check, @see CommandHandler
	this->registerCommandHandler(GT_MARK_CONCTROL, [](
		RecognizedSprite& recSprite,
		list<DrawableSprite*>& drawNodeList,
		Node* owner,
		void* udata)
	{
		DrawSpriteResultMap* rmap = static_cast<DrawSpriteResultMap*>(udata);
		if (rmap)
		{
			// iterate result map to find other control symbol
			// just remove it if find, transfer control to current sprite
			auto p = rmap->begin();
			while (p != rmap->end())
			{
				auto& rs = p->second;
				if ((recSprite._drawNode != rs._drawNode) && rs.isType(GT_MARK_CONCTROL))
				{
					owner->removeChild(rs._drawNode);
					drawNodeList.remove(rs._drawNode);
					auto to_erase = p;
					++p;
					rmap->erase(to_erase);
				}
				++p;
			}
			
			// lower priority to make sure other sprite can be generated before this
			recSprite._priority = 1;
			rmap->insert(pair<DrawableSprite*, RecognizedSprite>(recSprite._drawNode, recSprite));
		}
	});

	// delete, @see CommandHandler
	this->registerCommandHandler(GT_MARK_DELETE, [](
		RecognizedSprite& recSprite,
		list<DrawableSprite*>& drawNodeList,
		Node* owner,
		void* udata)
	{
		RecognitionResult& result = recSprite._result;
		DrawableSprite* currentDrawNode = recSprite._drawNode;
		Vec2 position = currentDrawNode->getShapeCenter();

		// iterate all sprites 
		// remove other sprite when this delete symbol IsSnappedTo other sprite
		for (auto i = drawNodeList.begin(); i != drawNodeList.end(); i++)
		{
			if (*i != currentDrawNode)
			{
				Vec2 candinate = (*i)->getShapeCenter();
				if (IsSnapedTo(currentDrawNode, *i))
				{
					DrawSpriteResultMap* rmap = static_cast<DrawSpriteResultMap*>(udata);
					if (rmap && rmap->end() != rmap->find(*i))rmap->erase(*i);
					owner->removeChild(*i);
					i = drawNodeList.erase(i);
				}
			}
		}
		// remove itself
		drawNodeList.remove(currentDrawNode);
		owner->removeChild(currentDrawNode);
	});

	return true;
}

void PreCommandHandlerFactory::handleDefault(
	RecognizedSprite& recSprite, 
	std::list<DrawableSprite*>& drawNodeList, 
	cocos2d::Node* owner,
	void* udata)
{
	// just add to result map, with defualt priority
	DrawSpriteResultMap* rmap = static_cast<DrawSpriteResultMap*>(udata);
	if (rmap)
	{
		rmap->insert(pair<DrawableSprite*, RecognizedSprite>(recSprite._drawNode, recSprite));
	}
}

bool PostCommandHandlerFactory::init()
{
	// circle, @see makePhysicsBodyAsBall
	this->registerCommandHandler(GT_MESH_CIRCLE, [](RecognizedSprite& recSprite,
		list<DrawableSprite*>& drawNodeList,
		Node* owner,
		void* udata)
	{
		handleDefaultWithPhysics(recSprite, drawNodeList, owner, udata, makePhysicsBodyAsBall);
	});

	// triangle, @see PostCommandHandlerFactory::handleDefault
	this->registerCommandHandler(GT_MESH_TRIANGLE, CC_CALLBACK_4(PostCommandHandlerFactory::handleDefault, this));

	// rectangle, @see PostCommandHandlerFactory::handleDefault
	this->registerCommandHandler(GT_MESH_RECTANGLE, CC_CALLBACK_4(PostCommandHandlerFactory::handleDefault, this));

	// pawn, @see makePhysicsBodyAsBox
	this->registerCommandHandler(GT_MESH_PAWN, [](
		RecognizedSprite& recSprite,
		list<DrawableSprite*>& drawNodeList,
		Node* owner,
		void* udata)
	{
		handleDefaultWithPhysics(recSprite, drawNodeList, owner, udata, makePhysicsBodyAsBox);
	});

	// groove, @see makePhysicsBodyAsGroove
	this->registerCommandHandler(GT_MESH_GROOVE, [](
		RecognizedSprite& recSprite,
		list<DrawableSprite*>& drawNodeList,
		Node* owner,
		void* udata)
	{
		handleDefaultWithPhysics(recSprite, drawNodeList, owner, udata, makePhysicsBodyAsGroove);
	});

	// spring, @see CommandHandler
	this->registerCommandHandler(GT_MESH_SPRING, [](
		RecognizedSprite& recSprite,
		list<DrawableSprite*>& drawNodeList,
		Node* owner,
		void* udata)
	{
		RecognitionResult& result = recSprite._result;
		DrawableSprite* ds = recSprite._drawNode;
		Vec2 position = ds->getShapeCenter();
		auto sprite = Sprite::createWithTexture(ds->createTexture(), ds->contentRect());
		sprite->setPosition(position);
		sprite->setFlippedY(true);

		// attach sprite to it's parent sprite/scene
		owner->addChild(sprite);

		GenSpriteResultMap* rmap = static_cast<GenSpriteResultMap*>(udata);
		if (rmap)
		{
			vector<Sprite*> targets(2);
			vector<Vec2> anchors(2);
			int cnt = 0;
			auto path = ds->getPath();
			auto first = path.front();
			auto end = path.back();

			for (auto p = rmap->begin(); p != rmap->end(); p++)
			{
				auto cur = p->second;
				// calculate current rectangle area
				auto curRect = Rect(
					Vec2(cur->getPositionX(), cur->getPositionY()) - cur->getContentSize() / 2,		// left-down location
					cur->getContentSize()
					);
				
				// if current rectangle area contains one of the spring side points.
				if (curRect.containsPoint(first))
				{ 
					targets[0] = cur;
					anchors[0] = (first - cur->getPosition());
					cnt++;
				}
				else if (curRect.containsPoint(end))
				{
					targets[1] = cur;
					anchors[1] = (end - cur->getPosition());
					cnt++;
				}
			}

			// spring can have only 2 joints
			if (cnt == 2)
			{
				// spring joint
				auto joint = PhysicsJointSpring::construct(
					targets[0]->getPhysicsBody(),
					targets[1]->getPhysicsBody(),
					anchors[0],
					anchors[1],
					30000.0f, 0.5f);
				joint->createConstraints();
				auto physicsWorld = owner->getScene()->getPhysicsWorld();
				physicsWorld->addJoint(joint);

				// schedule key for different spring instance
				static int springCnt = 0;
				char scheduleKey[20];
				sprintf(scheduleKey, "spring_%d", springCnt);
				springCnt++;

				// distance transformation
				auto originalDistance = EuclideanDistance(
					targets[0]->getPosition() + anchors[0],
					targets[1]->getPosition() + anchors[1]
					);

				// schedule by owner, just like tick update
				owner->schedule([sprite, targets, anchors, originalDistance](float delta)
				{
					// calculate rotate angle by joints location
					auto posA = ccpRotateByAngle(targets[0]->getPosition() + anchors[0], 
						targets[0]->getPosition(), CC_DEGREES_TO_RADIANS(-targets[0]->getRotation()));
					auto posB = ccpRotateByAngle(targets[1]->getPosition() + anchors[1],
						targets[1]->getPosition(), CC_DEGREES_TO_RADIANS(-targets[1]->getRotation()));
					// set location to the center of 2 joints
					sprite->setPosition((posA + posB) / 2);
					auto strentchDistance = EuclideanDistance(posA, posB);
					auto currentDirection = (posB - posA);
					auto originalDirection = Vec2(0.0f, -1.0f);
					// rotate spring
					float rotator = CC_RADIANS_TO_DEGREES(ccpAngle(currentDirection, originalDirection));
					sprite->setRotation(rotator);
					// strentch spring length
					sprite->setScaleY(strentchDistance / originalDistance);
				}, scheduleKey);
			}
		}
	});

	// pistal, @see CommandHandler
	this->registerCommandHandler(GT_MESH_PISTAL, [](
		RecognizedSprite& recSprite,
		list<DrawableSprite*>& drawNodeList,
		Node* owner,
		void* udata)
	{
		GenSpriteResultMap* rmap = static_cast<GenSpriteResultMap*>(udata);
		RecognitionResult& result = recSprite._result;
		DrawableSprite* ds = recSprite._drawNode;
		Vec2 position = ds->getShapeCenter();
		auto pistal = Pistal::createWithTexture(ds->createTexture(), ds->contentRect());
		pistal->setFlippedY(true);
		// check if there is gunman to be attached
		bool findGunman = false;
		if (rmap)
		{
			Vec2 my = Vec2(ds->contentRect().getMinX(), (ds->contentRect().getMinY() + ds->contentRect().getMaxY()) / 2);
			for (auto p = rmap->begin(); p != rmap->end(); p++)
			{
				Vec2 cur = (p->second)->getPosition() + Vec2(((p->second)->getContentSize().width / 2), 0);
				if (EuclideanDistance(cur, my) < 100.0f)
				{
					auto gunman = (p->second);
					gunman->addChild(pistal);
					gunman->addChild(pistal->getBulletLayer());
					pistal->setGunman(gunman);

					// calculate relative offset
					Vec2 gmLoc = gunman->getPosition();
					Size c = gunman->getContentSize();
					Vec2 a = gunman->getAnchorPoint();
					Vec2 offset = Vec2(c.width*a.x, c.height*a.y);
					pistal->setPosition(position - gmLoc + offset);

					findGunman = true;
					break;
				}
			}
		}
		// if not fount player
		if (!findGunman)
		{
			pistal->setPosition(position);
			pistal->addComponent(makePhysicsBodyAsBox(ds));
			owner->addChild(pistal);
			owner->addChild(pistal->getBulletLayer());
		}

		if (rmap)rmap->insert(pair<DrawableSprite*, Sprite*>(ds, pistal));
	});

	// static, @see CommandHandler
	this->registerCommandHandler(GT_MARK_STATIC, [](
		RecognizedSprite& recSprite,
		list<DrawableSprite*>& drawNodeList,
		Node* owner, 
		void* udata)
	{
		GenSpriteResultMap* rmap = static_cast<GenSpriteResultMap*>(udata);
		if (rmap)
		{
			RecognitionResult& result = recSprite._result;
			DrawableSprite* currentDrawNode = recSprite._drawNode;
			Vec2 position = currentDrawNode->getShapeCenter();
			for (auto i = drawNodeList.begin(); i != drawNodeList.end(); i++)
			{
				// is not static mark
				if (*i != currentDrawNode)
				{
					Vec2 candinate = (*i)->getShapeCenter();
					// if static mark is snaped to a node
					if (IsSnapedTo(currentDrawNode, *i))
					{

						auto p = rmap->find(*i);
						if (p != rmap->end())
						{
							// set node as static/non-dynamic
							auto s = (p->second);
							auto pb = s->getPhysicsBody();
							pb->setDynamic(false);
						}
						break;
					}
				}
			}
		}
	});

	// control
	this->registerCommandHandler(GT_MARK_CONCTROL, [](
		RecognizedSprite& recSprite,
		list<DrawableSprite*>& drawNodeList,
		Node* owner,
		void* udata)
	{
		GenSpriteResultMap* rmap = static_cast<GenSpriteResultMap*>(udata);
		if (rmap)
		{
			RecognitionResult& result = recSprite._result;
			DrawableSprite* currentDrawNode = recSprite._drawNode;
			Vec2 position = currentDrawNode->getShapeCenter();
			for (auto i = drawNodeList.begin(); i != drawNodeList.end(); i++)
			{
				// is not control mark
				if (*i != currentDrawNode)
				{
					Vec2 candinate = (*i)->getShapeCenter();
					// if control mark is snaped to a node, with epsilon 0.5
					if (IsSnapedTo(currentDrawNode, *i, 0.5f))
					{

						auto p = rmap->find(*i);
						if (p != rmap->end())
						{
							// attach a pawn controll to the node
							auto s = (p->second);
							s->addComponent(PawnController::create());
						}
						break;
					}
				}
			}
		}
	});
	return true;
}

void handleDefaultWithPhysics(
	RecognizedSprite& recSprite,
	list<DrawableSprite*>& drawNodeList,
	Node* owner,
	void* udata, 
	PhysicsBody*(*pfMakePhysicsBody)(DrawableSprite*))
{
	RecognitionResult& result = recSprite._result;
	DrawableSprite* ds = recSprite._drawNode;

	// create sprite with drawn texture(clipped by content rectangle) 
	auto sprite = Sprite::createWithTexture(ds->createTexture(), ds->contentRect());
	// set shape center as current postion
	Vec2 position = ds->getShapeCenter();
	sprite->setPosition(position);
	// flip Y
	sprite->setFlippedY(true);

	// if the pointer to function pfMakePhysicsBody is set,
	// add a physics body created by pfMakePhysicsBody to sprite
	if (pfMakePhysicsBody) sprite->addComponent(pfMakePhysicsBody(ds));

	// attach to owner
	owner->addChild(sprite);

	// add to generated sprite result map
	GenSpriteResultMap* rmap = static_cast<GenSpriteResultMap*>(udata);
	if (rmap)rmap->insert(pair<DrawableSprite*, Sprite*>(recSprite._drawNode, sprite));
}

void PostCommandHandlerFactory::handleDefault(
	RecognizedSprite& recSprite, 
	list<DrawableSprite*>& drawNodeList, 
	Node* owner, 
	void* udata)
{
	// default use makePhysicsBodyAsPolygon to generate physics body
	handleDefaultWithPhysics(recSprite, drawNodeList, owner, udata, makePhysicsBodyAsPolygon);
}

void PostCommandHandlerFactory::makeJoints(
	PhysicsWorld* const physicsWorld,
	JointsList& jointsList,
	GenSpriteResultMap& resultMap)
{
	for (auto pjoints = jointsList.begin(); pjoints != jointsList.end(); pjoints++)
	{
		// check is empty
		if (pjoints->empty())continue;

		PhysicsBody* body[2];

		// for current joints count
		// if c>=2 there is a pair of joints
		int c = 0;

		// iterate to find joints, make joints by one to one pair
		for (auto i = pjoints->begin(); i != pjoints->end(); i++)
		{
			auto p = resultMap.find(*i);
			if (p != resultMap.end())
			{
				auto sprite = p->second;
				auto cur = sprite->getPhysicsBody();
				body[c] = cur;
				if (c)
				{
					// if all joints is valid
					if (body[0] != nullptr
						&&body[1] != nullptr
						&&body[0] != body[1])
					{
						PhysicsJointDistance* joint = PhysicsJointDistance::construct(body[0], body[1], Point::ZERO, Point::ZERO);
						physicsWorld->addJoint(joint);
					}
					else
					{
						cocos2d::log("ERROR: Body[0]=%d, Body[1]=%d", body[0], body[1]);
					}
					// mark body[1] as first joint, wait for next joint
					body[0] = body[1];
					c = 0;
				}
				c++;
			}
		}
	}
}
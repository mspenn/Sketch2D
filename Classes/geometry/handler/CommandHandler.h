#ifndef __COMMAND_HANDLER_H__
#define __COMMAND_HANDLER_H__

#include "cocos2d.h"
#include "geometry/recognizer/RecognizedSprite.h"
#include "geometry/GeometricPhysics.h"

class CommandHandlerFactory;
class PreCommandHandlerFactory;
class PostCommandHandlerFactory;

typedef std::map<DrawableSprite*, RecognizedSprite> DrawSpriteResultMap;
typedef std::map<DrawableSprite*, cocos2d::Sprite*> GenSpriteResultMap;
typedef std::function<void(RecognizedSprite&, std::list<DrawableSprite*>&, cocos2d::Node*, void*)> CommandHandler;

class CommandHandlerFactory
{
public:
	virtual bool init() = 0;
	virtual void registerCommandHandler(const std::string& name, CommandHandler handler);
	virtual CommandHandler getCommandHandler(const std::string& name);
	
private:
	std::map<std::string, CommandHandler> _handlerMap;
};

class PreCommandHandlerFactory :public CommandHandlerFactory
{
public:
	virtual bool init();
	void handleDefault(RecognizedSprite& recSprite, std::list<DrawableSprite*>& drawNodeList, cocos2d::Node* owner, void* udata);
};

class PostCommandHandlerFactory :public CommandHandlerFactory
{
public:
	virtual bool init();
	void handleDefault(RecognizedSprite& recSprite, std::list<DrawableSprite*>& drawNodeList, cocos2d::Node* owner, void* udata);
	void makeJoints(cocos2d::PhysicsWorld * const physicsWorld, JointsList& jointsList, GenSpriteResultMap& resultMap);
};

/*
class CommandExecutor
{
public:
	void exec(const RecognizedSprite& rs, std::list<DrawableSprite*>&, cocos2d::Node*)
	{
		CommandHandler cmdh = _cmdHandlerFactory.getCommandHandler(rs.getGeometricType());
		if (!cmdh._Empty())cmdh(rs, _drawNodeList, this);
	}
private:

};
*/
#endif

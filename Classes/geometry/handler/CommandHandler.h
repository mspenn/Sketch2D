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

/**
 * Command handler
 * @param recSprite		recognized sprite associate with user drawed shapes
 * @see RecognizedSprite
 * @param drawNodeList	list of DrawableSprite*, reference to user drawed shapes in screen
 * @see DrawableSprite
 * @param owner			recognized sprite owner
 * @param udata			a pointer to user - defined data
 */
typedef std::function<void(RecognizedSprite&, std::list<DrawableSprite*>&, cocos2d::Node*, void*)> CommandHandler;

/**
 * Command Handler Factory
 * Factory to create Command Handler
 */
class CommandHandlerFactory
{
public:

	/**
	 * Initialize default/embbeded handlers
	 * pure virtual function
	 * return true if init called successfully, false otherwise
	 */
	virtual bool init() = 0;

	/**
	 * Register Command Handler to factory with associated name
	 * @param name	handler name to be registered, it's unique in this factory
	 */
	virtual void registerCommandHandler(const std::string& name, CommandHandler handler);

	/**
	 * Get Command Handler instance by handler associated name
	 * @param name	handler name registered to factory
	 * @retun CommandHandler instance
	 */
	virtual CommandHandler getCommandHandler(const std::string& name);
	
private:
	std::map<std::string, CommandHandler> _handlerMap;	// name-handler map
};

/**
 * Pre Command Handler Factory
 * Use this to process drawable recognized sprite
 */
class PreCommandHandlerFactory :public CommandHandlerFactory
{
public:

	/**
	 * Initialize default/embbeded pre-handlers, override
	 * @see CommandHandlerFactory::init
	 */
	virtual bool init();

	/**
	 * Default pre-handler
	 * @param recSprite		recognized sprite associate with user drawed shapes
	 * @see RecognizedSprite
	 * @param drawNodeList	list of DrawableSprite*, reference to user drawed shapes in screen 
	 * @see DrawableSprite
	 * @param owner			recognized sprite owner
	 * @param udata			a pointer to user-defined data
	 */
	void handleDefault(RecognizedSprite& recSprite, std::list<DrawableSprite*>& drawNodeList, cocos2d::Node* owner, void* udata);
};

/**
 * Post Command Handler Factory
 * Factory to create Command Handler
 */
class PostCommandHandlerFactory :public CommandHandlerFactory
{
public:
	
	/**
	 * Initialize default/embbeded post-handlers, override
	 * @see CommandHandlerFactory::init
	 */
	virtual bool init();

	/**
	 * Default post-handler
	 * @see CommandHandler
	 * @param recSprite		recognized sprite associate with user drawed shapes
	 * @see RecognizedSprite
	 * @param drawNodeList	list of DrawableSprite*, reference to user drawed shapes in screen
	 * @see DrawableSprite
	 * @param owner			recognized sprite owner
	 * @param udata			a pointer to user-defined data
	 */
	void handleDefault(RecognizedSprite& recSprite, std::list<DrawableSprite*>& drawNodeList, cocos2d::Node* owner, void* udata);

	/**
	 * Make physics joints
	 * @param physicsWorld	physics world in scene
	 * @param jointsList	physics joints list
	 * @param resultMap		store result as DrawableSprite*-cocos2d::Sprite* map
	 * @see DrawableSprite
	 * @see cocos2d::Sprite
	 */
	void makeJoints(cocos2d::PhysicsWorld * const physicsWorld, JointsList& jointsList, GenSpriteResultMap& resultMap);
};

#endif	/* __COMMAND_HANDLER_H__ */

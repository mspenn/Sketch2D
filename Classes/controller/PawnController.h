#ifndef __PAWN_CONTROLLER_H__
#define __PAWN_CONTROLLER_H__

#include "cocos2d.h"
#include "controller/CharacterController.h"

/**
 * Pawn Controller
 * Responsed to user keyboard input event, W, A, S, D, SPACE
 * Responsed to user mouse input event
 * It can be attached to cocos2d::Nodes
 * @see CharacterController
 */
class PawnController :public CharacterController
{
public:

	// Constructor
	PawnController();

	/**
	 * Call when initialized, override
	 * @return true if initialized successfully, false otherwise.
	 * @see CharacterController::init
	 */
	virtual bool init();

	/**
	 * Call when PawnController enter the "stage", override
	 * @see CharacterController::onEnter
	 */
	virtual void onEnter();

	/**
	 * Call every game tick, override
	 * @see cocos2d::Component::update
	 */
	void update(float delta) override;
	
	/**
	 * Set pawn moving velocity
	 * @param velocity	new velocity
	 */
	void setVelocity(cocos2d::Vec2 velocity)
	{
		this->_velocity = velocity;
	}
	
	/**
	 * Get pawn moving velocity
	 */
	cocos2d::Vec2 getVelocity() const
	{
		return this->_velocity;
	}
	
	/**
	 * Set if pawn is moving now
	 * @param isMoving	is pawn moving now?
	 */
	void setIsMoving(bool isMoving)
	{
		this->_isMoveing = isMoving;
	}
	
	/**
	 * Set if pawn is moving now
	 * @param isMoving	is pawn jumping now?
	 */
	void setIsJumping(bool isJumping)
	{
		this->_isJumping = isJumping;
	}
	
	/**
	 * Get if pawn is moving now
	 * @return true if pawn is moving this time, false otherwise
	 */
	bool isMoving()const
	{
		return this->_isMoveing;
	}

	/**
	 * Get if pawn is jumping now
	 * @return true if pawn is jumping this time, false otherwise
	 */
	bool isJumping()const{ return this->_isJumping; }

	/**
	 * Static factor for create CharacterController instance, with auto released manangement
	 */
	CREATE_FUNC(PawnController);
protected:
	
	cocos2d::Vec2 _velocity;	// current moving velocity
	bool _isMoveing;			// is pawn moving now
	bool _isJumping;			// is pawn jumping now
};

#endif	/* __PAWN_CONTROLLER_H__ */
#ifndef __PAWN_CONTROLLER_H__
#define __PAWN_CONTROLLER_H__

#include "cocos2d.h"
#include "controller/CharacterController.h"

class PawnController :public CharacterController
{
public:
	PawnController();
	virtual bool init();
	virtual void onEnter();
	void update(float delta) override;
	void setVelocity(cocos2d::Vec2 velocity){ this->_velocity = velocity; }
	cocos2d::Vec2 getVelocity() const { return this->_velocity; }
	void setIsMoving(bool isMoving){ this->_isMoveing = isMoving; }
	bool isMoving()const{ return this->_isMoveing; }
	CREATE_FUNC(PawnController);
protected:
	cocos2d::Vec2 _velocity;
	bool _isMoveing;
};

#endif
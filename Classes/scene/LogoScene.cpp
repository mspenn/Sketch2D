#include "LogoScene.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

USING_NS_CC;

#define PARTICLE_SYSTEM_TAG 0x0001

bool LogoScene::init()
{
	if (!Scene::init())
		return false;

	auto rootps = PUParticleSystem3D::create("res/blackHole.pu", "res/pu_mediapack_01.material");
	rootps->setCameraMask((unsigned short)CameraFlag::USER1);
	rootps->setPosition(-25.0f, 0.0f);
	auto moveby = MoveBy::create(2.0f, Vec2(50.0f, 0.0f));
	auto moveby1 = MoveBy::create(2.0f, Vec2(-50.0f, 0.0f));
	//    auto scale = ScaleBy::create(1.0f, 2.0f, 2.0f, 2.0f);
	//    auto rotate = RotateBy::create(1.0f, Vec3(100.0f, 100.0f, 100.0f));
	rootps->runAction(RepeatForever::create(Sequence::create(moveby, moveby1, nullptr)));
	//rootps->runAction(RepeatForever::create(Sequence::create(scale, scale->reverse(), nullptr)));
	//rootps->runAction(RepeatForever::create(Sequence::create(rotate, nullptr)));
	rootps->startParticleSystem();

	this->addChild(rootps, 0, PARTICLE_SYSTEM_TAG);

	return true;
}
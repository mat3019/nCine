#include "LuaAnimatedSprite.h"
#include "LuaClassWrapper.h"
#include "LuaClassTracker.h"
#include "LuaSprite.h"
#include "LuaRectAnimation.h"
#include "LuaUtils.h"
#include "AnimatedSprite.h"

namespace ncine {

namespace LuaNames {
namespace AnimatedSprite
{
	static const char *AnimatedSprite = "animated_sprite";

	static const char *addAnimation = "add_animation";
	static const char *clearAnimations = "clear_animations";

	static const char *isPaused = "is_paused";
	static const char *setPaused = "set_paused";

	static const char *setAnimation = "set_animation";
	static const char *setFrame = "set_frame";
}}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

void LuaAnimatedSprite::expose(LuaStateManager *stateManager)
{
	lua_State *L = stateManager->state();
	lua_newtable(L);

	if (stateManager->apiType() == LuaStateManager::ApiType::FULL)
	{
		LuaClassTracker<AnimatedSprite>::exposeDelete(L);
		LuaUtils::addFunction(L, LuaNames::newObject, newObject);
	}

	LuaUtils::addFunction(L, LuaNames::AnimatedSprite::addAnimation, addAnimation);
	LuaUtils::addFunction(L, LuaNames::AnimatedSprite::clearAnimations, clearAnimations);

	LuaUtils::addFunction(L, LuaNames::AnimatedSprite::isPaused, isPaused);
	LuaUtils::addFunction(L, LuaNames::AnimatedSprite::setPaused, setPaused);

	LuaUtils::addFunction(L, LuaNames::AnimatedSprite::setAnimation, setAnimation);
	LuaUtils::addFunction(L, LuaNames::AnimatedSprite::setFrame, setFrame);

	LuaSprite::exposeFunctions(L);

	lua_setfield(L, -2, LuaNames::AnimatedSprite::AnimatedSprite);
}

void LuaAnimatedSprite::release(void *object)
{
	AnimatedSprite *sprite = reinterpret_cast<AnimatedSprite *>(object);
	delete sprite;
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

int LuaAnimatedSprite::newObject(lua_State *L)
{
	SceneNode *parent = LuaClassWrapper<SceneNode>::unwrapUserDataOrNil(L, -4);
	Texture *texture = LuaClassWrapper<Texture>::unwrapUserData(L, -3);
	const float x = LuaUtils::retrieve<float>(L, -2);
	const float y = LuaUtils::retrieve<float>(L, -1);

	AnimatedSprite *sprite = new AnimatedSprite(parent, texture, x, y);
	LuaClassTracker<AnimatedSprite>::wrapTrackedUserData(L, sprite);

	return 1;
}

int LuaAnimatedSprite::isPaused(lua_State *L)
{
	AnimatedSprite *sprite = LuaClassWrapper<AnimatedSprite>::unwrapUserData(L, -1);
	LuaUtils::push(L, sprite->isPaused());
	return 1;
}

int LuaAnimatedSprite::setPaused(lua_State *L)
{
	AnimatedSprite *sprite = LuaClassWrapper<AnimatedSprite>::unwrapUserData(L, -2);
	const bool paused = LuaUtils::retrieve<bool>(L, -1);

	sprite->setPaused(paused);

	return 0;
}

int LuaAnimatedSprite::addAnimation(lua_State *L)
{
	if (lua_istable(L, -1) == false)
		luaL_argerror(L, -1, "Expecting a table");

	AnimatedSprite *sprite = LuaClassWrapper<AnimatedSprite>::unwrapUserData(L, -2);
	nctl::UniquePtr<RectAnimation> anim = LuaRectAnimation::retrieveTable(L, -1);

	sprite->addAnimation(nctl::move(anim));

	return 0;
}

int LuaAnimatedSprite::clearAnimations(lua_State *L)
{
	AnimatedSprite *sprite = LuaClassWrapper<AnimatedSprite>::unwrapUserData(L, -1);
	sprite->clearAnimations();
	return 0;
}

int LuaAnimatedSprite::setAnimation(lua_State *L)
{
	AnimatedSprite *sprite = LuaClassWrapper<AnimatedSprite>::unwrapUserData(L, -2);
	const int animNum = LuaUtils::retrieve<int32_t>(L, -1);

	sprite->setAnimation(animNum);

	return 0;
}

int LuaAnimatedSprite::setFrame(lua_State *L)
{
	AnimatedSprite *sprite = LuaClassWrapper<AnimatedSprite>::unwrapUserData(L, -2);
	const unsigned int frameNum = LuaUtils::retrieve<uint32_t>(L, -1);

	sprite->setFrame(frameNum);

	return 0;
}

}

#include "TES3ActionDataLua.h"

#include "sol.hpp"
#include "LuaUtil.h"
#include "LuaManager.h"

#include "TES3ActionData.h"
#include "TES3MobileActor.h"
#include "TES3Item.h"

namespace mwse {
	namespace lua {
		void bindTES3ActionData() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::ActionData>("tes3actionData");
			usertypeDefinition.set("new", sol::no_constructor);

			// Basic property binding.
			usertypeDefinition.set("aiBehaviorState", &TES3::ActionData::aiBehaviourState);
			usertypeDefinition.set("animationAttackState", &TES3::ActionData::animStateAttack);
			usertypeDefinition.set("attackDirection", &TES3::ActionData::attackDirection);
			usertypeDefinition.set("attackSwing", &TES3::ActionData::attackSwing);
			usertypeDefinition.set("currentAnimationGroup", &TES3::ActionData::currentAnimGroup);
			usertypeDefinition.set("lastBarterHoursPassed", &TES3::ActionData::lastBarterHoursPassed);
			usertypeDefinition.set("physicalDamage", &TES3::ActionData::physicalDamage);
			usertypeDefinition.set("walkDestination", &TES3::ActionData::walkDestination);

			// Access to other objects that need to be packaged.
			usertypeDefinition.set("hitTarget", sol::readonly_property([](TES3::ActionData& self) { return makeLuaObject(self.hitTarget); }));
			usertypeDefinition.set("nockedProjectile", sol::readonly_property([](TES3::ActionData& self) { return makeLuaObject(self.nockedProjectile); }));
			usertypeDefinition.set("stolenFrom", sol::readonly_property([](TES3::ActionData& self) { return makeLuaObject(self.stolenFromFactionOrNPC); }));
			usertypeDefinition.set("target", sol::readonly_property([](TES3::ActionData& self) { return makeLuaObject(self.target); }));
		}
	}
}

#include "TES3LockpickLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Lockpick.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {
		void bindTES3Lockpick() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Lockpick>("tes3lockpick");
			usertypeDefinition.set("new", sol::no_constructor);

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>());
			setUserdataForPhysicalObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition.set("condition", &TES3::Lockpick::maxCondition);
			usertypeDefinition.set("quality", &TES3::Lockpick::quality);
			usertypeDefinition.set("value", &TES3::Lockpick::value);
			usertypeDefinition.set("weight", &TES3::Lockpick::weight);

			// Access to other objects that need to be packaged.
			usertypeDefinition.set("script", sol::readonly_property([](TES3::Lockpick& self) { return makeLuaObject(self.getScript()); }));

			// Functions exposed as properties.
			usertypeDefinition.set("icon", sol::property(
				&TES3::Lockpick::getIconPath,
				[](TES3::Lockpick& self, const char* value) { if (strlen(value) < 32) strcpy(self.icon, value); }
			));
			usertypeDefinition.set("mesh", sol::property(&TES3::Lockpick::getModelPath, &TES3::Lockpick::setModelPath));
			usertypeDefinition.set("name", sol::property(&TES3::Lockpick::getName, &TES3::Lockpick::setName));

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition.set("model", sol::property(&TES3::Lockpick::getModelPath, &TES3::Lockpick::setModelPath));
		}
	}
}

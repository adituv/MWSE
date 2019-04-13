#include "TES3GlobalVariableLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3GlobalVariable.h"

namespace mwse {
	namespace lua {
		void bindTES3GlobalVariable() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::GlobalVariable>("tes3globalVariable");
			usertypeDefinition.set("new", sol::no_constructor);

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition.set(sol::base_classes, sol::bases<TES3::BaseObject>());
			setUserdataForBaseObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition.set("value", &TES3::GlobalVariable::value);
		}
	}
}

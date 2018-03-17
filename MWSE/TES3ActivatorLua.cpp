#include "TES3ActivatorLua.h"

#include "LuaManager.h"

#include "TES3Activator.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {
		void bindTES3Activator() {
			auto type = LuaManager::getInstance().getState().new_usertype<TES3::Activator>("TES3Activator",
				// Disable construction of this type.
				"new", sol::no_constructor,

				//
				// Properties.
				//

				"objectType", &TES3::Activator::objectType,

				"id", sol::readonly_property(&TES3::Activator::getObjectID),
				"name", sol::property(&TES3::Activator::getName, &TES3::Activator::setName),

				"model", sol::readonly_property(&TES3::Activator::getModelPath),

				"script", sol::readonly_property(&TES3::Activator::getScript)

				);
		}
	}
}

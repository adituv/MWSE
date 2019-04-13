#include "NITimeControllerLua.h"

#include "NIObjectLua.h"

#include "sol.hpp"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIRTTI.h"
#include "NIObjectNET.h"
#include "NITimeController.h"

namespace mwse {
	namespace lua {
		void bindNITimeController() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::TimeController>("niTimeController");
			usertypeDefinition.set("new", sol::no_constructor);

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition.set(sol::base_classes, sol::bases<NI::Object>());
			setUserdataForNIObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition.set("frequency", &NI::TimeController::frequency);
			usertypeDefinition.set("phase", &NI::TimeController::phase);
			usertypeDefinition.set("lowKeyFrame", &NI::TimeController::lowKeyFrame);
			usertypeDefinition.set("highKeyFrame", &NI::TimeController::highKeyFrame);
			usertypeDefinition.set("startTime", &NI::TimeController::startTime);
			usertypeDefinition.set("lastTime", &NI::TimeController::lastTime);
			usertypeDefinition.set("target", sol::readonly_property(&NI::TimeController::target));
			usertypeDefinition.set("nextController", &NI::TimeController::nextController);
		}
	}
}

#include "TES3AILua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3AIData.h"
#include "TES3AIPackage.h"
#include "TES3Cell.h"
#include "TES3MobileActor.h"

namespace mwse {
	namespace lua {
		void bindTES3AI() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Binding for TES3::AIPlanner
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::AIPlanner>("tes3aiPlanner");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("indexActivePackage", sol::readonly_property(&TES3::AIPlanner::indexActivePackage));
				usertypeDefinition.set("countPackages", sol::readonly_property(&TES3::AIPlanner::countPackages));

				// Access to other objects that need to be packaged.
				usertypeDefinition.set("mobile", sol::readonly_property([](TES3::AIPlanner& self) { return makeLuaObject(self.mobileActor); }));

				// Basic function binding.
				usertypeDefinition.set("getActivePackage", &TES3::AIPlanner::getActivePackage);

				// Indirect bindings to unions and arrays.
				usertypeDefinition.set("packages", sol::readonly_property([](TES3::AIPlanner& self) { return std::ref(self.packages); }));
			}

			// Binding for TES3::AIPackage
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::AIPackage>("tes3aiPackage");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("distance", sol::readonly_property(&TES3::AIPackage::distance));
				usertypeDefinition.set("duration", sol::readonly_property(&TES3::AIPackage::duration));
				usertypeDefinition.set("hourOfDay", sol::readonly_property(&TES3::AIPackage::hourOfDay));
				usertypeDefinition.set("isDone", sol::readonly_property(&TES3::AIPackage::done));
				usertypeDefinition.set("isFinalized", sol::readonly_property(&TES3::AIPackage::finalized));
				usertypeDefinition.set("isMoving", sol::readonly_property(&TES3::AIPackage::moving));
				usertypeDefinition.set("isReset", sol::readonly_property(&TES3::AIPackage::reset));
				usertypeDefinition.set("isStarted", sol::readonly_property(&TES3::AIPackage::started));
				usertypeDefinition.set("targetPosition", sol::readonly_property(&TES3::AIPackage::targetPosition));
				usertypeDefinition.set("type", sol::readonly_property(&TES3::AIPackage::packageType));

				// Access to other objects that need to be packaged.
				usertypeDefinition.set("destinationCell", sol::readonly_property([](TES3::AIPackage& self) { return makeLuaObject(self.destinationCell); }));
			}
		}
	}
}


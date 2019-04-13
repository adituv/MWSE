#include "TES3ReferenceLua.h"

#include "sol.hpp"
#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Cell.h"
#include "TES3Reference.h"
#include "TES3ReferenceList.h"

namespace mwse {
	namespace lua {
		void bindTES3ReferenceList() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::ReferenceList>("tes3referenceList");
			usertypeDefinition.set("new", sol::no_constructor);

			// Basic property binding.
			usertypeDefinition.set("size", sol::readonly_property(&TES3::ReferenceList::size));

			// Access to other objects that need to be packaged.
			usertypeDefinition.set("cell", sol::readonly_property([](TES3::ReferenceList& self) { return makeLuaObject(self.cell); }));
			usertypeDefinition.set("head", sol::readonly_property([](TES3::ReferenceList& self) { return makeLuaObject(self.head); }));
			usertypeDefinition.set("tail", sol::readonly_property([](TES3::ReferenceList& self) { return makeLuaObject(self.tail); }));
		}
	}
}

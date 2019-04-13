#include "TES3EnchantmentLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Enchantment.h"

namespace mwse {
	namespace lua {
		void bindTES3Enchantment() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Enchantment>("tes3enchantment");
			usertypeDefinition.set("new", sol::no_constructor);

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Object, TES3::BaseObject>());
			setUserdataForObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition.set("castType", &TES3::Enchantment::castType);
			usertypeDefinition.set("chargeCost", &TES3::Enchantment::chargeCost);
			usertypeDefinition.set("flags", &TES3::Enchantment::flags);
			usertypeDefinition.set("magickaCost", &TES3::Enchantment::magickaCost);
			usertypeDefinition.set("maxCharge", &TES3::Enchantment::maxCharge);

			// Indirect bindings to unions and arrays.
			usertypeDefinition.set("effects", sol::readonly_property([](TES3::Enchantment& self) { return std::ref(self.effects); }));
		}
	}
}

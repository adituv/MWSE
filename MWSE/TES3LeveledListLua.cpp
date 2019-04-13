#include "TES3InventoryLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3LeveledList.h"

namespace mwse {
	namespace lua {
		void bindTES3LeveledList() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Binding for TES3::LeveledListNode
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::LeveledListNode>("tes3leveledListNode");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("levelRequired", sol::readonly_property(&TES3::LeveledListNode::levelRequirement));

				// Access to other objects that need to be packaged.
				usertypeDefinition.set("object", sol::readonly_property([](TES3::LeveledListNode& self) { return makeLuaObject(self.object); }));
			}

			// Binding for TES3::LeveledCreature
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::LeveledCreature>("tes3leveledCreature");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::PhysicalObject, TES3::Object, TES3::BaseObject>());
				setUserdataForPhysicalObject(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition.set("chanceForNothing", sol::readonly_property(&TES3::LeveledCreature::chanceForNothing));
				usertypeDefinition.set("count", sol::readonly_property(&TES3::LeveledCreature::itemCount));
				usertypeDefinition.set("flags", sol::readonly_property(&TES3::LeveledCreature::flags));
				usertypeDefinition.set("list", sol::readonly_property(&TES3::LeveledCreature::itemList));

				// Basic function binding.
				usertypeDefinition.set("pickFrom", [](TES3::LeveledCreature& self) { return makeLuaObject(self.resolve()); });
			}

			// Binding for TES3::LeveledItem
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::LeveledItem>("tes3leveledItem");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::PhysicalObject, TES3::Object, TES3::BaseObject>());
				setUserdataForPhysicalObject(usertypeDefinition);

				// Allow object to be converted to strings using their object ID.
				usertypeDefinition.set(sol::meta_function::to_string, &TES3::LeveledItem::getObjectID);

				// Basic property binding.
				usertypeDefinition.set("chanceForNothing", sol::readonly_property(&TES3::LeveledItem::chanceForNothing));
				usertypeDefinition.set("count", sol::readonly_property(&TES3::LeveledItem::itemCount));
				usertypeDefinition.set("flags", sol::readonly_property(&TES3::LeveledItem::flags));
				usertypeDefinition.set("list", sol::readonly_property(&TES3::LeveledItem::itemList));

				// Basic function binding.
				usertypeDefinition.set("pickFrom", [](TES3::LeveledItem& self) { return makeLuaObject(self.resolve()); });
			}
		}
	}
}

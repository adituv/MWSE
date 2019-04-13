#include "TES3UIInventoryTileLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"

#include "TES3Item.h"

namespace mwse {
	namespace lua {
		void bindTES3UIInventoryTile() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::UI::InventoryTile>("tes3inventoryTile");
			usertypeDefinition.set("new", sol::no_constructor);

			// Functions exposed as properties.
			usertypeDefinition.set("count", sol::readonly_property(&TES3::UI::InventoryTile::count));
			usertypeDefinition.set("element", sol::readonly_property(&TES3::UI::InventoryTile::element));
			usertypeDefinition.set("flags", sol::readonly_property(&TES3::UI::InventoryTile::flags));
			usertypeDefinition.set("isBoundItem", sol::readonly_property(&TES3::UI::InventoryTile::isBoundItem));
			usertypeDefinition.set("itemData", sol::readonly_property(&TES3::UI::InventoryTile::itemData));
			usertypeDefinition.set("type", sol::readonly_property(&TES3::UI::InventoryTile::tileType));

			// Access to other objects that need to be packaged.
			usertypeDefinition.set("item", sol::readonly_property([](TES3::UI::InventoryTile& self) { return makeLuaObject(self.item); }));

			// Access into flags.
			usertypeDefinition.set("isBartered", sol::readonly_property([](TES3::UI::InventoryTile& self) { return self.getFlag(TES3::UI::InventoryTileFlag::Bartered); }));
			usertypeDefinition.set("isEquipped", sol::readonly_property([](TES3::UI::InventoryTile& self) { return self.getFlag(TES3::UI::InventoryTileFlag::Equipped); }));
		}
	}
}

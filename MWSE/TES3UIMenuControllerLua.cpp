#pragma once

#include "TES3UIMenuControllerLua.h"

#include "LuaManager.h"

#include "TES3ScriptCompiler.h"

#include "TES3UIElement.h"
#include "TES3UIMenuController.h"

#include "NINode.h"

namespace mwse {
	namespace lua {
		void bindTES3UIMenuController() {
			sol::state& state = LuaManager::getInstance().getState();

			// Binding for TES3::UI::FontColor
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::UI::FontColor>("tes3uiFontColor");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("r", &TES3::UI::FontColor::r);
				usertypeDefinition.set("g", &TES3::UI::FontColor::g);
				usertypeDefinition.set("b", &TES3::UI::FontColor::b);
			}

			// Binding for TES3::UI::FontColor
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::UI::MenuController>("tes3uiMenuController");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("fontColors", sol::readonly_property([](TES3::UI::MenuController& self) { return std::ref(self.fontColors); }));
				usertypeDefinition.set("helpDelay", sol::readonly_property(&TES3::UI::MenuController::helpDelay));
				usertypeDefinition.set("helpRoot", sol::readonly_property(&TES3::UI::MenuController::helpRoot));
				usertypeDefinition.set("inputController", sol::readonly_property(&TES3::UI::MenuController::menuInputController));
				usertypeDefinition.set("mainRoot", sol::readonly_property(&TES3::UI::MenuController::mainRoot));
				usertypeDefinition.set("scriptCompiler", sol::readonly_property(&TES3::UI::MenuController::scriptCompiler));

				// Functions exposed as properties.
				usertypeDefinition.set("inventoryMenuEnabled", sol::property(&TES3::UI::MenuController::getInventoryMenuEnabled, &TES3::UI::MenuController::setInventoryMenuEnabled));
				usertypeDefinition.set("magicMenuEnabled", sol::property(&TES3::UI::MenuController::getMagicMenuEnabled, &TES3::UI::MenuController::setMagicMenuEnabled));
				usertypeDefinition.set("mapMenuEnabled", sol::property(&TES3::UI::MenuController::getMapMenuEnabled, &TES3::UI::MenuController::setMapMenuEnabled));
				usertypeDefinition.set("statsMenuEnabled", sol::property(&TES3::UI::MenuController::getStatsMenuEnabled, &TES3::UI::MenuController::setStatsMenuEnabled));
			}
		}
	}
}

#include "TES3SpellLua.h"

#include "sol.hpp"
#include "LuaUtil.h"
#include "LuaManager.h"

#include "TES3Util.h"

#include "TES3Reference.h"
#include "TES3Spell.h"

namespace mwse {
	namespace lua {
		TES3::Spell* createSpell(std::string id, std::string name) {
			// Make sure a spell doesn't already exist with this id.
			if (tes3::getObjectById<TES3::Spell>(id, TES3::ObjectType::Spell) != NULL) {
				return NULL;
			}

			// Get spell list.
			TES3::LinkedList<TES3::Spell>* spellsList = tes3::getDataHandler()->nonDynamicData->spellsList;
			TES3::Spell* spellListHead = spellsList->head;

			// Create new spell.
			TES3::Spell* newSpell = tes3::malloc<TES3::Spell>();
			memset(newSpell, 0, sizeof(TES3::Spell));
			newSpell->vTable = spellListHead->vTable;
			newSpell->objectType = TES3::ObjectType::Spell;
			newSpell->owningCollection.asSpellList = spellsList;
			newSpell->magickaCost = 1;

			// Limit name to 31 characters.
			if (name.length() > 31) {
				return NULL;
			}

			// Set id/name.
			newSpell->setID(id.c_str());
			newSpell->setName(name.c_str());

			// Set effects.
			for (int i = 0; i < 8; i++) {
				newSpell->effects[i].effectID = TES3::EffectID::None;
			}

			// Set the first effect just so that there is something? TODO: Why?
			tes3::setEffect(newSpell->effects, 1, TES3::EffectID::WaterBreathing, TES3::SkillID::Invalid, TES3::EffectRange::Self, 0, 1, 0, 0);

			// Add object to the game.
			tes3::getDataHandler()->nonDynamicData->addNewObject(newSpell);

			// Finally return the spell.
			return newSpell;
		}

		void bindTES3Spell() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.create_simple_usertype<TES3::Spell>();
			usertypeDefinition.set("new", sol::no_constructor);

			// Basic property binding.
			usertypeDefinition.set("castType", &TES3::Spell::castType);
			usertypeDefinition.set("flags", &TES3::Spell::spellFlags);
			usertypeDefinition.set("magickaCost", &TES3::Spell::magickaCost);

			// Indirect bindings to unions and arrays.
			usertypeDefinition.set("effects", sol::readonly_property([](TES3::Spell& self) { return std::ref(self.effects); }));

			// Basic function binding.
			usertypeDefinition.set("create", &createSpell);
			usertypeDefinition.set("calculateCastChance", [](TES3::Spell& self, sol::table params) -> float
			{
				bool checkMagicka = getOptionalParam<bool>(params, "checkMagicka", true);
				sol::object caster = params["caster"];
				if (caster.is<TES3::Reference>()) {
					return self.calculateCastChance(caster.as<TES3::Reference*>(), checkMagicka);
				}
				else if (caster.is<TES3::MobileActor>()) {
					return self.calculateCastChance(caster.as<TES3::MobileActor*>(), checkMagicka);
				}

				return 0.0f;
			}
			);

			// Functions exposed as properties.
			usertypeDefinition.set("autoCalc", sol::property(&TES3::Spell::getAutoCalc, &TES3::Spell::setAutoCalc));
			usertypeDefinition.set("name", sol::property(&TES3::Spell::getName, &TES3::Spell::setName));

			// Finish up our usertype.
			state.set_usertype("tes3spell", usertypeDefinition);
		}
	}
}

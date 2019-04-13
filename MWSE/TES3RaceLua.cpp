#include "TES3RaceLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3BodyPart.h"
#include "TES3Race.h"

namespace mwse {
	namespace lua {
		void bindTES3Race() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Binding for TES3::Race::SkillBonus.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Race::SkillBonus>("tes3raceSkillBonus");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("skill", &TES3::Race::SkillBonus::skill);
				usertypeDefinition.set("bonus", &TES3::Race::SkillBonus::bonus);
			}

			// Binding for TES3::Race::BaseAttribute.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Race::BaseAttribute>("tes3raceBaseAttribute");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("male", &TES3::Race::BaseAttribute::male);
				usertypeDefinition.set("female", &TES3::Race::BaseAttribute::female);
			}

			// Binding for TES3::Race::HeightWeight.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Race::HeightWeight>("tes3raceHeightWeight");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("male", &TES3::Race::HeightWeight::male);
				usertypeDefinition.set("female", &TES3::Race::HeightWeight::female);
			}

			// Binding for TES3::Race::BodyParts.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Race::BodyParts>("tes3raceBodyParts");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("head1", &TES3::Race::BodyParts::head1);
				usertypeDefinition.set("hair", &TES3::Race::BodyParts::hair);
				usertypeDefinition.set("neck", &TES3::Race::BodyParts::neck);
				usertypeDefinition.set("chest", &TES3::Race::BodyParts::chest);
				usertypeDefinition.set("groin", &TES3::Race::BodyParts::groin);
				usertypeDefinition.set("hands", &TES3::Race::BodyParts::hands);
				usertypeDefinition.set("wrist", &TES3::Race::BodyParts::wrist);
				usertypeDefinition.set("forearm", &TES3::Race::BodyParts::forearm);
				usertypeDefinition.set("upperArm", &TES3::Race::BodyParts::upperArm);
				usertypeDefinition.set("foot", &TES3::Race::BodyParts::foot);
				usertypeDefinition.set("ankle", &TES3::Race::BodyParts::ankle);
				usertypeDefinition.set("knee", &TES3::Race::BodyParts::knee);
				usertypeDefinition.set("upperLeg", &TES3::Race::BodyParts::upperLeg);
				usertypeDefinition.set("clavicle", &TES3::Race::BodyParts::clavicle);
				usertypeDefinition.set("tail", &TES3::Race::BodyParts::tail);
				usertypeDefinition.set("head2", &TES3::Race::BodyParts::head2);
			}

			// Binding for TES3::Race.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Race>("tes3race");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::BaseObject>());
				setUserdataForBaseObject(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition.set("femaleBody", &TES3::Race::femaleBody);
				usertypeDefinition.set("flags", &TES3::Race::flags);
				usertypeDefinition.set("height", &TES3::Race::height);
				usertypeDefinition.set("maleBody", &TES3::Race::maleBody);
				usertypeDefinition.set("weight", &TES3::Race::weight);

				// Indirect bindings to unions and arrays.
				usertypeDefinition.set("baseAttributes", sol::readonly_property([](TES3::Race& self) { return std::ref(self.baseAttributes); }));
				usertypeDefinition.set("skillBonuses", sol::readonly_property([](TES3::Race& self) { return std::ref(self.skillBonuses); }));

				// Functions exposed as properties.
				usertypeDefinition.set("name", sol::readonly_property([](TES3::Race& self) { return self.name; }));
			}
		}
	}
}

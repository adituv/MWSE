#pragma once

#include "sol.hpp"

#include "LuaUtil.h"

#include "NINode.h"
#include "TES3GameFile.h"
#include "TES3ReferenceList.h"

namespace mwse {
	namespace lua {
		template <typename T>
		void setUserdataForBaseObject(sol::usertype<T>& usertypeDefinition) {
			// Basic property binding.
			usertypeDefinition.set("objectType", sol::readonly_property(&TES3::BaseObject::objectType));
			usertypeDefinition.set("objectFlags", sol::readonly_property(&TES3::BaseObject::objectFlags));

			// Allow object to be converted to strings using their object ID.
			usertypeDefinition.set(sol::meta_function::to_string, &TES3::BaseObject::getObjectID);

			// Functions exposed as properties.
			usertypeDefinition.set("id", sol::readonly_property(&TES3::BaseObject::getObjectID));
			usertypeDefinition.set("sourceMod", sol::readonly_property(
				[](TES3::BaseObject& self) -> const char*
			{
				if (self.sourceMod) {
					return self.sourceMod->filename;
				}
				return nullptr;
			}
			));
			usertypeDefinition.set("modified", sol::property(&TES3::BaseObject::getObjectModified, &TES3::BaseObject::setObjectModified));
			usertypeDefinition.set("disabled", sol::readonly_property([](TES3::BaseObject& self) { return (self.objectFlags & TES3::ObjectFlag::Disabled) != 0; }));
			usertypeDefinition.set("deleted", sol::readonly_property([](TES3::BaseObject& self) { return (self.objectFlags & TES3::ObjectFlag::Delete) != 0; }));
		}

		template <typename T>
		void setUserdataForObject(sol::usertype<T>& usertypeDefinition) {
			setUserdataForBaseObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition.set("scale", sol::property(&TES3::Object::getScale, [](TES3::Object& self, float value) { self.setScale(value); }));

			// Indirect bindings to unions and arrays.
			usertypeDefinition.set("owningCollection", sol::property([](TES3::Object& self) { return self.owningCollection.asReferenceList; }));

			// Access to other objects that need to be packaged.
			usertypeDefinition.set("nextInCollection", sol::readonly_property([](TES3::Object& self) { return makeLuaObject(self.nextInCollection); }));
			usertypeDefinition.set("previousInCollection", sol::readonly_property([](TES3::Object& self) { return makeLuaObject(self.previousInCollection); }));
			usertypeDefinition.set("sceneNode", sol::readonly_property([](TES3::Object& self) { return makeLuaObject(self.sceneNode); }));
			usertypeDefinition.set("sceneReference", sol::readonly_property([](TES3::Object& self) { return makeLuaObject(self.sceneGraphReference); }));
		}

		template <typename T>
		void setUserdataForPhysicalObject(sol::usertype<T>& usertypeDefinition) {
			setUserdataForObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition.set("boundingBox", sol::readonly_property(&TES3::PhysicalObject::boundingBox));

			// Functions exposed as properties.
			usertypeDefinition.set("stolenList", sol::readonly_property(&TES3::PhysicalObject::getStolenList));
		}
	}
}
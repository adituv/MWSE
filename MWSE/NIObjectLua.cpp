#include "NIObjectLua.h"

#include "sol.hpp"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIDefines.h"
#include "NIAVObject.h"
#include "NIDynamicEffect.h"
#include "NINode.h"
#include "NIObject.h"
#include "NIObjectNET.h"
#include "NIPointer.h"
#include "NIRTTI.h"

namespace mwse {
	namespace lua {
		void bindNIObject() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Binding for NI::RTTI.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::RTTI>("niRTTI");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("name", &NI::RTTI::name);
				usertypeDefinition.set("parent", &NI::RTTI::baseRTTI);
			}

			// Binding for NI::Object.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::Object>("niObject");
				usertypeDefinition.set("new", sol::no_constructor);

				// Inherit NI::Object.
				setUserdataForNIObject(usertypeDefinition);
			}

			// Binding for NI::ObjectNET.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::ObjectNET>("niObjectNET");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<NI::Object>());
				setUserdataForNIObjectNET(usertypeDefinition);
			}

			// Binding for NI::AVObject.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::AVObject>("niAVObject");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<NI::ObjectNET, NI::Object>());
				setUserdataForNIAVObject(usertypeDefinition);
			}

			// Binding for NI::DynamicEffectLinkedList.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::DynamicEffectLinkedList>("niDynamicEffectLinkedList");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("data", sol::readonly_property([](NI::DynamicEffectLinkedList& self) { return makeLuaNiPointer(self.data); }));
				usertypeDefinition.set("next", &NI::DynamicEffectLinkedList::next);
			}

			// Binding for NI::NodeLinkedList.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::NodeLinkedList>("niNodeLinkedList");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("data", sol::readonly_property([](NI::NodeLinkedList& self) { return makeLuaNiPointer(self.data); }));
				usertypeDefinition.set("next", &NI::NodeLinkedList::next);
			}

			// Binding for NI::PropertyLinkedList.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::PropertyLinkedList>("niPropertyLinkedList");
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("data", sol::readonly_property([](NI::PropertyLinkedList& self) { return makeLuaNiPointer(self.data); }));
				usertypeDefinition.set("next", &NI::PropertyLinkedList::next);
			}
		}
	}
}

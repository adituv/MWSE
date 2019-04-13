#include "NILightLua.h"

#include "sol.hpp"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NINode.h"
#include "NIAmbientLight.h"
#include "NIDirectionalLight.h"
#include "NIRTTI.h"
#include "NISpotLight.h"

namespace mwse {
	namespace lua {
		void bindNILight() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Binding for NI::DynamicEffect. TODO: MOVE THIS OUTSIDE OF HERE AT SOME POINT!
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::DynamicEffect>("niDynamicEffect");
				usertypeDefinition.set("new", sol::no_constructor);

				// Inherit NI::DynamicEffect.
				usertypeDefinition.set(sol::base_classes, sol::bases<NI::Object, NI::ObjectNET, NI::AVObject>());
				setUserdataForNIDynamicEffect(usertypeDefinition);
			}

			// Binding for NI::Light.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::Light>("niLight");
				usertypeDefinition.set("new", sol::no_constructor);

				// Inherit NI::DynamicEffect.
				usertypeDefinition.set(sol::base_classes, sol::bases<NI::Object, NI::ObjectNET, NI::AVObject, NI::DynamicEffect>());
				setUserdataForNILight(usertypeDefinition);
			}

			// Binding for NI::AmbientLight.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::AmbientLight>("niAmbientLight");
				usertypeDefinition.set("new", sol::no_constructor);

				// Inherit NI::Light.
				usertypeDefinition.set(sol::base_classes, sol::bases<NI::Object, NI::ObjectNET, NI::AVObject, NI::DynamicEffect, NI::Light>());
				setUserdataForNILight(usertypeDefinition);
			}

			// Binding for NI::DirectionalLight.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::DirectionalLight>("niDirectionalLight");
				usertypeDefinition.set("new", sol::no_constructor);

				// Inherit NI::Light.
				usertypeDefinition.set(sol::base_classes, sol::bases<NI::Object, NI::ObjectNET, NI::AVObject, NI::DynamicEffect, NI::Light>());
				setUserdataForNILight(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition.set("direction", &NI::DirectionalLight::direction);
			}

			// Binding for NI::PointLight.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::PointLight>("niPointLight");
				usertypeDefinition.set("new", []() { return makeLuaNiPointer(NI::PointLight::create()); });

				// Inherit NI::Light.
				usertypeDefinition.set(sol::base_classes, sol::bases<NI::Object, NI::ObjectNET, NI::AVObject, NI::DynamicEffect, NI::Light>());
				setUserdataForNIPointLight(usertypeDefinition);
			}

			// Binding for NI::SpotLight.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::SpotLight>("niSpotLight");
				usertypeDefinition.set("new", sol::no_constructor);

				// Inherit NI::Light.
				usertypeDefinition.set(sol::base_classes, sol::bases<NI::Object, NI::ObjectNET, NI::AVObject, NI::DynamicEffect, NI::Light, NI::PointLight>());
				setUserdataForNIPointLight(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition.set("direction", &NI::SpotLight::direction);
				usertypeDefinition.set("spotAngle", &NI::SpotLight::spotAngle);
				usertypeDefinition.set("spotExponent", &NI::SpotLight::spotExponent);
			}
		}
	}
}

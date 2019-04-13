#include "NIColorLua.h"

#include "sol.hpp"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIColor.h"

#include <iomanip>

namespace mwse {
	namespace lua {
		void bindNIColor() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Color>("niColor");
			usertypeDefinition.set("new", sol::no_constructor);

			// Operator overloading.
			usertypeDefinition.set(sol::meta_function::addition, &NI::Color::operator+);
			usertypeDefinition.set(sol::meta_function::subtraction, &NI::Color::operator-);
			usertypeDefinition.set(sol::meta_function::multiplication, sol::overload(
				sol::resolve<NI::Color(const NI::Color&)>(&NI::Color::operator*),
				sol::resolve<NI::Color(const float)>(&NI::Color::operator*)
			));
			usertypeDefinition.set(sol::meta_function::to_string, [](NI::Color& self) {
				std::ostringstream ss;
				ss << std::fixed << std::setprecision(2) << "<" << self.r << ", " << self.g << ", " << self.b << ">";
				return ss.str();
			});

			// Basic property binding.
			usertypeDefinition.set("r", &NI::Color::r);
			usertypeDefinition.set("g", &NI::Color::g);
			usertypeDefinition.set("b", &NI::Color::b);

			// Long aliases for color access.
			usertypeDefinition.set("red", &NI::Color::r);
			usertypeDefinition.set("green", &NI::Color::g);
			usertypeDefinition.set("blue", &NI::Color::b);

			// Basic function binding.
			usertypeDefinition.set("copy", [](NI::Color& self) { return NI::Color(self); });
			usertypeDefinition.set("clamp", &NI::Color::clamp);

			// Conversion to TES3::Vector3.
			usertypeDefinition.set("toVector3", [](NI::Color& self) { return TES3::Vector3(self.r, self.g, self.b); });
		}
	}
}

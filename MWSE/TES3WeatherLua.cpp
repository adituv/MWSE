#include "TES3WeatherLua.h"

#include "LuaManager.h"

#include "TES3WeatherAsh.h"
#include "TES3WeatherBlight.h"
#include "TES3WeatherBlizzard.h"
#include "TES3WeatherClear.h"
#include "TES3WeatherCloudy.h"
#include "TES3WeatherController.h"
#include "TES3WeatherFoggy.h"
#include "TES3WeatherOvercast.h"
#include "TES3WeatherRain.h"
#include "TES3WeatherSnow.h"
#include "TES3WeatherThunder.h"

namespace mwse {
	namespace lua {
		void bindTES3Weather() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Binding for TES3::WeatherAsh
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::WeatherAsh>("tes3weatherAsh");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Weather>());
				setUserdataForWeather(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition.set("threshold", &TES3::WeatherAsh::stormThreshold);
			}

			// Binding for TES3::WeatherBlight
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::WeatherBlight>("tes3weatherBlight");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Weather>());
				setUserdataForWeather(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition.set("blightDiseaseChance", &TES3::WeatherBlight::diseaseChance);
				usertypeDefinition.set("blightDiseases", sol::readonly_property(&TES3::WeatherBlight::blightDiseases));
				usertypeDefinition.set("threshold", &TES3::WeatherBlight::stormThreshold);
			}

			// Binding for TES3::WeatherBlizzard
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::WeatherBlizzard>("tes3weatherBlizzard");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Weather>());
				setUserdataForWeather(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition.set("threshold", &TES3::WeatherBlizzard::stormThreshold);
			}

			// Binding for TES3::WeatherClear
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::WeatherClear>("tes3weatherClear");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Weather>());
				setUserdataForWeather(usertypeDefinition);
			}

			// Binding for TES3::WeatherCloudy
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::WeatherCloudy>("tes3weatherCloudy");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Weather>());
				setUserdataForWeather(usertypeDefinition);
			}

			// Binding for TES3::WeatherFoggy
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::WeatherFoggy>("tes3weatherFoggy");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Weather>());
				setUserdataForWeather(usertypeDefinition);
			}

			// Binding for TES3::WeatherOvercast
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::WeatherOvercast>("tes3weatherOvercast");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Weather>());
				setUserdataForWeather(usertypeDefinition);
			}

			// Binding for TES3::WeatherRain
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::WeatherRain>("tes3weatherRain");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Weather>());
				setUserdataForWeather(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition.set("maxParticles", &TES3::WeatherRain::raindropsMax);
				usertypeDefinition.set("particleEntranceSpeed", &TES3::WeatherRain::rainEntranceSpeed);
				usertypeDefinition.set("particleHeightMax", &TES3::WeatherRain::rainHeightMax);
				usertypeDefinition.set("particleHeightMin", &TES3::WeatherRain::rainHeightMin);
				usertypeDefinition.set("particleRadius", &TES3::WeatherRain::rainRadius);
				usertypeDefinition.set("rainActive", sol::readonly_property(&TES3::WeatherRain::rainPlaying));
				usertypeDefinition.set("threshold", &TES3::WeatherRain::rainThreshold);

				// Binding for IDs and paths.
				usertypeDefinition.set("rainLoopSoundId", sol::property(
					[](TES3::WeatherRain& self) { return self.soundIDRainLoop; },
					[](TES3::WeatherRain& self, const char* value) { if (strlen(value) < 260) strcpy(self.soundIDRainLoop, value); }
				));

				// Access to other objects that need to be packaged.
				usertypeDefinition.set("rainLoopSound", sol::readonly_property([](TES3::WeatherRain& self) { return makeLuaObject(self.rainLoopSound); }));
			}

			// Binding for TES3::WeatherSnow
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::WeatherSnow>("tes3weatherSnow");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Weather>());
				setUserdataForWeather(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition.set("maxParticles", &TES3::WeatherSnow::snowflakesMax);
				usertypeDefinition.set("particleEntranceSpeed", &TES3::WeatherSnow::snowEntranceSpeed);
				usertypeDefinition.set("particleHeightMax", &TES3::WeatherSnow::snowHeightMax);
				usertypeDefinition.set("particleHeightMin", &TES3::WeatherSnow::snowHeightMin);
				usertypeDefinition.set("particleRadius", &TES3::WeatherSnow::snowRadius);
				usertypeDefinition.set("threshold", &TES3::WeatherSnow::snowThreshold);
			}

			// Binding for TES3::WeatherThunder
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<TES3::WeatherThunder>("tes3weatherThunder");
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Weather>());
				setUserdataForWeather(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition.set("flashDecrement", &TES3::WeatherThunder::flashDecrement);
				usertypeDefinition.set("maxParticles", &TES3::WeatherThunder::raindropsMax);
				usertypeDefinition.set("particleEntranceSpeed", &TES3::WeatherThunder::rainEntranceSpeed);
				usertypeDefinition.set("particleHeightMax", &TES3::WeatherThunder::rainHeightMax);
				usertypeDefinition.set("particleHeightMin", &TES3::WeatherThunder::rainHeightMin);
				usertypeDefinition.set("particleRadius", &TES3::WeatherThunder::rainRadius);
				usertypeDefinition.set("rainActive", &TES3::WeatherThunder::rainPlaying);
				usertypeDefinition.set("threshold", &TES3::WeatherThunder::rainThreshold);
				usertypeDefinition.set("thunderFrequency", &TES3::WeatherThunder::thunderFrequency);
				usertypeDefinition.set("thunderSoundCount", &TES3::WeatherThunder::thunderSoundCount);
				usertypeDefinition.set("thunderThreshold", &TES3::WeatherThunder::thunderThreshold);

				// Binding for IDs and paths.
				usertypeDefinition.set("rainLoopSoundId", sol::property(
					[](TES3::WeatherThunder& self) { return self.soundIdRainLoop; },
					[](TES3::WeatherThunder& self, const char* value) { if (strlen(value) < 260) strcpy(self.soundIdRainLoop, value); }
				));
				usertypeDefinition.set("thunderSound1Id", sol::property(
					[](TES3::WeatherThunder& self) { return self.soundIdThunder1; },
					[](TES3::WeatherThunder& self, const char* value) { if (strlen(value) < 260) strcpy(self.soundIdThunder1, value); }
				));
				usertypeDefinition.set("thunderSound2Id", sol::property(
					[](TES3::WeatherThunder& self) { return self.soundIdThunder2; },
					[](TES3::WeatherThunder& self, const char* value) { if (strlen(value) < 260) strcpy(self.soundIdThunder2, value); }
				));
				usertypeDefinition.set("thunderSound3Id", sol::property(
					[](TES3::WeatherThunder& self) { return self.soundIdThunder3; },
					[](TES3::WeatherThunder& self, const char* value) { if (strlen(value) < 260) strcpy(self.soundIdThunder3, value); }
				));
				usertypeDefinition.set("thunderSound4Id", sol::property(
					[](TES3::WeatherThunder& self) { return self.soundIdThunder4; },
					[](TES3::WeatherThunder& self, const char* value) { if (strlen(value) < 260) strcpy(self.soundIdThunder4, value); }
				));

				// Access to other objects that need to be packaged.
				usertypeDefinition.set("rainLoopSound", sol::readonly_property([](TES3::WeatherThunder& self) { return makeLuaObject(self.rainLoopSound); }));
				usertypeDefinition.set("thunderSound1", sol::readonly_property([](TES3::WeatherThunder& self) { return makeLuaObject(self.thunderSound1); }));
				usertypeDefinition.set("thunderSound2", sol::readonly_property([](TES3::WeatherThunder& self) { return makeLuaObject(self.thunderSound2); }));
				usertypeDefinition.set("thunderSound3", sol::readonly_property([](TES3::WeatherThunder& self) { return makeLuaObject(self.thunderSound3); }));
				usertypeDefinition.set("thunderSound4", sol::readonly_property([](TES3::WeatherThunder& self) { return makeLuaObject(self.thunderSound4); }));
			}
		}
	}
}

#include "LuaUtil.h"

#include "sol.hpp"
#include "LuaManager.h"

#include "TES3Util.h"
#include "Log.h"

#include "NIAmbientLight.h"
#include "NIAVObject.h"
#include "NICamera.h"
#include "NICollisionSwitch.h"
#include "NIDefines.h"
#include "NIDirectionalLight.h"
#include "NINode.h"
#include "NIObject.h"
#include "NIObjectNET.h"
#include "NIPick.h"
#include "NIPointLight.h"
#include "NIRTTI.h"
#include "NISpotLight.h"
#include "NISwitchNode.h"
#include "NITriShape.h"

#include "TES3Defines.h"
#include "TES3Activator.h"
#include "TES3Alchemy.h"
#include "TES3Apparatus.h"
#include "TES3Armor.h"
#include "TES3BodyPart.h"
#include "TES3Book.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3Clothing.h"
#include "TES3Container.h"
#include "TES3Creature.h"
#include "TES3DataHandler.h"
#include "TES3Dialogue.h"
#include "TES3DialogueInfo.h"
#include "TES3Door.h"
#include "TES3Enchantment.h"
#include "TES3Faction.h"
#include "TES3GameFile.h"
#include "TES3GameSetting.h"
#include "TES3GlobalVariable.h"
#include "TES3Ingredient.h"
#include "TES3LeveledList.h"
#include "TES3Light.h"
#include "TES3Lockpick.h"
#include "TES3MagicEffect.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3Misc.h"
#include "TES3MobileCreature.h"
#include "TES3MobileNPC.h"
#include "TES3MobilePlayer.h"
#include "TES3MobileProjectile.h"
#include "TES3NPC.h"
#include "TES3Probe.h"
#include "TES3Quest.h"
#include "TES3Race.h"
#include "TES3Reference.h"
#include "TES3Region.h"
#include "TES3RepairTool.h"
#include "TES3Script.h"
#include "TES3Skill.h"
#include "TES3Sound.h"
#include "TES3SoundGenerator.h"
#include "TES3Spell.h"
#include "TES3Static.h"
#include "TES3UIElement.h"
#include "TES3Vectors.h"
#include "TES3Weapon.h"
#include "TES3Weather.h"
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

#include <Windows.h>

#define TES3_vTable_MobileCreature 0x74AFA4
#define TES3_vTable_MobileNPC 0x74AE6C
#define TES3_vTable_MobilePlayer 0x74B174
#define TES3_vTable_MobileProjectile 0x74B2B4
#define TES3_vTable_SpellProjectile 0x74B360

namespace mwse {
	namespace lua {
		TES3::Script* getOptionalParamExecutionScript(sol::optional<sol::table> maybeParams) {
			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeScript = params["_script"];
				if (maybeScript.valid()) {
					if (maybeScript.is<std::string>()) {
						return TES3::DataHandler::get()->nonDynamicData->findScriptByName(maybeScript.as<std::string>().c_str());
					}
					else if (maybeScript.is<TES3::Script*>()) {
						return maybeScript.as<TES3::Script*>();
					}
				}
			}

			// Fall back to the currently executing script.
			return LuaManager::getInstance().getCurrentScript();
		}

		TES3::Reference* getOptionalParamExecutionReference(sol::optional<sol::table> maybeParams) {
			TES3::Reference* reference = NULL;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeReference = params["reference"];
				if (maybeReference.valid()) {
					if (maybeReference.is<std::string>()) {
						reference = tes3::getReference(maybeReference.as<std::string>());
					}
					else if (maybeReference.is<TES3::Reference*>()) {
						reference = maybeReference.as<TES3::Reference*>();
					}
					else if (maybeReference.is<TES3::MobileActor*>()) {
						reference = maybeReference.as<TES3::MobileActor*>()->reference;
					}
				}
			}

			if (reference == NULL) {
				reference = LuaManager::getInstance().getCurrentReference();
			}

			return reference;
		}

		TES3::Script* getOptionalParamScript(sol::optional<sol::table> maybeParams, const char* key) {
			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<std::string>()) {
						return TES3::DataHandler::get()->nonDynamicData->findScriptByName(maybeValue.as<std::string>().c_str());
					}
					else if (maybeValue.is<TES3::Script*>()) {
						return maybeValue.as<TES3::Script*>();
					}
				}
			}

			return NULL;
		}

		TES3::Reference* getOptionalParamReference(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::Reference* value = NULL;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<std::string>()) {
						value = tes3::getReference(maybeValue.as<std::string>());
					}
					else if (maybeValue.is<TES3::Reference*>()) {
						value = maybeValue.as<TES3::Reference*>();
					}
					else if (maybeValue.is<TES3::MobileActor*>()) {
						value = maybeValue.as<TES3::MobileActor*>()->reference;
					}
				}
			}

			return value;
		}

		TES3::MobileActor* getOptionalParamMobileActor(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::MobileActor* value = nullptr;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<std::string>()) {
						TES3::Reference * reference = tes3::getReference(maybeValue.as<std::string>());
						if (reference) {
							value = reference->getAttachedMobileActor();
						}
					}
					else if (maybeValue.is<TES3::Reference*>()) {
						value = maybeValue.as<TES3::Reference*>()->getAttachedMobileActor();
					}
					else if (maybeValue.is<TES3::MobileActor*>()) {
						value = maybeValue.as<TES3::MobileActor*>();
					}
				}
			}

			return value;
		}

		TES3::Spell* getOptionalParamSpell(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::Spell* value = NULL;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<std::string>()) {
						value = TES3::DataHandler::get()->nonDynamicData->getSpellById(maybeValue.as<std::string>().c_str());
					}
					else if (maybeValue.is<TES3::Spell*>()) {
						value = maybeValue.as<TES3::Spell*>();
					}
				}
			}

			return value;
		}

		TES3::Dialogue* getOptionalParamDialogue(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::Dialogue* value = NULL;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<const char*>()) {
						value = TES3::DataHandler::get()->nonDynamicData->findDialogue(maybeValue.as<const char*>());
					}
					else if (maybeValue.is<TES3::Dialogue*>()) {
						value = maybeValue.as<TES3::Dialogue*>();
					}
				}
			}

			return value;
		}

		TES3::Sound* getOptionalParamSound(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::Sound* value = NULL;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<std::string>()) {
						value = TES3::DataHandler::get()->nonDynamicData->findSound(maybeValue.as<std::string>().c_str());
					}
					else if (maybeValue.is<TES3::Sound*>()) {
						value = maybeValue.as<TES3::Sound*>();
					}
				}
			}

			return value;
		}

		sol::optional<TES3::Vector3> getOptionalParamVector3(sol::optional<sol::table> maybeParams, const char* key) {
			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					// Were we given a real vector?
					if (maybeValue.is<TES3::Vector3*>()) {
						return *maybeValue.as<TES3::Vector3*>();
					}

					// Were we given a table?
					else if (maybeValue.get_type() == sol::type::table) {
						sol::table value = maybeValue.as<sol::table>();
						TES3::Vector3* result = tes3::malloc<TES3::Vector3>();
						result->x = value[1];
						result->y = value[2];
						result->z = value[3];
						return TES3::Vector3(value[1], value[2], value[3]);
					}
				}
			}

			return sol::optional<TES3::Vector3>();
		}

		TES3::Cell* getOptionalParamCell(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::Cell* value = nullptr;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<const char*>()) {
						value = TES3::DataHandler::get()->nonDynamicData->getCellByName(maybeValue.as<const char*>());
					}
					else if (maybeValue.is<TES3::Cell*>()) {
						value = maybeValue.as<TES3::Cell*>();
					}
					else if (maybeValue.get_type() == sol::type::table) {
						sol::table coordsTable = maybeValue.as<sol::table>();
						if (coordsTable.size() == 2) {
							value = TES3::DataHandler::get()->nonDynamicData->getCellByGrid(coordsTable[1], coordsTable[2]);
						}
					}
				}
			}

			return value;
		}

		void setVectorFromLua(TES3::Vector3* vector, sol::stack_object value) {
			// Is it a vector?
			if (value.is<TES3::Vector3*>()) {
				TES3::Vector3 * newVector = value.as<TES3::Vector3*>();
				vector->x = newVector->x;
				vector->y = newVector->y;
				vector->z = newVector->z;
			}
			// Allow a simple table to be provided.
			else if (value.get_type() == sol::type::table) {
				// Get the values from the table.
				sol::table table = value.as<sol::table>();
				if (table.size() == 3) {
					vector->x = table[1];
					vector->y = table[2];
					vector->z = table[3];
				}
			}
		}

		sol::object makeLuaObject(TES3::BaseObject* object) {
			if (object == NULL) {
				return sol::nil;
			}

			LuaManager& luaManager = LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();

			// Search in cache first.
			sol::object result = stateHandle.getCachedUserdata(object);
			if (result != sol::nil) {
				return result;
			}

			sol::state& state = stateHandle.state;

			switch (object->objectType) {
			case TES3::ObjectType::Activator:
				result = sol::make_object(state, reinterpret_cast<TES3::Activator*>(object));
				break;
			case TES3::ObjectType::Alchemy:
				result = sol::make_object(state, reinterpret_cast<TES3::Alchemy*>(object));
				break;
			case TES3::ObjectType::Apparatus:
				result = sol::make_object(state, reinterpret_cast<TES3::Apparatus*>(object));
				break;
			case TES3::ObjectType::Armor:
				result = sol::make_object(state, reinterpret_cast<TES3::Armor*>(object));
				break;
			case TES3::ObjectType::Bodypart:
				result = sol::make_object(state, reinterpret_cast<TES3::BodyPart*>(object));
				break;
			case TES3::ObjectType::Book:
				result = sol::make_object(state, reinterpret_cast<TES3::Book*>(object));
				break;
			case TES3::ObjectType::Cell:
				result = sol::make_object(state, reinterpret_cast<TES3::Cell*>(object));
				break;
			case TES3::ObjectType::Class:
				result = sol::make_object(state, reinterpret_cast<TES3::Class*>(object));
				break;
			case TES3::ObjectType::Clothing:
				result = sol::make_object(state, reinterpret_cast<TES3::Clothing*>(object));
				break;
			case TES3::ObjectType::Container:
			{
				if (reinterpret_cast<TES3::Actor*>(object)->actorFlags & TES3::ActorFlagContainer::IsBase) {
					result = sol::make_object(state, reinterpret_cast<TES3::Container*>(object));
				}
				else {
					result = sol::make_object(state, reinterpret_cast<TES3::ContainerInstance*>(object));
				}
			}
			break;
			case TES3::ObjectType::Creature:
			{
				if (reinterpret_cast<TES3::Actor*>(object)->actorFlags & TES3::ActorFlagCreature::IsBase) {
					result = sol::make_object(state, reinterpret_cast<TES3::Creature*>(object));
				}
				else {
					result = sol::make_object(state, reinterpret_cast<TES3::CreatureInstance*>(object));
				}
			}
			break;
			case TES3::ObjectType::Dialogue:
				result = sol::make_object(state, reinterpret_cast<TES3::Dialogue*>(object));
				break;
			case TES3::ObjectType::DialogueInfo:
				result = sol::make_object(state, reinterpret_cast<TES3::DialogueInfo*>(object));
				break;
			case TES3::ObjectType::Door:
				result = sol::make_object(state, reinterpret_cast<TES3::Door*>(object));
				break;
			case TES3::ObjectType::Enchantment:
				result = sol::make_object(state, reinterpret_cast<TES3::Enchantment*>(object));
				break;
			case TES3::ObjectType::Faction:
				result = sol::make_object(state, reinterpret_cast<TES3::Faction*>(object));
				break;
			case TES3::ObjectType::Global:
				result = sol::make_object(state, reinterpret_cast<TES3::GlobalVariable*>(object));
				break;
			case TES3::ObjectType::GameSetting:
				result = sol::make_object(state, reinterpret_cast<TES3::GameSetting*>(object));
				break;
			case TES3::ObjectType::Ingredient:
				result = sol::make_object(state, reinterpret_cast<TES3::Ingredient*>(object));
				break;
			case TES3::ObjectType::LeveledCreature:
				result = sol::make_object(state, reinterpret_cast<TES3::LeveledCreature*>(object));
				break;
			case TES3::ObjectType::LeveledItem:
				result = sol::make_object(state, reinterpret_cast<TES3::LeveledItem*>(object));
				break;
			case TES3::ObjectType::Light:
				result = sol::make_object(state, reinterpret_cast<TES3::Light*>(object));
				break;
			case TES3::ObjectType::Lockpick:
				result = sol::make_object(state, reinterpret_cast<TES3::Lockpick*>(object));
				break;
			case TES3::ObjectType::MagicEffect:
				result = sol::make_object(state, reinterpret_cast<TES3::MagicEffect*>(object));
				break;
			case TES3::ObjectType::Misc:
				result = sol::make_object(state, reinterpret_cast<TES3::Misc*>(object));
				break;
			case TES3::ObjectType::NPC:
			{
				if (reinterpret_cast<TES3::Actor*>(object)->actorFlags & TES3::ActorFlagNPC::IsBase) {
					result = sol::make_object(state, reinterpret_cast<TES3::NPC*>(object));
				}
				else {
					result = sol::make_object(state, reinterpret_cast<TES3::NPCInstance*>(object));
				}
			}
			break;
			case TES3::ObjectType::Probe:
				result = sol::make_object(state, reinterpret_cast<TES3::Probe*>(object));
				break;
			case TES3::ObjectType::Quest:
				result = sol::make_object(state, reinterpret_cast<TES3::Quest*>(object));
				break;
			case TES3::ObjectType::Race:
				result = sol::make_object(state, reinterpret_cast<TES3::Race*>(object));
				break;
			case TES3::ObjectType::Reference:
				result = sol::make_object(state, reinterpret_cast<TES3::Reference*>(object));
				break;
			case TES3::ObjectType::Region:
				result = sol::make_object(state, reinterpret_cast<TES3::Region*>(object));
				break;
			case TES3::ObjectType::Repair:
				result = sol::make_object(state, reinterpret_cast<TES3::RepairTool*>(object));
				break;
			case TES3::ObjectType::Script:
				result = sol::make_object(state, reinterpret_cast<TES3::Script*>(object));
				break;
			case TES3::ObjectType::Skill:
				result = sol::make_object(state, reinterpret_cast<TES3::Skill*>(object));
				break;
			case TES3::ObjectType::Sound:
				result = sol::make_object(state, reinterpret_cast<TES3::Sound*>(object));
				break;
			case TES3::ObjectType::SoundGenerator:
				result = sol::make_object(state, reinterpret_cast<TES3::SoundGenerator*>(object));
				break;
			case TES3::ObjectType::Spell:
				result = sol::make_object(state, reinterpret_cast<TES3::Spell*>(object));
				break;
			case TES3::ObjectType::MagicSourceInstance:
				result = sol::make_object(state, reinterpret_cast<TES3::MagicSourceInstance*>(object));
				break;
			case TES3::ObjectType::Static:
				result = sol::make_object(state, reinterpret_cast<TES3::Static*>(object));
				break;
			case TES3::ObjectType::Ammo:
			case TES3::ObjectType::Weapon:
				result = sol::make_object(state, reinterpret_cast<TES3::Weapon*>(object));
				break;
			}

			// Insert the object into cache.
			if (result != sol::nil) {
				stateHandle.insertUserdataIntoCache(object, result);
			}

			return result;
		}

		sol::object makeLuaObject(TES3::MobileObject* object) {
			if (object == NULL) {
				return sol::nil;
			}

			LuaManager& luaManager = LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();

			// Search in cache first.
			sol::object result = stateHandle.getCachedUserdata(object);
			if (result != sol::nil) {
				return result;
			}

			sol::state& state = stateHandle.state;

			switch ((unsigned int)object->vTable.mobileObject) {
			case TES3_vTable_MobileCreature:
				result = sol::make_object(state, reinterpret_cast<TES3::MobileCreature*>(object));
				break;
			case TES3_vTable_MobileNPC:
				result = sol::make_object(state, reinterpret_cast<TES3::MobileNPC*>(object));
				break;
			case TES3_vTable_MobilePlayer:
				result = sol::make_object(state, reinterpret_cast<TES3::MobilePlayer*>(object));
				break;
			case TES3_vTable_MobileProjectile:
			case TES3_vTable_SpellProjectile:
				result = sol::make_object(state, reinterpret_cast<TES3::MobileProjectile*>(object));
				break;
			}

			// Insert the object into cache.
			if (result != sol::nil) {
				stateHandle.insertUserdataIntoCache(object, result);
			}

			return result;
		}

		sol::object makeLuaObject(TES3::Weather* weather) {
			if (weather == NULL) {
				return sol::nil;
			}

			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			switch (weather->index) {
			case TES3::WeatherType::Ash: return sol::make_object(state, reinterpret_cast<TES3::WeatherAsh*>(weather));
			case TES3::WeatherType::Blight: return sol::make_object(state, reinterpret_cast<TES3::WeatherBlight*>(weather));
			case TES3::WeatherType::Blizzard: return sol::make_object(state, reinterpret_cast<TES3::WeatherBlizzard*>(weather));
			case TES3::WeatherType::Clear: return sol::make_object(state, reinterpret_cast<TES3::WeatherClear*>(weather));
			case TES3::WeatherType::Cloudy: return sol::make_object(state, reinterpret_cast<TES3::WeatherCloudy*>(weather));
			case TES3::WeatherType::Foggy: return sol::make_object(state, reinterpret_cast<TES3::WeatherFoggy*>(weather));
			case TES3::WeatherType::Overcast: return sol::make_object(state, reinterpret_cast<TES3::WeatherOvercast*>(weather));
			case TES3::WeatherType::Rain: return sol::make_object(state, reinterpret_cast<TES3::WeatherRain*>(weather));
			case TES3::WeatherType::Snow: return sol::make_object(state, reinterpret_cast<TES3::WeatherSnow*>(weather));
			case TES3::WeatherType::Thunder: return sol::make_object(state, reinterpret_cast<TES3::WeatherThunder*>(weather));
			}

			return sol::make_object(state, weather);
		}

		sol::object makeLuaObject(TES3::GameFile* gameFile) {
			if (gameFile == nullptr) {
				return sol::nil;
			}

			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;
			return sol::make_object(state, gameFile);
		}

		sol::object makeLuaObject(NI::Object* object) {
			if (object == nullptr) {
				return sol::nil;
			}

			LuaManager& luaManager = LuaManager::getInstance();

			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			switch ((uintptr_t)object->getRunTimeTypeInformation()) {
			case NI::RTTIStaticPtr::NiAlphaProperty:
				return sol::make_object(state, reinterpret_cast<NI::AlphaProperty*>(object));
			case NI::RTTIStaticPtr::NiAmbientLight:
				return sol::make_object(state, reinterpret_cast<NI::AmbientLight*>(object));
			case NI::RTTIStaticPtr::NiAVObject:
				return sol::make_object(state, reinterpret_cast<NI::AVObject*>(object));
			case NI::RTTIStaticPtr::NiCamera:
				return sol::make_object(state, reinterpret_cast<NI::Camera*>(object));
			case NI::RTTIStaticPtr::NiCollisionSwitch:
				return sol::make_object(state, reinterpret_cast<NI::CollisionSwitch*>(object));
			case NI::RTTIStaticPtr::NiDirectionalLight:
				return sol::make_object(state, reinterpret_cast<NI::DirectionalLight*>(object));
			case NI::RTTIStaticPtr::NiFogProperty:
				return sol::make_object(state, reinterpret_cast<NI::FogProperty*>(object));
			case NI::RTTIStaticPtr::NiMaterialProperty:
				return sol::make_object(state, reinterpret_cast<NI::MaterialProperty*>(object));
			case NI::RTTIStaticPtr::NiNode:
				return sol::make_object(state, reinterpret_cast<NI::Node*>(object));
			case NI::RTTIStaticPtr::NiObjectNET:
				return sol::make_object(state, reinterpret_cast<NI::ObjectNET*>(object));
			case NI::RTTIStaticPtr::NiPixelData:
				return sol::make_object(state, reinterpret_cast<NI::PixelData*>(object));
			case NI::RTTIStaticPtr::NiPointLight:
				return sol::make_object(state, reinterpret_cast<NI::PointLight*>(object));
			case NI::RTTIStaticPtr::NiSourceTexture:
				return sol::make_object(state, reinterpret_cast<NI::SourceTexture*>(object));
			case NI::RTTIStaticPtr::NiSpotLight:
				return sol::make_object(state, reinterpret_cast<NI::SpotLight*>(object));
			case NI::RTTIStaticPtr::NiStencilProperty:
				return sol::make_object(state, reinterpret_cast<NI::StencilProperty*>(object));
			case NI::RTTIStaticPtr::NiSwitchNode:
				return sol::make_object(state, reinterpret_cast<NI::SwitchNode*>(object));
			case NI::RTTIStaticPtr::NiTexturingProperty:
				return sol::make_object(state, reinterpret_cast<NI::TexturingProperty*>(object));
			case NI::RTTIStaticPtr::NiTriShape:
				return sol::make_object(state, reinterpret_cast<NI::TriShape*>(object));
			case NI::RTTIStaticPtr::NiVertexColorProperty:
				return sol::make_object(state, reinterpret_cast<NI::VertexColorProperty*>(object));
			}

			if (object->isInstanceOfType(NI::RTTIStaticPtr::NiNode)) {
				return sol::make_object(state, reinterpret_cast<NI::Node*>(object));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiTriShape)) {
				return sol::make_object(state, reinterpret_cast<NI::TriShape*>(object));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiDynamicEffect)) {
				return sol::make_object(state, reinterpret_cast<NI::DynamicEffect*>(object));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiAVObject)) {
				return sol::make_object(state, reinterpret_cast<NI::AVObject*>(object));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiSourceTexture)) {
				return sol::make_object(state, reinterpret_cast<NI::SourceTexture*>(object));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiObjectNET)) {
				return sol::make_object(state, reinterpret_cast<NI::ObjectNET*>(object));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiPixelData)) {
				return sol::make_object(state, reinterpret_cast<NI::PixelData*>(object));
			}

			return sol::make_object(state, object);
		}

		sol::object makeLuaNiPointer(NI::Object * object) {
			if (object == nullptr) {
				return sol::nil;
			}

			LuaManager& luaManager = LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			switch ((uintptr_t)object->getRunTimeTypeInformation()) {
			case NI::RTTIStaticPtr::NiAlphaProperty:
				return sol::make_object(state, NI::Pointer<NI::AlphaProperty>(reinterpret_cast<NI::AlphaProperty*>(object)));
			case NI::RTTIStaticPtr::NiAmbientLight:
				return sol::make_object(state, NI::Pointer<NI::AmbientLight>(reinterpret_cast<NI::AmbientLight*>(object)));
			case NI::RTTIStaticPtr::NiAVObject:
				return sol::make_object(state, NI::Pointer<NI::AVObject>(reinterpret_cast<NI::AVObject*>(object)));
			case NI::RTTIStaticPtr::NiCamera:
				return sol::make_object(state, NI::Pointer<NI::Camera>(reinterpret_cast<NI::Camera*>(object)));
			case NI::RTTIStaticPtr::NiCollisionSwitch:
				return sol::make_object(state, NI::Pointer<NI::CollisionSwitch>(reinterpret_cast<NI::CollisionSwitch*>(object)));
			case NI::RTTIStaticPtr::NiDirectionalLight:
				return sol::make_object(state, NI::Pointer<NI::DirectionalLight>(reinterpret_cast<NI::DirectionalLight*>(object)));
			case NI::RTTIStaticPtr::NiFogProperty:
				return sol::make_object(state, NI::Pointer<NI::FogProperty>(reinterpret_cast<NI::FogProperty*>(object)));
			case NI::RTTIStaticPtr::NiMaterialProperty:
				return sol::make_object(state, NI::Pointer<NI::MaterialProperty>(reinterpret_cast<NI::MaterialProperty*>(object)));
			case NI::RTTIStaticPtr::NiNode:
				return sol::make_object(state, NI::Pointer<NI::Node>(reinterpret_cast<NI::Node*>(object)));
			case NI::RTTIStaticPtr::NiObjectNET:
				return sol::make_object(state, NI::Pointer<NI::ObjectNET>(reinterpret_cast<NI::ObjectNET*>(object)));
			case NI::RTTIStaticPtr::NiPixelData:
				return sol::make_object(state, NI::Pointer<NI::PixelData>(reinterpret_cast<NI::PixelData*>(object)));
			case NI::RTTIStaticPtr::NiPointLight:
				return sol::make_object(state, NI::Pointer<NI::PointLight>(reinterpret_cast<NI::PointLight*>(object)));
			case NI::RTTIStaticPtr::NiSourceTexture:
				return sol::make_object(state, NI::Pointer<NI::SourceTexture>(reinterpret_cast<NI::SourceTexture*>(object)));
			case NI::RTTIStaticPtr::NiSpotLight:
				return sol::make_object(state, NI::Pointer<NI::SpotLight>(reinterpret_cast<NI::SpotLight*>(object)));
			case NI::RTTIStaticPtr::NiStencilProperty:
				return sol::make_object(state, NI::Pointer<NI::StencilProperty>(reinterpret_cast<NI::StencilProperty*>(object)));
			case NI::RTTIStaticPtr::NiSwitchNode:
				return sol::make_object(state, NI::Pointer<NI::SwitchNode>(reinterpret_cast<NI::SwitchNode*>(object)));
			case NI::RTTIStaticPtr::NiTexturingProperty:
				return sol::make_object(state, NI::Pointer<NI::TexturingProperty>(reinterpret_cast<NI::TexturingProperty*>(object)));
			case NI::RTTIStaticPtr::NiTriShape:
				return sol::make_object(state, NI::Pointer<NI::TriShape>(reinterpret_cast<NI::TriShape*>(object)));
			case NI::RTTIStaticPtr::NiVertexColorProperty:
				return sol::make_object(state, NI::Pointer<NI::VertexColorProperty>(reinterpret_cast<NI::VertexColorProperty*>(object)));
			}

			if (object->isInstanceOfType(NI::RTTIStaticPtr::NiNode)) {
				return sol::make_object(state, NI::Pointer<NI::Node>(reinterpret_cast<NI::Node*>(object)));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiTriShape)) {
				return sol::make_object(state, NI::Pointer<NI::TriShape>(reinterpret_cast<NI::TriShape*>(object)));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiDynamicEffect)) {
				return sol::make_object(state, NI::Pointer<NI::DynamicEffect>(reinterpret_cast<NI::DynamicEffect*>(object)));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiAVObject)) {
				return sol::make_object(state, NI::Pointer<NI::AVObject>(reinterpret_cast<NI::AVObject*>(object)));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiSourceTexture)) {
				return sol::make_object(state, NI::Pointer<NI::SourceTexture>(reinterpret_cast<NI::SourceTexture*>(object)));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiObjectNET)) {
				return sol::make_object(state, NI::Pointer<NI::ObjectNET>(reinterpret_cast<NI::ObjectNET*>(object)));
			}
			else if (object->isInstanceOfType(NI::RTTIStaticPtr::NiPixelData)) {
				return sol::make_object(state, NI::Pointer<NI::PixelData>(reinterpret_cast<NI::PixelData*>(object)));
			}

			return sol::make_object(state, object);
		}

		void logStackTrace(const char* message) {
			if (message != nullptr) {
				log::getLog() << message << std::endl;
			}

			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;
			sol::protected_function_result result = state["debug"]["traceback"]();
			if (result.valid()) {
				sol::optional<std::string> asString = result;
				if (asString) {
					log::getLog() << asString.value() << std::endl;
				}
			}
		}
	}
}

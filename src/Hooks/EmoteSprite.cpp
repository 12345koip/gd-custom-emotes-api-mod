#include <Geode/Geode.hpp>
#include <Geode/modify/EmoteSprite.hpp>
#include "src/Impl/DataManager/DataManager.hpp"

using namespace geode::prelude;
using namespace CustomEmotes::Impl;

class $modify(EmoteSprite) {
    static void onModify(auto& self) {
        if (!self.setHookPriority("EmoteSprite::defForName", Priority::First))
            log::warn("Could not set hook priority for EmoteSprite::defForName. Custom emotes may not work as expected!");
    }

    static EmoteDef* defForName(const std::string& name) {
        auto* dataManager = DataManager::GetSingleton();

        for (const auto& container: dataManager->GetEmoteContainers()) {
            const auto result = container->DefByName(name);
            if (result.isErr()) continue;

            const auto& customDef = result.unwrap();
            const EmoteDef* matchingDef = customDef->GetEmoteDef();
            return const_cast<EmoteDef*>(matchingDef);
        }

        return EmoteSprite::defForName(name);
    }
};
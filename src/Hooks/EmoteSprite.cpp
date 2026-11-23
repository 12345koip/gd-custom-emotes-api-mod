#include <Geode/Geode.hpp>
#include <Geode/modify/EmoteSprite.hpp>

using namespace geode::prelude;

class $modify(EmoteSprite) {
    static void onModify(auto& self) {
        if (!self.setHookPriority("EmoteSprite::defForName", Priority::First))
            log::warn("Could not set hook priority for EmoteSprite::defForName. Custom emotes may not work as expected!");
    }

    static EmoteDef* defForName(const std::string& name) {
        log::warn("defForName called");
        return EmoteSprite::defForName(name);
    }
};
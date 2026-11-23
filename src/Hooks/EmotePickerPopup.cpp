#include <Geode/Geode.hpp>
#include <Geode/modify/EmotePickerPopup.hpp>
#include "src/Impl/DataManager/DataManager.hpp"

using namespace geode::prelude;
using namespace CustomEmotes::Impl;

class $modify(EmotePickerPopup) {
    static void onModify(auto& self) {
        if (!self.setHookPriority("EmotePickerPopup::onBuildWindowContent", Priority::VeryEarly))
            log::warn("Could not set hook priority for EmotePickerPopup::onBuildWindowContent. Custom emotes may not work as expected!");

        if (!self.setHookPriority("EmotePickerPopup::layoutEmoteGrid", Priority::VeryEarly))
            log::warn("Could not set hook priority for EmotePickerPopup::layoutEmoteGrid. Custom emotes may not work as expected!");
    }

    void onBuildWindowContent() {
        EmotePickerPopup::onBuildWindowContent();
    }

    void layoutEmoteGrid() {
        EmotePickerPopup::layoutEmoteGrid();
    }
};
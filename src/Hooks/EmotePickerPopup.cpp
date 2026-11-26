#include <ranges>
#include <Geode/Geode.hpp>
#include <Geode/modify/EmotePickerPopup.hpp>
#include "src/Impl/DataManager/DataManager.hpp"

using namespace geode::prelude;
using namespace CustomEmotes::Impl;
using namespace CustomEmotes;


//helpers for layoutEmoteGrid.
static void placeEmoteCell(CCNode* node, float& y, int& col, int& placedInSection, const int maxCol, const float padX) {
    if (col == maxCol) { //max columns, newline
        y -= 26.0f;
        col = 0;
    }

    node->setPosition({
        padX + 13.0f + (26.0f * col),
        y - 13.0f
    });

    ++col;
    ++placedInSection;
}

static void endSection(float& y, int& col, int& numPlacedInSection) {
    if (numPlacedInSection == 0) return;

    y -= 38.0f;

    col = 0;
    numPlacedInSection = 0;
}





class $modify(EmotePickerPopup) {
    struct Fields {
        std::map<CustomEmotesContainer*, std::vector<CCMenuItemSpriteExtra*>> customEmotes {};
    };


    static void onModify(auto& self) {
        if (!self.setHookPriority("EmotePickerPopup::onBuildWindowContent", Priority::First))
            log::warn("Could not set hook priority for EmotePickerPopup::onBuildWindowContent. Custom emotes may not work as expected!");

        if (!self.setHookPriority("EmotePickerPopup::layoutEmoteGrid", Priority::First))
            log::warn("Could not set hook priority for EmotePickerPopup::layoutEmoteGrid. Custom emotes may not work as expected!");

        if (!self.setHookPriority("EmotePickerPopup::onPickEmote", Priority::First))
            log::warn("Could not set hook priority for EmotePickerPopup::onPickEmote. Custom emotes may not work as expected!");




        //better emote picker will handle laying out the emote grid, including custom emotes.
        if (Loader::get()->isModLoaded("kx_.better-emote-picker")) {
            log::info("Better emote picker is loaded, disabling conflicting hooks.");

            auto hookRes = self.getHook("EmotePickerPopup::layoutEmoteGrid");
            if (hookRes.isErr()) {
                log::warn("Could not get hook for EmotePickerPopup::layoutEmoteGrid: {}", hookRes.unwrapErr());
                return;
            }

            auto* hook = hookRes.unwrap();
            hook->setAutoEnable(false);
            log::info("Successfully disabled hook for EmotePickerPopup::layoutEmoteGrid");
        }
    }

    void onBuildWindowContent() {
        EmotePickerPopup::onBuildWindowContent();

        //remove all current.
        for (auto& container: this->m_fields->customEmotes | std::views::values) {
            for (const auto& emoteCell: container)
                emoteCell->removeFromParent();

            container.clear();
        }

        //now add all custom emotes.
        auto* dataManager = DataManager::GetSingleton();
        for (const auto& container: dataManager->GetEmoteContainers()) {
            if (!container->IsEnabled()) continue;

            for (const auto& customDef: container->GetAllCustomEmoteDefs()) {
                if (!customDef->IsEnabled()) continue;

                const auto* rawDef = customDef->GetEmoteDef();
                auto cell = this->addCell(rawDef, rawDef->m_particleIndex);
                if (!cell) continue;

                cell->setID(fmt::format("custom_emote_{}", rawDef->m_particleIndex));
                cell->setEnabled(this->m_canAdd);
                cell->setOpacity(this->m_canAdd? 255: 90);
                this->m_fields->customEmotes[container.get()].push_back(cell);
            }
        }

        this->layoutEmoteGrid();
    }

    void layoutEmoteGrid() {
        EmotePickerPopup::layoutEmoteGrid();

        //the last non-alias def is at index 195, so its placed last and is hence the lowest in the scroller.
        const auto lastCell = this->m_scrollContentMenu->getChildByTag(195);
        if (!lastCell) return;
        const float lowestY = lastCell->getPositionY();

        const int maxCols = std::max(3, static_cast<int>((this->m_windowWidth - 8.0f) / 26.0f));
        const float padX = (this->m_windowWidth - (maxCols * 26.0f)) * 0.5f;
        float currentY = lowestY - 25.0f;
        int currentCol = 0;
        int numPlacedInSection = 0;

        //place all custom emotes.
        for (const auto& spriteList: this->m_fields->customEmotes | std::views::values) {
            for (auto* sprite: spriteList)
                placeEmoteCell(sprite, currentY, currentCol, numPlacedInSection, maxCols, padX);

            endSection(currentY, currentCol, numPlacedInSection);
        }

        this->m_scrollContentLayer->setContentHeight(5.0f - currentY);
    }

    void onPickEmote(CCObject* sender) {
        if (!this->canAddEmote()) return;

        auto* sprite = static_cast<CCMenuItemSpriteExtra*>(sender);
        const auto tag = sprite->getTag();

        if (tag < kStartingBase) //not a custom emote.
            return EmotePickerPopup::onPickEmote(sender);

        auto* dataManager = DataManager::GetSingleton();
        auto* def = dataManager->DefBySpriteTag(tag);

        if (!def) {
            log::warn("CustomEmoteDef not found for CustomEmote. Sprite tag: {}", tag);
            return;
        }

        this->m_input->insertAtCursor(fmt::format(":{}:", def->GetEmoteDef()->m_name));
    }
};
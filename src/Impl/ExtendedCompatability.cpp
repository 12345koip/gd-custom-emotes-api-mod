#define GEODE_DEFINE_EVENT_EXPORTS

#include "include/ExtendedCompatability.hpp"
#include "src/Impl/DataManager/DataManager.hpp"

using namespace CustomEmotes;
using namespace CustomEmotes::Impl;

const std::vector<CustomEmotesContainer*> ExtendedCompatability::GetAllCustomEmoteContainers() {
    auto& containersList = DataManager::GetSingleton()->GetEmoteContainers();

    std::vector<CustomEmotesContainer*> out {};
    out.reserve(containersList.size());

    std::ranges::transform(containersList.begin(), containersList.end(), std::back_inserter(out), [](const auto& container) -> CustomEmotesContainer* {
       return container.get();
    });

    return out;
}

CustomEmoteDef* ExtendedCompatability::GetCustomEmoteDefBySpriteTag(const int tag) {
    return DataManager::GetSingleton()->DefBySpriteTag(tag);
}

int ExtendedCompatability::GetInitialContainerBaseIndex() {
    return kStartingBase;
}

int ExtendedCompatability::GetContainerBaseStep() {
    return kBaseStep;
}

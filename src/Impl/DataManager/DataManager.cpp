#include "DataManager.hpp"

using namespace CustomEmotes::Impl;
using namespace geode::prelude;

int DataManager::GetNextEmoteContainerBase() {
    const auto base = this->m_currentEmoteBase;
    this->m_currentEmoteBase += kBaseStep;
    return base;
}

void DataManager::RegisterContainer(std::unique_ptr<CustomEmotesContainer>&& container) {
    this->m_openContainers.push_back(std::move(container));
}

CustomEmotes::CustomEmoteDef* DataManager::DefBySpriteTag(const int tag) const {
    for (const auto& container: this->m_openContainers) {
        auto res = container->DefByAbsoluteIndex(tag);
        if (res.ok())
            return res.unwrap();
        else
            log::debug("CustomEmoteDef not in container: {}", res.unwrapErr());
    }

    log::debug("CustomEmoteDef for tag {} not found", tag);
    return nullptr;
}
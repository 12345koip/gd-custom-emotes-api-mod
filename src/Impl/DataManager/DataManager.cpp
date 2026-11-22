#include "DataManager.hpp"

using namespace CustomEmotes::Impl;


int DataManager::GetNextEmoteContainerBase() {
    //it's borderline impossible that the emote base could overflow the 32-bit integer limit,
    //but since this is an api mod i suppose i should assume the worst...
    if (INT_MAX - this->m_currentEmoteBase < kBaseStep)
        this->m_currentEmoteBase = kStartingBase;

    this->m_currentEmoteBase += kBaseStep;
    return this->m_currentEmoteBase;
}

void DataManager::RegisterContainer(std::unique_ptr<CustomEmotesContainer>&& container) {
    this->m_openContainers.push_back(std::move(container));
}
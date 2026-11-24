#pragma once
#include <Geode/Geode.hpp>
#include "include/CustomEmotes.hpp"

namespace CustomEmotes::Impl {
    static constexpr int kStartingBase = 11000;
    static constexpr int kBaseStep = 150;

    class DataManager final {
        private:
            std::vector<std::unique_ptr<CustomEmotesContainer>> m_openContainers {};
            int m_currentEmoteBase = kStartingBase;;

        public:
            const std::vector<std::unique_ptr<CustomEmotesContainer>>& GetEmoteContainers() const {return this->m_openContainers;}
            int GetNextEmoteContainerBase();
            void RegisterContainer(std::unique_ptr<CustomEmotesContainer>&& container);
            CustomEmoteDef* DefBySpriteTag(const int tag) const;

            static DataManager* GetSingleton() {
                static DataManager singleton;
                return &singleton;
            }
    };
}
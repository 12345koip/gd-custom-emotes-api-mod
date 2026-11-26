#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/EventExport.hpp>
#include "CustomEmotes.hpp"

#undef MY_MOD_ID
#define MY_MOD_ID "kx_.custom-emotes-api"

namespace CustomEmotes::ExtendedCompatability {
    /**
     * @return All `CustomEmoteContainer`s.
     */
    GEODE_EVENT_EXPORT(const std::vector<CustomEmotesContainer*>, GetAllCustomEmoteContainers, (), ());

    /**
     * @brief Retrieves a CustomEmoteDef by its corresponding sprite's tag, returning `nullptr` if not found.
     * @param tag The corresponding EmoteSprite's tag.
     * @return The `CustomEmoteDef` if found, `nullptr` otherwise.
     */
    GEODE_EVENT_EXPORT(CustomEmoteDef*, GetCustomEmoteDefBySpriteTag, (const int tag), (tag));

    /**
     * @brief Retrieves the initial base offset used to index CustomEmoteDefs.
     * @return The initial base offset used to index CustomEmoteDefs.
     */
    GEODE_EVENT_EXPORT(int, GetInitialContainerBaseIndex, (), ());

    /**
     * @brief Returns the step applied to the CustomEmotesContainer base index each time a new one is created.
     * @return The step factor applied to the base.
     */
    GEODE_EVENT_EXPORT(int, GetContainerBaseStep, (), ());
}

#undef MY_MOD_ID
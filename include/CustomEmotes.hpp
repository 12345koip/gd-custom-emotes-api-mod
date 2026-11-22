#pragma once
#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS
    #ifdef KX_CUSTOMEMOTESAPI_EXPORTING
        #define EMOTESAPI_DLL __declspec(dllexport)
    #else
        #define EMOTESAPI_DLL __declspec(dllimport)
    #endif
#else
    #define EMOTESAPI_DLL __attribute__((visibility("default")))
#endif

namespace CustomEmotes {
    class CustomEmotesContainer;


    /**
     * A class representing a custom emote.
     */
    class EMOTESAPI_DLL CustomEmoteDef {
        private:
            /**
             * This emote's name.
             */
            std::string m_name;

            /**
             * The internal EmoteDef corresponding to this emote.
             */
            EmoteDef m_def;

            /**
             * The CustomEmotesContainer this emote belongs to.
             */
            CustomEmotesContainer* m_owningContainer;

            /**
             * The CustomEmoteDef this def is an alias to. `nullptr` if this CustomEmoteDef is not an alias.
             */
            CustomEmoteDef* m_canonicalDef;

            /**
             * The particle index, relative to the base of the CustomEmoteContainer this emote belongs to.
             * @note The ordering is 0-based.
             */
            int m_relativeParticleIndex;

            /**
             * Whether or not this emote is currently enabled. A disabled emote will effectively not render from now on.
             */
            bool m_enabled;

            friend class CustomEmotesContainer;

        public:
            /**
             * Adds an alias for this emote.
             * @param aliasName The name for the alias.
             * @return A pointer to the CustomEmoteDef for the alias, if created successfully.
             */
            geode::Result<CustomEmoteDef*> AddAlias(const std::string_view aliasName);

            /**
             * Controls this emote's enabled state. A disabled emote won't render anywhere.
             * @param enabled Whether or not the emote should be enabled.
             */
            void SetEnabled(const bool enabled);

            /**
             * @return The particle index of this emote, relative to it's owning CustomEmoteContainer's base.
             */
            int GetRelativeParticleIndex() const;

            /**
             * @return The absolute particle index of this emote. It's texture is keyed in the texture cache under the same index.
             */
            int GetAbsoluteParticleIndex() const;

            /**
             * @return The EmoteDef corresponding to this emote.
             */
            const EmoteDef* GetDef() const;

            /**
             * @return Whether or not this CustomEmoteDef is an alias for another.
             */
            bool IsAlias() const;

            /**
             * @returns The CustomEmoteDef this one is an alias too, if present. If this is not an alias, this CustomEmoteDef is returned.
             */
            CustomEmoteDef* GetCanonicalDef() const;
    };


    /**
     * A container class for CustomEmoteDefs. One container represents one new section to be drawn in the EmotePickerPopup.
     */
    class EMOTESAPI_DLL CustomEmotesContainer {
        private:
            /**
             * The internal container for all the custom emotes that are a part of this container.
             * @note this is inherently ordered by index.
             */
            std::vector<std::shared_ptr<CustomEmoteDef>> m_emotes {};

            /**
             * The name of this container.
             */
            std::string m_name;

            /**
             * The base index of this container.
             */
            int m_particleBase;

            /**
             * The relative index to be assigned to the next custom emote.
             */
            int m_nextRelativeIndex = 0;

            /**
             * Whether or not this container is currently enabled. If `false`, all custom emotes in this container are effectively disabled.
             */
            bool m_enabled = true;

            friend class CustomEmoteDef;

        public:
            CustomEmotesContainer() = default;
            ~CustomEmotesContainer() = default;

            /**
             * Returns the CustomEmoteDef denoted by the index relative to this container's base, if associated with this container.
             * @param index The particle index of the target CustomEmoteDef, relative to this container's base index.
             * @return The target CustomEmoteDef, if associated with this container.
             */
            geode::Result<CustomEmoteDef*> DefByRelativeIndex(const int index) const;


            /**
             * Returns the CustomEmoteDef denoted by the index relative to this container's base, if associated with this container.
             * @param index The `m_particleIndex` of the target CustomEmoteDef.
             * @return The target CustomEmoteDef, if associated with this container.
             */
            geode::Result<CustomEmoteDef*> DefByAbsoluteIndex(const int index) const;

            /**
             * Returns the first CustomEmoteDef that matches `name`.
             * @param name The name of the emote to find the CustomEmoteDef for.
             * @return The corresponding CustomEmoteDef, if found.
             */
            geode::Result<CustomEmoteDef*> DefByName(const std::string_view name) const;

            /**
             * Creates a new custom emote using the provided image and name.
             * @param filePath The path to the standalone emote image file.
             * @param name The name for the emote.
             * @return The CustomEmoteDef associated with the emote, if creation was successful.
             * @note If you are using a spritesheet, use `NewEmoteFromSpritesheet` instead.
             */
            geode::Result<CustomEmoteDef*> NewEmoteFromImage(const std::string_view filePath, const std::string_view name);

            /**
             * Creates a new custom emote using the provided sprite frame and name.
             * @param frameName The frame name of the target sprite in its spritesheet.
             * @return The CustomEmoteDef associated with the emote, if creation was successful.
             * @note If you are using a standalone image, use `NewEmoteFromImage` instead.
             */
            geode::Result<CustomEmoteDef*> NewEmoteFromSpritesheet(const std::string_view frameName);

            /**
             * Checks if this container is full.
             * @return whether or not the container is full.
             */
            bool IsFull() const;

            /**
             * Creates a new CustomEmotesContainer with the provided name.
             * @param name The name for the container.
             * @return The container, if creation was successful.
             */
            static geode::Result<CustomEmotesContainer*> create(const std::string_view name);
    };
}
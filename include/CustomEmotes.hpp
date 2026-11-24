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
     * @note This class does not extend EmoteDef due to necessary field type changes.
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
             * The CustomEmoteDef this def is an alias to. Set to the current object if this CustomEmoteDef is not an alias.
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
            geode::Result<CustomEmoteDef*> AddAlias(const std::string& aliasName);

            /**
             * Controls this emote's enabled state. A disabled emote won't render anywhere.
             * @param enabled Whether or not the emote should be enabled.
             */
            void SetEnabled(const bool enabled);

            /**
             * @return Whether or not this custom emote will show in the scroller
             */
            bool IsEnabled() const;

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
            const EmoteDef* GetEmoteDef() const;

            /**
             * @return Whether or not this CustomEmoteDef is an alias for another.
             */
            bool IsAlias() const;

            /**
             * Sets the scale for emotes this CustomEmoteDef.
             * @param newScale The new scale to be applied to the emote.
             */
            void SetScale(const float newScale);

            /**
             * Sets the x-axis positional offset for this CustomEmoteDef.
             * @param newX The new positional offset on the x-axis.
             */
            void SetPositionalOffsetX(const float newX);

            /**
             * Sets the y-axis positional offset for this CustomEmoteDef.
             * @param newY The new positional offset on the y-axis.
             */
            void SetPositionalOffsetY(const float newY);

            /**
             * Sets the positional offset for this CustomEmoteDef.
             * @param newOffset The new positional offset.
             */
            void SetPositionalOffset(const cocos2d::CCPoint& newOffset);

            /**
             * Sets the color for this CustomEmoteDef.
             * @param newColor The new color to be used.
             */
            void SetColor(const cocos2d::ccColor3B& newColor);

            /**
             * Used to control whether this emote should be flipped on the x-axis.
             * @param newFlipX Whether or not this emote should be flipped.
             */
            void SetFlipX(const bool newFlipX);

            /**
             * @returns The CustomEmoteDef this one is an alias too, if present. If this is not an alias, this CustomEmoteDef is returned.
             */
            CustomEmoteDef* GetCanonicalDef() const;
    };


    /**
     * A helper structure used to provide options for a CustomEmoteDef.
     */
    class EMOTESAPI_DLL CustomEmoteOptions {
        public:
            CustomEmoteOptions() = default;

            /**
             * The scale of the emote. Corresponds to `EmoteDef::m_scale`.
             */
            float scale = 0.8f;

            /**
             * Corresponds to `EmoteDef::m_x`.
             */
            float x = 0.0f;

            /**
             * Corresponds to `EmoteDef::m_y`.
             */
            float y = 0.0f;

            /**
             * The color of the emote. Corresponds to `EmoteDef::m_color`.
             */
            cocos2d::ccColor3B color {230, 235, 230};

            /**
             * Whether or not the emote should be flipped horizontally. Corresponds to `EmoteDef::m_flipX`.
             */
            bool flipX = false;
    };

    /**
     * A container class for CustomEmoteDefs. One container represents one new section to be drawn in the EmotePickerPopup.
     */
    class EMOTESAPI_DLL CustomEmotesContainer {
        private:
            /**
             * The internal container for all the custom emotes that are a part of this container.
             * @note this is inherently ordered by index. Aliases are not stored here (see `m_emoteAliases`)
             */
            std::vector<std::shared_ptr<CustomEmoteDef>> m_emotes {};

            /**
             * The internal container used for all aliases of custom emotes that are a part of this container.
             */
            std::map<int, std::vector<std::shared_ptr<CustomEmoteDef>>> m_emoteAliases;

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
             * @note Try and avoid this where possible.
             */
            geode::Result<CustomEmoteDef*> DefByName(const std::string_view name) const;

            /**
             * Creates a new custom emote using the provided image and name.
             * @param filePath The path to the standalone emote image file.
             * @param emoteName The name for the emote. This will be converted to lowercase.
             * @param options Optional extra data for the custom emote.
             * @return The CustomEmoteDef associated with the emote, if creation was successful.
             * @note If you are using a spritesheet, use `NewEmoteFromSpritesheet` instead.
             */
            geode::Result<CustomEmoteDef*> NewEmoteFromImage(const std::string_view filePath, const std::string& rawEmoteName, const CustomEmoteOptions& options = {});

            /**
             * Creates a new custom emote using the provided sprite frame and name.
             * @param frameName The frame name of the target sprite in its spritesheet.
             * @param emoteName The name to use for the emote. This will be converted to lowercase.
             * @param options Optional extra data for the custom emote.
             * @return The CustomEmoteDef associated with the emote, if creation was successful.
             * @note If you are using a standalone image, use `NewEmoteFromImage` instead.
             */
            geode::Result<CustomEmoteDef*> NewEmoteFromSpritesheet(const std::string_view frameName, const std::string& rawEmoteName, const CustomEmoteOptions& options = {});

            /**
             * Checks if this container is full.
             * @return whether or not the container is full.
             */
            bool IsFull() const;

            /**
             * Retrieves the relative base applied to CustomEmoteDefs which are a part of this container.
             * @return The base index of this container.
             */
            int GetParticleIndexBase() const;

            /**
             * @return Whether or not this container is enabled.
             */
            bool IsEnabled() const;

            /**
             * Setter to control if this container should render in the scroller.
             * @param newEnabled Whether or not this container should show in the scroller.
             */
            void SetEnabled(const bool newEnabled);

            /**
             * Creates an emote alias for the given custom emote.
             * @param emote The custom emote to create an alias for.
             * @param aliasName The name to be given to the alias.
             * @return The CustomEmoteDef for the alias, if created successfully.
             */
            geode::Result<CustomEmoteDef*> AddAliasForEmote(CustomEmoteDef* emote, const std::string& aliasName);

            /**
             * @return All the CustomEmoteDefs associated with this container.
             */
            const std::vector<CustomEmoteDef*> GetAllCustomEmoteDefs() const;

            /**
             * @return All the CustomEmoteDef aliases associated with this container.
             */
            const std::vector<CustomEmoteDef*> GetAllCustomEmoteAliases() const;

            /**
             * Creates a new CustomEmotesContainer with the provided name.
             * @param name The name for the container.
             * @return The container, if creation was successful.
             */
            static geode::Result<CustomEmotesContainer*> create(const std::string& name);
    };
}
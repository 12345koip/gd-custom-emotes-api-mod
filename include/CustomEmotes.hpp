#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/EventExport.hpp>

#undef MY_MOD_ID
#define MY_MOD_ID "kx_.custom-emotes-api"

namespace CustomEmotes {
    class CustomEmotesContainer;

    /**
     * A class representing a custom emote.
     * @note This class does not extend EmoteDef due to necessary field type changes.
     */
    class CustomEmoteDef {
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
             * Whether or not this emote is currently enabled. A disabled emote will effectively not render.
             */
            bool m_enabled;

            friend class CustomEmotesContainer;

        public:
            /**
             * @brief Adds an alias for this emote.
             * @param aliasName The name for the alias. This will be converted to lowercase.
             * @return A pointer to the CustomEmoteDef for the alias, if created successfully.
             * @note This cannot be called with an alias.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmoteDef, geode::Result<CustomEmoteDef*>, AddAlias, (const std::string& aliasName), (this, aliasName));

            /**
             * @brief Controls this emote's enabled state. A disabled emote won't render anywhere.
             * @param enabled Whether or not this emote should be enabled.
             * @note Calling this on an alias will only affect the alias. If you need to change the emote
             * and its aliases, call it from `alias->GetCanonicalDef()`.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmoteDef, void, SetEnabled, (const bool enabled), (this, enabled));

            /**
             * @return Whether or not this custom emote will show in the scroller
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmoteDef, bool, IsEnabled, (), (this));

            /**
             * @return The particle index of this emote, relative to it's owning CustomEmoteContainer's base.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmoteDef, int, GetRelativeParticleIndex, (), (this));

            /**
             * @return The absolute particle index of this emote. It's texture is keyed in the texture cache under the same index.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmoteDef, int, GetAbsoluteParticleIndex, (), (this));

            /**
             * @return The EmoteDef corresponding to this custom emote.
             * @note To read standard game properties of this CustomEmoteDef, you should use this function
             * and read directly from its return result.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmoteDef, const EmoteDef*, GetEmoteDef, (), (this));

            /**
             * @return Whether or not this CustomEmoteDef is an alias for another.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmoteDef, bool, IsAlias, (), (this));

            /**
             * @brief Sets the scale for emotes this CustomEmoteDef.
             * @param newScale The new scale to be applied to the emote.
             * @note Calling this on an alias will only affect the alias. If you need to change the emote
             * and its aliases, call it from `alias->GetCanonicalDef()`.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmoteDef, void, SetScale, (const float newScale), (this, newScale));

            /**
             * @brief Sets the x-axis positional offset for this CustomEmoteDef.
             * @param newX The new positional offset on the x-axis.
             * @note Calling this on an alias will only affect the alias. If you need to change the emote
             * and its aliases, call it from `alias->GetCanonicalDef()`.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmoteDef, void, SetPositionalOffsetX, (const float newX), (this, newX));

            /**
             * @brief Sets the y-axis positional offset for this CustomEmoteDef.
             * @param newY The new positional offset on the y-axis.
             * @note Calling this on an alias will only affect the alias. If you need to change the emote
             * and its aliases, call it from `alias->GetCanonicalDef()`.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmoteDef, void, SetPositionalOffsetY, (const float newY), (this, newY));

            /**
             * @brief Sets the positional offset for this CustomEmoteDef.
             * @param newOffset The new positional offset.
             * @note Calling this on an alias will only affect the alias. If you need to change the emote
             * and its aliases, call it from `alias->GetCanonicalDef()`.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmoteDef, void, SetPositionalOffset, (const cocos2d::CCPoint& newOffset), (this, newOffset));

            /**
             * @brief Sets the color for this CustomEmoteDef.
             * @param newColor The new color to be used.
             * @note Calling this on an alias will only affect the alias. If you need to change the emote
             * and its aliases, call it from `alias->GetCanonicalDef()`.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmoteDef, void, SetColor, (const cocos2d::ccColor3B& newColor), (this, newColor));

            /**
             * @brief Used to control whether this emote should be flipped on the x-axis.
             * @param newFlipX Whether or not this emote should be flipped.
             * @note Calling this on an alias will only affect the alias. If you need to change the emote
             * and its aliases, call it from `alias->GetCanonicalDef()`.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmoteDef, void, SetFlipX, (const bool newFlipX), (this, newFlipX));

            /**
             * @returns The CustomEmoteDef this one is an alias too, if present. If this is not an alias, this CustomEmoteDef is returned.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmoteDef, CustomEmoteDef*, GetCanonicalDef, (), (this));

            /**
             * @returns All aliases corresponding to this custom emote.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmoteDef, geode::Result<std::vector<CustomEmoteDef*>>, GetAliases, (), (this));
    };


    /**
     * A helper structure used to provide options for a CustomEmoteDef.
     */
    class CustomEmoteOptions {
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
    class CustomEmotesContainer {
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
             * The name of this container. This is not used by this mod, but other mods may wish to use it.
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
             * @brief Returns the CustomEmoteDef denoted by the index relative to this container's base, if associated with this container.
             * @param index The particle index of the target CustomEmoteDef, relative to this container's base index.
             * @return The target CustomEmoteDef, if associated with this container.
             * @note Will not return an alias.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmotesContainer, geode::Result<CustomEmoteDef*>, DefByRelativeIndex, (const int index), (this, index));


            /**
             * @brief Returns the CustomEmoteDef denoted by the index relative to this container's base, if associated with this container.
             * @param index The `m_particleIndex` of the target CustomEmoteDef.
             * @return The target CustomEmoteDef, if associated with this container.
             * @note Will not return an alias.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmotesContainer, geode::Result<CustomEmoteDef*>, DefByAbsoluteIndex, (const int index), (this, index));

            /**
             * @brief Returns the first CustomEmoteDef that matches `name`.
             * @param rawName The name of the emote to find the CustomEmoteDef for. This will be converted to lowercase.
             * @return The corresponding CustomEmoteDef, if found.
             * @note Try and avoid this where possible. This can return an alias.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmotesContainer, geode::Result<CustomEmoteDef*>, DefByName, (const std::string& rawName), (this, rawName));

            /**
             * @brief Creates a new custom emote using the provided image and name.
             * @param fileName The file name for the standalone emote image file.
             * @param rawEmoteName The name for the emote. This will be converted to lowercase.
             * @param options Optional extra data for the custom emote.
             * @return The CustomEmoteDef associated with the emote, if creation was successful.
             * @note If you are using a spritesheet, use `NewEmoteFromSpritesheet` instead.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmotesContainer, geode::Result<CustomEmoteDef*>, NewEmoteFromImage, (const std::string_view fileName, const std::string& rawEmoteName, const CustomEmoteOptions& options), (this, fileName, rawEmoteName, options));

            /**
             * @brief Creates a new custom emote using the provided sprite frame and name.
             * @param frameName The frame name of the target sprite in its spritesheet.
             * @param emoteName The name to use for the emote. This will be converted to lowercase.
             * @param options Optional extra data for the custom emote.
             * @return The CustomEmoteDef associated with the emote, if creation was successful.
             * @note If you are using a standalone image, use `NewEmoteFromImage` instead.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmotesContainer, geode::Result<CustomEmoteDef*>, NewEmoteFromSpritesheet, (const std::string_view frameName, const std::string& rawEmoteName, const CustomEmoteOptions& options), (this, frameName, rawEmoteName, options));

            /**
             * @return The name of this CustomEmotesContainer.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmotesContainer, const std::string_view, GetName, (), (this));

            /**
             * @param newName The new name for this container.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmotesContainer, void, SetName, (const std::string& newName), (this, newName));

            /**
             * @brief Checks if this container is full.
             * @return whether or not the container is full.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmotesContainer, bool, IsFull, (), (this));

            /**
             * @brief Retrieves the relative base applied to CustomEmoteDefs which are a part of this container.
             * @return The base index of this container.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmotesContainer, int, GetParticleIndexBase, (), (this));

            /**
             * @return Whether or not this container is enabled.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmotesContainer, bool, IsEnabled, (), (this));

            /**
             * @brief Setter to control if this emote should render in the scroller.
             * @param newEnabled Whether or not this container should be enabled.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmotesContainer, void, SetEnabled, (const bool newEnabled), (this, newEnabled));

            /**
             * @brief Creates an emote alias for the given custom emote.
             * @param emote The custom emote to create an alias for.
             * @param aliasName The name to be given to the alias. This will be converted to lowercase.
             * @return The CustomEmoteDef for the alias, if created successfully.
             */
            GEODE_EVENT_EXPORT_MEMBER(CustomEmotesContainer, geode::Result<CustomEmoteDef*>, AddAliasForEmote, (CustomEmoteDef* emote, const std::string& aliasName), (this, emote, aliasName));

            /**
             * @return All the CustomEmoteDefs associated with this container.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmotesContainer, const std::vector<CustomEmoteDef*>, GetAllCustomEmoteDefs, (), (this));

            /**
             * @return All the CustomEmoteDef aliases associated with this container.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmotesContainer, std::vector<CustomEmoteDef*>, GetAllCustomEmoteAliases, (), (this));

            /**
             * @param emote The emote to retrieve all aliases for.
             * @return All aliases for the give emote.
             */
            GEODE_EVENT_EXPORT_MEMBER_CONST(CustomEmotesContainer, geode::Result<std::vector<CustomEmoteDef*>>, GetAllAliasesForEmote, (const CustomEmoteDef* emote), (this, emote));

            /**
             * @brief Creates a new CustomEmotesContainer with the provided name.
             * @param name The name for the container.
             * @return The container, if creation was successful.
             */
            GEODE_EVENT_EXPORT_MEMBER_STATIC(geode::Result<CustomEmotesContainer*>, create, (const std::string& name), (name));
    };
}

#undef MY_MOD_ID
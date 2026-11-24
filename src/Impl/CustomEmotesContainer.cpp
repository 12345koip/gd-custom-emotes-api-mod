#include "include/CustomEmotes.hpp"
#include "DataManager/DataManager.hpp"
#include <ranges>

using namespace geode::prelude;
using namespace CustomEmotes::Impl;
using namespace CustomEmotes;

Result<CustomEmotesContainer*> CustomEmotesContainer::create(const std::string& name) {
    auto* dataManager = DataManager::GetSingleton();
    const int containerBase = dataManager->GetNextEmoteContainerBase();

    auto container = std::make_unique<CustomEmotesContainer>();
    container->m_name = name;
    container->m_particleBase = containerBase;

    auto* raw = container.get();
    dataManager->RegisterContainer(std::move(container));
    return Ok(raw);
}

Result<CustomEmoteDef*> CustomEmotesContainer::DefByRelativeIndex(const int index) const {
    if (index < 0 || index >= this->m_emotes.size())
        return Err("Index out of range");

    auto* defPtr = this->m_emotes[index].get();
    return Ok(defPtr);
}

Result<CustomEmoteDef*> CustomEmotesContainer::DefByAbsoluteIndex(const int index) const {
    const int relativeIndex = index - this->m_particleBase;
    return this->DefByRelativeIndex(relativeIndex);
}

//horrible time complexity, but oh well. there's no better way i can think of
Result<CustomEmoteDef*> CustomEmotesContainer::DefByName(const std::string_view name) const {
    //search standalones
    const auto it_standalone = std::ranges::find_if(this->m_emotes, [name](const auto& other) -> bool {
        return other->m_name == name;
    });

    if (it_standalone != this->m_emotes.end())
        return Ok(it_standalone->get());

    //now search aliases
    for (const auto& aliasContainer: this->m_emoteAliases | std::views::values) {
        const auto it_alias = std::ranges::find_if(aliasContainer, [name](const auto& other) -> bool {
            return other->m_name == name;
        });

        if (it_alias != aliasContainer.end())
            return Ok(it_alias->get());
    }

    return Err("No CustomEmoteDef with name '{}' found in container", name);
}

bool CustomEmotesContainer::IsFull() const {
    return this->m_emotes.size() == kBaseStep;
}

int CustomEmotesContainer::GetParticleIndexBase() const {
    return this->m_particleBase;
}

const std::string_view CustomEmotesContainer::GetName() const {
    return this->m_name;
}

void CustomEmotesContainer::SetName(const std::string& newName) {
    this->m_name = newName;
}


Result<CustomEmoteDef*> CustomEmotesContainer::AddAliasForEmote(CustomEmoteDef* emote, const std::string& aliasName) {
    //NOTE: multiple aliases for an emote that use the same name aren't inherently an issue, so they aren't checked for here.

    auto alias = std::make_shared<CustomEmoteDef>(*emote);
    alias->m_name = aliasName;
    alias->m_canonicalDef = emote;
    alias->m_def.m_isAlias = true;
    alias->m_def.m_name = alias->m_name.c_str();

    this->m_emoteAliases[emote->m_relativeParticleIndex].push_back(alias);
    return Ok(alias.get());
}

const std::vector<CustomEmoteDef*> CustomEmotesContainer::GetAllCustomEmoteDefs() const {
    std::vector<CustomEmoteDef*> out {};
    out.reserve(this->m_emotes.size());

    std::ranges::transform(this->m_emotes, std::back_inserter(out), [](const auto& ptr) -> CustomEmoteDef* {
        return ptr.get();
    });

    return out;
}

const std::vector<CustomEmoteDef*> CustomEmotesContainer::GetAllCustomEmoteAliases() const {
    std::vector<CustomEmoteDef*> out {};

    //okay this views stuff is actually insanely cool
    auto rawPtrs = this->m_emoteAliases | std::views::values | std::views::join | std::views::transform([](const auto& ptr) -> CustomEmoteDef* {
        return ptr.get();
    });

    out.assign(rawPtrs.begin(), rawPtrs.end());
    return out;
}

bool CustomEmotesContainer::IsEnabled() const {
    return this->m_enabled;
}

void CustomEmotesContainer::SetEnabled(const bool newEnabled) {
    this->m_enabled = newEnabled;
}


//create an EmoteDef for the given CustomEmote data.
static EmoteDef makeEmoteDefForCustom(const std::string_view name, const int absoluteIndex, const CustomEmoteOptions& options) {
    return EmoteDef {
        .m_name = name.data(),
        .m_particleIndex = static_cast<unsigned int>(absoluteIndex), //TODO: FIX once changed in bindings
        .m_scale = options.scale,
        .m_x = options.x, .m_y = options.y,
        .m_color = options.color,
        .m_isAlias = false,
        .m_flipX = options.flipX
    };
}


Result<CustomEmoteDef*> CustomEmotesContainer::NewEmoteFromImage(const std::string_view fileName, const std::string& rawEmoteName, const CustomEmoteOptions& options) {
    if (this->IsFull())
        return Err("This container is full. Please use another one.");

    //must lower it to match vanilla.
    const auto emoteName = geode::utils::string::toLower(rawEmoteName);

    auto* textureCache = CCTextureCache::sharedTextureCache();
    auto* spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();

    auto* texture = textureCache->addImage(fileName.data(), false);
    if (!texture)
        return Err("Image '{}' couldn't be added to the texture cache. Please check the path and try again!", fileName);

    const auto size = texture->getContentSize();
    const auto absoluteIndex = this->m_particleBase + this->m_nextRelativeIndex;
    const auto targetName = fmt::format("particle_{}_001.png", absoluteIndex);

    if (size.width <= 0 || size.height <= 0)
        return Err("Image '{}' has invalid dimensions", fileName);

    //and add it under the particle name.
    auto* frame = CCSpriteFrame::createWithTexture(texture, {0, 0, size.width, size.height});
    spriteFrameCache->addSpriteFrame(frame, targetName.c_str());



    //construct custom emote def.
    const auto customDef = std::make_shared<CustomEmoteDef>();
    customDef->m_name = emoteName;

    const auto def = makeEmoteDefForCustom(customDef->m_name, absoluteIndex, options); //the string_view::data() dangles otherwise.
    customDef->m_canonicalDef = customDef.get();
    customDef->m_def = def;
    customDef->m_relativeParticleIndex = this->m_nextRelativeIndex++;
    customDef->m_owningContainer = this;
    customDef->m_enabled = true;

    this->m_emotes.push_back(customDef);
    return Ok(customDef.get());
}



Result<CustomEmoteDef*> CustomEmotesContainer::NewEmoteFromSpritesheet(const std::string_view frameName, const std::string& rawEmoteName, const CustomEmoteOptions& options) {
    if (this->IsFull())
        return Err("This container is full. Please use another one.");

    //must lower it to match vanilla.
    const auto emoteName = geode::utils::string::toLower(rawEmoteName);

    //ensure the sprite is loaded from the sheet.
    auto* spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    auto* frame = spriteFrameCache->spriteFrameByName(frameName.data());
    if (!frame)
        return Err("Sprite frame '{}' not found. Please ensure the spritesheet is loaded.", frameName);

    const auto absoluteIndex = this->m_particleBase + this->m_nextRelativeIndex;
    const auto targetName = fmt::format("particle_{}_001.png", absoluteIndex);
    spriteFrameCache->addSpriteFrame(frame, targetName.c_str());




    //construct custom emote def.
    const auto customDef = std::make_shared<CustomEmoteDef>();
    customDef->m_name = emoteName;

    const auto def = makeEmoteDefForCustom(customDef->m_name, absoluteIndex, options);
    customDef->m_canonicalDef = customDef.get();
    customDef->m_def = def;
    customDef->m_relativeParticleIndex = this->m_nextRelativeIndex++;
    customDef->m_owningContainer = this;
    customDef->m_enabled = true;

    this->m_emotes.push_back(customDef);
    return Ok(customDef.get());
}
#include "include/CustomEmotes.hpp"
#include "DataManager/DataManager.hpp"

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

Result<CustomEmoteDef*> CustomEmotesContainer::DefByName(const std::string_view name) const {
    const auto it = std::ranges::find_if(this->m_emotes, [name](const auto& other) -> bool {
        return other->m_name == name;
    });

    if (it == this->m_emotes.end())
        return Err("No CustomEmoteDef with name '{}' found in container", name);

    return Ok(it->get());
}

bool CustomEmotesContainer::IsFull() const {
    return this->m_emotes.size() == kBaseStep;
}

int CustomEmotesContainer::GetParticleIndexBase() const {
    return this->m_particleBase;
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


//create an EmoteDef for the given CustomEmote data.
static EmoteDef makeEmoteDefForCustom(const std::string_view name, const int absoluteIndex, const CustomEmoteOptions& options, const bool isAlias) {
    return {
        .m_name = name.data(),
        .m_particleIndex = static_cast<unsigned int>(absoluteIndex), //TODO: FIX once changed in bindings
        .m_scale = options.scale,
        .m_x = options.x, .m_y = options.y,
        .m_color = options.color,
        .m_isAlias = isAlias,
        .m_flipX = options.flipX
    };
}


Result<CustomEmoteDef*> CustomEmotesContainer::NewEmoteFromImage(const std::string_view filePath, const std::string& rawEmoteName, const CustomEmoteOptions& options) {
    if (this->IsFull())
        return Err("This container is full. Please use another one.");

    //must lower it to match vanilla.
    const auto emoteName = geode::utils::string::toLower(rawEmoteName);

    auto* textureCache = CCTextureCache::sharedTextureCache();
    auto* spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();

    auto* texture = textureCache->addImage(filePath.data(), false);
    if (!texture)
        return Err("Image '{}' couldn't be added to the texture cache. Please check the path and try again!", filePath);

    const auto size = texture->getContentSize();
    const auto absoluteIndex = this->m_particleBase + this->m_nextRelativeIndex;
    const auto targetName = fmt::format("particle_{}_001.png", absoluteIndex);


    //...now see if it's there already.
    if (spriteFrameCache->spriteFrameByName(targetName.data()))
        return Err("Another emote is already using this sprite frame: '{}'", targetName);

    //and add it under the particle name.
    auto* frame = CCSpriteFrame::createWithTexture(texture, {0, 0, size.width, size.height});
    spriteFrameCache->addSpriteFrame(frame, targetName.c_str());



    //construct custom emote def.
    const auto customDef = std::make_shared<CustomEmoteDef>();
    customDef->m_name = emoteName;

    const auto def = makeEmoteDefForCustom(customDef->m_name, absoluteIndex, options, false); //the string_view::data() dangles otherwise.
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

    //...now see if it's there already.
    if (spriteFrameCache->spriteFrameByName(targetName.data()))
        return Err("Another emote is already using this sprite frame: '{}'", targetName);

    spriteFrameCache->addSpriteFrame(frame, targetName.c_str());




    //construct custom emote def.
    const auto customDef = std::make_shared<CustomEmoteDef>();
    customDef->m_name = emoteName;

    const auto def = makeEmoteDefForCustom(customDef->m_name, absoluteIndex, options, false); //the string_view::data() dangles otherwise.
    customDef->m_canonicalDef = customDef.get();
    customDef->m_def = def;
    customDef->m_relativeParticleIndex = this->m_nextRelativeIndex++;
    customDef->m_owningContainer = this;
    customDef->m_enabled = true;

    this->m_emotes.push_back(customDef);
    return Ok(customDef.get());
}
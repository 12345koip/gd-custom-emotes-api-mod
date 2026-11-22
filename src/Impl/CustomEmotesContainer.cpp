#include "include/CustomEmotes.hpp"
#include "DataManager/DataManager.hpp"

using namespace geode::prelude;
using namespace CustomEmotes::Impl;
using namespace CustomEmotes;

Result<CustomEmotesContainer*> CustomEmotesContainer::create(const std::string_view name) {
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

Result<CustomEmoteDef*> CustomEmotesContainer::NewEmoteFromImage(const std::string_view filePath, const std::string_view name) {
    if (this->IsFull())
        return Err("This container is full. Please use another one.");

    auto* textureCache = CCTextureCache::sharedTextureCache();
    auto* spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();

    auto* texture = textureCache->addImage(filePath.data(), false);
    if (!texture)
        return Err("Image '{}' couldn't be added to the texture cache. Please check the path and try again!", filePath);

    //...now see if it's there already.
    const auto size = texture->getContentSize();
    const auto index = this->m_particleBase + this->m_nextRelativeIndex;
    const auto frameName = fmt::format("particle_{}_001.png", index);
    if (spriteFrameCache->spriteFrameByName(frameName.data()))
        return Err("Another emote is already using this name: '{}'", frameName);

    //and add it under the particle name.
    auto* frame = CCSpriteFrame::createWithTexture(texture, {0, 0, size.width, size.height});
    spriteFrameCache->addSpriteFrame(frame, frameName.c_str());



    //need to give it a def.
    //TODO: make this a helper for NewEmoteFromFrame or whatever its called
    EmoteDef def {
        .m_name = name.data(),
        .m_particleIndex = static_cast<unsigned int>(index), //TODO: FIX once changed in bindings
        .m_scale = 0.8f,
        .m_x = 0.0f, .m_y = 0.0f,
        .m_color = ccColor3B(230, 230, 230),
        .m_isAlias = false,
        .m_flipX = false
    };

    auto customDef = std::make_shared<CustomEmoteDef>();
    customDef->m_name = name;
    customDef->m_canonicalDef = customDef.get();
    customDef->m_def = def;
    customDef->m_relativeParticleIndex = this->m_nextRelativeIndex++;
    customDef->m_owningContainer = this;
    customDef->m_enabled = true;

    this->m_emotes.push_back(customDef);
    return Ok(customDef.get());
}
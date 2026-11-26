#define GEODE_DEFINE_EVENT_EXPORTS
#include "include/CustomEmotes.hpp"

using namespace geode::prelude;
using namespace CustomEmotes;

bool CustomEmoteDef::IsAlias() const {
    return this->m_canonicalDef != this;
}

CustomEmoteDef* CustomEmoteDef::GetCanonicalDef() const {
    return this->m_canonicalDef;
}

const EmoteDef* CustomEmoteDef::GetEmoteDef() const {
    return &this->m_def;
}

int CustomEmoteDef::GetRelativeParticleIndex() const {
    return this->m_relativeParticleIndex;
}

int CustomEmoteDef::GetAbsoluteParticleIndex() const {
    return this->m_relativeParticleIndex + this->m_owningContainer->m_particleBase;
}

void CustomEmoteDef::SetEnabled(const bool enabled) {
    this->m_enabled = enabled;

    if (!this->IsAlias()) {
        auto result = this->GetAliases();
        if (!result.isOk()) return;

        for (auto* alias: result.unwrap())
            alias->SetEnabled(enabled);
    }
}

void CustomEmoteDef::SetScale(const float newScale) {
    this->m_def.m_scale = newScale;


    if (!this->IsAlias()) {
        auto result = this->GetAliases();
        if (!result.isOk()) return;

        for (auto* alias: result.unwrap())
            alias->SetScale(newScale);
    }
}

void CustomEmoteDef::SetPositionalOffsetX(const float newX) {
    this->m_def.m_x = newX;

    if (!this->IsAlias()) {
        auto result = this->GetAliases();
        if (!result.isOk()) return;

        for (auto* alias: result.unwrap())
            alias->SetPositionalOffsetX(newX);
    }
}

void CustomEmoteDef::SetPositionalOffsetY(const float newY) {
    this->m_def.m_y = newY;

    if (!this->IsAlias()) {
        auto result = this->GetAliases();
        if (!result.isOk()) return;

        for (auto* alias: result.unwrap())
            alias->SetPositionalOffsetY(newY);
    }
}

void CustomEmoteDef::SetPositionalOffset(const CCPoint& newOffset) {
    this->m_def.m_x = newOffset.x;
    this->m_def.m_y = newOffset.y;

    if (!this->IsAlias()) {
        auto result = this->GetAliases();
        if (!result.isOk()) return;

        for (auto* alias: result.unwrap())
            alias->SetPositionalOffset(newOffset);
    }
}

void CustomEmoteDef::SetColor(const ccColor3B& newColor) {
    this->m_def.m_color = newColor;
}

void CustomEmoteDef::SetFlipX(const bool newFlipX) {
    this->m_def.m_flipX = newFlipX;
}

bool CustomEmoteDef::IsEnabled() const {
    return this->m_enabled;
}

Result<std::vector<CustomEmoteDef*>> CustomEmoteDef::GetAliases() const {
    return this->m_owningContainer->GetAllAliasesForEmote(this);
}

Result<CustomEmoteDef*> CustomEmoteDef::AddAlias(const std::string& aliasName) {
    return this->m_owningContainer->AddAliasForEmote(this, aliasName);
}
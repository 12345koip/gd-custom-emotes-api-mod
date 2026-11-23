#include "include/CustomEmotes.hpp"

using namespace geode::prelude;
using namespace CustomEmotes;

bool CustomEmoteDef::IsAlias() const {
    return this->m_canonicalDef != this;
}

CustomEmoteDef* CustomEmoteDef::GetCanonicalDef() const {
    return this->m_canonicalDef;
}

const EmoteDef* CustomEmoteDef::GetDef() const {
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
}

Result<CustomEmoteDef*> CustomEmoteDef::AddAlias(const std::string& aliasName) {
    return this->m_owningContainer->AddAliasForEmote(this, aliasName);
}
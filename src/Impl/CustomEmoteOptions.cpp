#include "include/CustomEmotes.hpp"

using namespace CustomEmotes;

CustomEmoteOptions& CustomEmoteOptions::operator=(const EmoteDef& def) {
    this->color = def.m_color;
    this->flipX = def.m_flipX;
    this->scale = def.m_scale;
    this->x = def.m_x;
    this->y = def.m_y;

    return *this;
}

CustomEmoteOptions& CustomEmoteOptions::operator=(EmoteDef&& def) {
    this->color = def.m_color;
    this->flipX = def.m_flipX;
    this->scale = def.m_scale;
    this->x = def.m_x;
    this->y = def.m_y;

    def.m_color = {0, 0, 0};
    def.m_flipX = false;
    def.m_scale = 0.0f;
    def.m_x = 0.0f;
    def.m_y = 0.0f;

    return *this;
}

CustomEmoteOptions::CustomEmoteOptions(const EmoteDef& def) {
    this->color = def.m_color;
    this->flipX = def.m_flipX;
    this->scale = def.m_scale;
    this->x = def.m_x;
    this->y = def.m_y;
}

CustomEmoteOptions::CustomEmoteOptions(EmoteDef&& def) {
    this->color = def.m_color;
    this->flipX = def.m_flipX;
    this->scale = def.m_scale;
    this->x = def.m_x;
    this->y = def.m_y;

    def.m_color = {0, 0, 0};
    def.m_flipX = false;
    def.m_scale = 0.0f;
    def.m_x = 0.0f;
    def.m_y = 0.0f;
}
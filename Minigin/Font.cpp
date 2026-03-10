#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "Font.h"

TTF_Font* dae::Font::GetFont() const {
	return m_font;
}

dae::Font::Font(const std::string& fullPath, int size)
    : m_font(nullptr), m_fullPath(fullPath)
{
    m_font = TTF_OpenFont(fullPath.c_str(), static_cast<float>(size));
    if (m_font == nullptr)
    {
        throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
    }
}


dae::Font::~Font()
{
	TTF_CloseFont(m_font);
}

std::shared_ptr<dae::Font> dae::Font::SetSize(unsigned int size) const
{
	return std::make_shared<Font>(m_fullPath.c_str(), size);
}
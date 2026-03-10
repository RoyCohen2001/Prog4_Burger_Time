#pragma once
#include <string>
#include <memory>

struct TTF_Font;
namespace dae
{
	/**
	 * Simple RAII wrapper for a TTF_Font
	 */
	class Font final
	{
	public:
		TTF_Font* GetFont() const;
		explicit Font(const std::string& fullPath, int size);
		~Font();

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;
		
		std::shared_ptr<Font> SetSize(unsigned int size) const;
	private:
		TTF_Font* m_font;
		std::string m_fullPath;
	};
}

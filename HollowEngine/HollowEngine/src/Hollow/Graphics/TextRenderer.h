#pragma once

namespace Hollow
{
	class Shader;
	struct UITextData;

	struct Character
	{
		GLuint textID; //ID handle of the glyph texture
		glm::vec2 size; //size of glyph
		glm::vec2 bearing; //offset from baseline to left/top of glyph
		GLuint advance; //horizontal offset to advance to next glyph
	};
	
	class TextRenderer
	{
	public:
		/// <summary>
		/// Not yet implemented
		/// </summary>
		HOLLOW_API void Init();

		/// <summary>
		/// Loads a font from the specified file
		/// </summary>
		/// <param name="fontFile">File path to font texture</param>
		HOLLOW_API void LoadFont(std::string fontFile);

		/// <summary>
		/// Renders the text by creating a texture with the specified UITextData
		/// </summary>
		/// <param name="data">The UI data to be used when creating the text texture</param>
		/// <param name="shader">The OpenGL shader to be used when rendering text</param>
		HOLLOW_API void RenderText(UITextData& data, Shader* shader);

	private:
		std::wstring ToWString(std::string& aChars);
	private:
		std::map<wchar_t, Character> mCharacters;
		GLuint mVAO_Text, mVBO_Text;
		FT_Library mft;
		FT_Face mface;
	};
}
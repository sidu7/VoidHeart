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
		void Init();
		void RenderText(UITextData& data, Shader* shader);

	private:
		void Load();

	private:
		std::map<GLchar, Character> mCharacters;
		GLuint mVAO_Text, mVBO_Text;
		FT_Library mft;
		FT_Face mface;
	};
}
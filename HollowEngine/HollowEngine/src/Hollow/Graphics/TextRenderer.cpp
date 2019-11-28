#include <hollowpch.h>
#include "TextRenderer.h"

#include "Hollow/Graphics/Data/UITextData.h"
#include "Shader.h"

namespace Hollow
{
	void TextRenderer::Init()
	{
		//Init FreeType
		if (FT_Init_FreeType(&mft))
		{
			HW_CORE_ERROR("Error::Freetype: Could not init FreeType Library");
		}
		Load();
	}

	void TextRenderer::Load()
	{
		//Load font as mface
		if (FT_New_Face(mft, "Resources/Fonts/Kiddish.ttf", 0, &mface))
		{
			HW_CORE_ERROR("Error::Freetype: Failed to load font");
		}
		//Set size to load glyphs as
		FT_Set_Pixel_Sizes(mface, 0, 48);
		//Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//Load first 128 characters of ASCII set
		for (GLubyte c = 0; c < 128; ++c)
		{
			//Load character glyph
			if (FT_Load_Char(mface, c, FT_LOAD_RENDER))
			{
				HW_CORE_ERROR("Error::Freetype: Failed to load Glyph");
				continue;
			}

			//Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RED,
				mface->glyph->bitmap.width,
				mface->glyph->bitmap.rows,
				0, GL_RED, GL_UNSIGNED_BYTE,
				mface->glyph->bitmap.buffer
			);

			//Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glm::vec2 size_((float)mface->glyph->bitmap.width, (float)mface->glyph->bitmap.rows);
			glm::vec2 bearing_((float)mface->glyph->bitmap_left, (float)mface->glyph->bitmap_top);

			//Now store character for later use
			Character character = {
				texture,
				size_,
				bearing_,
				mface->glyph->advance.x
			};
			mCharacters.insert(std::pair<GLchar, Character>(c, character));
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		//Destroy FreeType once we're finished
		FT_Done_Face(mface);
		FT_Done_FreeType(mft);

		//Configure VAO/VBO for texture quads
		glGenVertexArrays(1, &mVAO_Text);
		glGenBuffers(1, &mVBO_Text);
		glBindVertexArray(mVAO_Text);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO_Text);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void TextRenderer::RenderText(UITextData& data, Shader* shader)
	{
		//Activate corresponding render state
		shader->Use();
		shader->SetVec3("textColor", data.mColor.x, data.mColor.y, data.mColor.z);
		shader->SetInt("textTexture", 0);
	
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(mVAO_Text);

		//Iterate through all characters
		std::string::const_iterator c;
		for (c = data.mText.begin(); c != data.mText.end(); ++c)
		{
			Character ch = mCharacters[*c];

			GLfloat xPos = data.mPosition.x + ch.bearing.x * data.mScale.x;
			GLfloat yPos = data.mPosition.y - (ch.size.y - ch.bearing.y) * data.mScale.y;

			GLfloat w = ch.size.x * data.mScale.x;
			GLfloat h = ch.size.y * data.mScale.y;

			//Update mVBO_Text for each character
			GLfloat vertices[6][4] = {
				{xPos,		yPos + h,	0.0, 0.0},
				{xPos,		yPos,		0.0, 1.0},
				{xPos + w,	yPos,		1.0, 1.0},

				{xPos,		yPos + h,	0.0, 0.0},
				{xPos + w,	yPos,		1.0, 1.0},
				{xPos + w,	yPos + h,	1.0, 0.0}
			};

			//Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.textID);
			//Update content of mVBO_Text memory
			glBindBuffer(GL_ARRAY_BUFFER, mVBO_Text);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			//Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			//Advance cursors for next glyph
			data.mPosition.x += (ch.advance >> 6)* data.mScale.x;
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

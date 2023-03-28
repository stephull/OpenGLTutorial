#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "text_renderer.h"
#include "resource_manager.h"

using namespace std;

TextRenderer::TextRenderer(unsigned int width, unsigned int height) {
	this->TextShader = ResourceManager::LoadShader("text_2d.vs", "text_2d.fs", nullptr, "text");
	glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);
	this->TextShader.SetMatrix4("projection", proj, true);
	this->TextShader.SetInteger("text", 0);

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::Load(string font, unsigned int size) {
	this->Characters.clear();

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		cout << "FreeType error: Could not initialize FreeType library" << endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face)) {
		cout << "FreeType error: Failed to load font" << endl;
	}

	FT_Set_Pixel_Sizes(face, 0, size);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_GlyphSlot glyph;
	FT_Bitmap bm;
	FT_Int bm_left;
	FT_Int bm_top;
	FT_Vector adv;

	for (GLubyte c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c , FT_LOAD_RENDER)) {
			cout << "FreeType error: Failed to load Glyph" << endl;
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glyph = face->glyph;

		bm = glyph->bitmap;
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RED, bm.width, bm.rows, 0, GL_RED, GL_UNSIGNED_BYTE, bm.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		bm_left = glyph->bitmap_left;
		bm_top = glyph->bitmap_top;
		adv = glyph->advance;
		Character character = {
			texture,
			glm::ivec2(bm.width, bm.rows),
			glm::ivec2(bm_left, bm_top),
			(unsigned int)adv.x
		};

		Characters.insert(pair<char, Character>(c, character));
	}
}

void TextRenderer::RenderText(string text, float x, float y, float scale, glm::vec3 color) {
	this->TextShader.Use();
	this->TextShader.SetVector3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	string::const_iterator c = text.begin();
	for (c; c != text.end(); c++) {
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		int y_tmp = this->Characters['H'].Bearing.y - ch.Bearing.y;
		float ypos = y + y_tmp * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		float vertices[6][4]{
			{ xpos, ypos + h, 0.0f, 1.0f },
			{ xpos + w, ypos, 1.0f, 0.0f },
			{ xpos, ypos, 0.0f, 0.0f },
			{ xpos, ypos + h, 0.0f, 1.0f },
			{ xpos + w, ypos + h, 1.0f, 1.0f },
			{ xpos + w, ypos, 1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.id);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
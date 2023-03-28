#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"

using namespace std;
using namespace glm;

struct Character {
	unsigned int id;
	ivec2 Size;
	ivec2 Bearing;
	unsigned int Advance;
};

class TextRenderer {
public:
	map<char, Character> Characters;
	Shader TextShader;

	TextRenderer(unsigned int width, unsigned int height);

	void Load(string font, unsigned int size);
	void RenderText(string text, float x, float y, float scale, vec3 color = vec3(1.0f));

private:
	unsigned int VAO, VBO;
};

#endif
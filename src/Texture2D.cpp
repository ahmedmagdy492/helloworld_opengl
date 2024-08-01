#include "Texture2D.h"

static int textureUnit = 0;

Texture2D::Texture2D(const char* imagePath, GLenum imgPixelFormat) {
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath, &width, &height, &nChannels, 0);

	if (!data)
		throw new std::string("Unable to load provided image");

	glGenTextures(1, &ID);
	this->currentTextureUnit = GL_TEXTURE0 + textureUnit;
	glActiveTexture(this->currentTextureUnit);
	textureUnit++;
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imgPixelFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

void Texture2D::setOption(GLenum name, int value) {
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, name, value);
}
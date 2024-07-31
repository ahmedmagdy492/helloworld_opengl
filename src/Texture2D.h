#pragma once

#include <iostream>
#include <glad\glad.h>
#include "libs\stb_image.h"

class Texture2D {
private:
	unsigned int ID;
	int width, height, nChannels;

public:
	Texture2D(const char* imagePath, GLenum imgPixelFormat);

	void setOption(GLenum name, int value);
};
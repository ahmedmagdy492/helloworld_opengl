#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

typedef void (*FrameBufferResizeHandler)(int width, int height);

class GLWindow {
private:
	GLFWwindow* window;

	void InitWindow();

public:
	GLWindow(int width, int height, const std::string& title);

	bool ShouldWindowClose();

	bool IsKeyPressed(unsigned short key, unsigned short type);

	void SetOnFrameBufferChangeHandler(FrameBufferResizeHandler handler);

	void SwapBuffers();

	void PollEvents();

	void CloseWindow();

	~GLWindow();
};
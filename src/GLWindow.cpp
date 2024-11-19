#include "include/GLWindow.h"

void GLWindow::InitWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLWindow::GLWindow(int width, int height, const std::string& title) : window(nullptr) {
	InitWindow();

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(window);
}

bool GLWindow::ShouldWindowClose() {
	return glfwWindowShouldClose(window);
}


bool GLWindow::IsKeyPressed(unsigned short key, unsigned short type) {
	return glfwGetKey(window, key) == type;
}

void GLWindow::SetOnFrameBufferChangeHandler(FrameBufferResizeHandler handler) {
	glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)handler);
}

void GLWindow::SwapBuffers() {
	glfwSwapBuffers(window);
}

void GLWindow::PollEvents() {
	glfwPollEvents();
}

void GLWindow::CloseWindow() {
	glfwSetWindowShouldClose(window, true);
}

GLWindow::~GLWindow() {
	glfwTerminate();
}
#include "Core.hpp"
#include "Shader.hpp"

class Window {

public:

	Window(int width, int height, const char* title = "Default Title") {
		
		if (!glfwInit()) {
			std::cerr << "Não foi possível inicializar GLFW" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!window) {
			std::cerr << "Não foi possível inicializar a janela" << std::endl;
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		if (glewInit() != GLEW_OK) {
			std::cerr << "Não foi possível inicializar o GLEW" << std::endl;
			std::exit(EXIT_FAILURE);
		}

	}

	~Window() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	bool shouldClose() const {
		return glfwWindowShouldClose(window);
	}

	void pollEvents() const {
		return glfwPollEvents();
	}

	void swapBuffers() const {
		glfwSwapBuffers(window);
	}

	static void framebuffer_size_callback(GLFWwindow* wind, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void setShader(const Shader& shader) {
		currentShader = shader;
	}

	void clear() const {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	operator GLFWwindow* () const {
		return window;
	}

private:

	GLFWwindow* window = nullptr;
	Shader currentShader;

};
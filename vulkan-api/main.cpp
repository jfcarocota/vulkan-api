#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>

#include "VulknaRenderer.h"

GLFWwindow* window;

VulknaRenderer vulkanRenderer;

void InitWindow(std::string windowName = "Test", const int width = 800, const int height = 800)
{
	//init glfw
	glfwInit();

	//indicar que GLFW no use opengl para levantar Vulkan
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // desactiva el reescalado de la ventana de GLFW

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr); //c_str convierte de std string a literalmente const char* myString
}

int main()
{
	InitWindow();

	//Crear instancia de VulknaRenderer

	if (vulkanRenderer.init(window) == EXIT_FAILURE) // checar que no falló al crear la la instancia de VK
	{
		return EXIT_FAILURE;
	}

	//main loop

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	vulkanRenderer.CleanUp();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
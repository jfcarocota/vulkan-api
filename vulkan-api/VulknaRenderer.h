#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>//sirve para cachar escepciones, util para debugear vulkan
#include <vector>

#define APPLICATION_NAME "Vulkan Test"
#define ENGINE_NAME "Beagle Engine"

class VulknaRenderer
{
	public:
		VulknaRenderer();

		int init(GLFWwindow* window);

		~VulknaRenderer();
	private:
		GLFWwindow* window{};

		VkInstance instance{};

		void CreateIsntance();
};


#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>//sirve para cachar escepciones, util para debugear vulkan
#include <vector>
#include "Utilities.h"

#define APPLICATION_NAME "Vulkan Test"
#define ENGINE_NAME "Beagle Engine"

class VulknaRenderer
{
	public:
		VulknaRenderer();

		int init(GLFWwindow* window);

		void CleanUp();

		~VulknaRenderer();
	private:
		GLFWwindow* window{};

		struct {
			VkPhysicalDevice physicalDevice;
			VkDevice logicalDevice;
		} mainDevice;

		VkQueue graphicsQueue;

		VkInstance instance{};

		void CreateIsntance();

		void CreateLogicalDevice();

		void GetPhysicalDevice();

		bool CheckDeviceSuitable(VkPhysicalDevice device);

		bool CheckInstanceExtensionsSupport(std::vector<const char*>* checkExtensions);

		QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device);
};


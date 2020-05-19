#include "VulknaRenderer.h"

#include <iostream>


VulknaRenderer::VulknaRenderer()
{
	this->window = nullptr;
}

int VulknaRenderer::init(GLFWwindow* window)
{
	this->window = window;

	try
	{
		CreateIsntance();
	}
	catch (const std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}

VulknaRenderer::~VulknaRenderer()
{
}

void VulknaRenderer::CreateIsntance()
{
	//creación de información para la instancia de vulcan VkInstance
	VkInstanceCreateInfo createinfo{};
	// vamos a indicarle a que tipo de estructura se va adaptar esta variable, sType significa Struct Type o tipo de esturctura
	createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
}

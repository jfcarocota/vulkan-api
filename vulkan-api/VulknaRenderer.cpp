#include "VulknaRenderer.h"

#include <iostream>


VulknaRenderer::VulknaRenderer()
{
	this->window = nullptr;
	this->instance = nullptr;
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
	//información de la app literalmente, no de vulkan, solo la app en general
	//no afecta al programa como tal y es conveniente para ayudar al developer, hace la vida mas fácil uwu
	VkApplicationInfo appinfo{};

	//Indica la forma de la estructura para app info
	appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	//Literalmente es el nombre de tu app, pApplicationName ya forma parte del struct ya que lo iniciamos de esa forma en el info
	appinfo.pApplicationName = APPLICATION_NAME;
	//Indica la version de la app, en este caso la 1.0.1, osea (1, 0, 1)
	appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	//El nombre de tu Engine
	appinfo.pEngineName = ENGINE_NAME;
	//version de tu engine
	appinfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	//verison de vulkan api que vamos a usar
	appinfo.apiVersion = VK_API_VERSION_1_2;

	// vamos a indicarle a que tipo de estructura se va adaptar esta variable, sType significa Struct Type o tipo de esturctura
	createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createinfo.pApplicationInfo = &appinfo;

	//Obtener el nombre de las extensiones de glfw
	//Las almacenaremos en un vector que guardara otro apuntador con los datos de cada extension
	std::vector<const char*> instanceExtensions = std::vector<const char*>();
	//contador de extensiones, recordar uint32_t significa un entero const sin signo de 32 bits, propio de glfw
	uint32_t glfwExtensionsCount = 0;

	//creamos un apuntador que apunte a otro para almacenar el contenedor de nuestras extenciones
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);//la funcion recibe un apuntador asi que necesita valor por ref

	for (size_t i = 0; i < glfwExtensionsCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	createinfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size()); //lo castea a const unsigned int
	createinfo.ppEnabledExtensionNames = instanceExtensions.data();

	//por el momento las layers de validacion quedan pendientes
	createinfo.enabledLayerCount = 0;
	createinfo.ppEnabledLayerNames = nullptr;

	//crear la instancia
	VkResult result = vkCreateInstance(&createinfo, nullptr, &instance);


	if (result != VK_SUCCESS)
	{
		if (result == VK_ERROR_INCOMPATIBLE_DRIVER)
		{
			std::cout << "driver incompatible" << std::endl;
		}
		throw std::runtime_error("Failed to create Vulkan Instance");
	}
}

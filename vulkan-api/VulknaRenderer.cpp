#include "VulknaRenderer.h"

#include <iostream>


VulknaRenderer::VulknaRenderer()
{
	
}

int VulknaRenderer::init(GLFWwindow* window)
{
	this->window = window;

	try
	{
		CreateIsntance();
		GetPhysicalDevice();
		CreateLogicalDevice();
	}
	catch (const std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}

void VulknaRenderer::CleanUp()
{
	vkDestroyDevice(mainDevice.logicalDevice, nullptr);
	vkDestroyInstance(instance, nullptr);
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
	uint32_t glfwExtensionsCount{};

	//creamos un apuntador que apunte a otro para almacenar el contenedor de nuestras extenciones
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);//la funcion recibe un apuntador asi que necesita valor por ref

	for (size_t i = 0; i < glfwExtensionsCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	//Verificar que las extensiones de la isntancia de Vk son soportadas
	if (!CheckInstanceExtensionsSupport(&instanceExtensions))
	{
		throw std::runtime_error("Vk instance doesn't support requiered extensiones!");
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

void VulknaRenderer::CreateLogicalDevice()
{
	//Regresa la familia de la cola para el dispositivo fisico seleccionado
	QueueFamilyIndices indices = GetQueueFamilies(mainDevice.physicalDevice);

	//Representa la cola que necesitamos crear y la información para crearla (solo se hará una cola por ahora)
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamilty;//Representa el indice de la familia seleccionada para crear la cola
	queueCreateInfo.queueCount = 1;//El numero de cola que quiero crear
	float priority = 1.0f;//Representa la prioridad de las colas, ya que podemos tener mas de 1, 1 = Alta, 0 = baja y valores intermedios.
	queueCreateInfo.pQueuePriorities = &priority; //es un puntero, asi que necesita forzosamente una variable y su ref &

	//Información para crear el dispositivo lógico (logicalDevice), algnas veces conocido simplemente como device, no confundir con physicalDevice
	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1; // numero de colas para crear la información
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;//Lista de cola de información para crear las colas requeridas
	deviceCreateInfo.enabledExtensionCount = 0;//número de extensiones habilitadas para dispositivo lógico
	deviceCreateInfo.ppEnabledExtensionNames = nullptr;//Lista de extensiones habilitadas para el dispositivo lógico

	//Caracteristicas que el dispositivo físico estará usando
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	//Logical device = device
	VkResult result = vkCreateDevice(mainDevice.physicalDevice, &deviceCreateInfo, nullptr, &mainDevice.logicalDevice);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Physical Device!");
	}

	//Las colas deben crearse a la par del dispositivo logico (logicalDevice o device)
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamilty, 0, &graphicsQueue);//recordar el index es 0 porque por el momento solo tenemos una queue
}

void VulknaRenderer::GetPhysicalDevice()
{
	//Enumera las instancias de Vk a las cuales puede acceder
	uint32_t deviceCount{};
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	
	//Revisar si no encontro dispositivos, de ser asi, entonces la GPU no soporta vulkan, ya que es incapaz de cagar dispositivos
	if (deviceCount == 0)
	{
		throw std::runtime_error("Can't find GPUs that support Vulkan Instance!");
	}

	//obtener la lista de dispositivos
	std::vector<VkPhysicalDevice> deviceList(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data());
	
	//Por ahora solo tomará el primer dispositivo que encuentre
	//mainDevice.physicalDevice = deviceList[0];

	for (const auto& device : deviceList)
	{
		if (CheckDeviceSuitable(device))
		{
			mainDevice.physicalDevice = device;
			break;
		}
	}
}

//Tenemos que verificar que el dispositivo que seleccionemes es adecuado a nuestra Instancia de Vulkan
bool VulknaRenderer::CheckDeviceSuitable(VkPhysicalDevice device)
{
	/*
	//Información sobre el dispositivo
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	//Información sobre lo que ese dispositivo puede hacer, shaders, geometría, etc
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	*/

	QueueFamilyIndices indices = GetQueueFamilies(device);
	return indices.IsValid();
}

bool VulknaRenderer::CheckInstanceExtensionsSupport(std::vector<const char*>* checkExtensions)
{
	//Primero hay que calcular el nunmero de extensiones antes de poder enviar el tamaño del array con las referncias a las extensiones

	uint32_t extensionesCount{};
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionesCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionesCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionesCount, extensions.data());

	//Comparar si hay extensiones que no son soportadas
	for(const auto  &checkExtension :  *checkExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			if (strcmp(checkExtension, extension.extensionName))
			{
				hasExtension = true;
				break;
			}
		}
		if (!hasExtension)
		{
			return false;
		}
	}
	return true;
}

QueueFamilyIndices VulknaRenderer::GetQueueFamilies(VkPhysicalDevice device)
{
	//Queremos obtener la información sobre las propiedades de cada familia de Queues
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount{};

	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

	//Revisamos y almenos tiene un type requerido por la Queue
	int i{};
	for (const auto& queueFamily : queueFamilyList)
	{
		//indica el numero de familia de graficas encontradas
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamilty = i;
		}
		//Para de buscar si el indice es valido
		if (indices.IsValid())
		{
			break;
		}
		i++;
	}
	return indices;
}

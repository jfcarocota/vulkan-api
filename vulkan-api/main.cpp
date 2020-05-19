#define GLFW_INCLUDE_VULKAN //con esta sentecia llamamos por dentro de glfw a vulkan api, debemos tenr vulkan api ligado.
#include<GLFW/glfw3.h>//Esta libreria trae una sentencia para incluir vulkan
//#include<vulkan/vulkan.h> //Esta seria la forma normal de incluir vulkan

#define GLM_FORCERADIANS //Este define forzará las medidas de angulos en radianes, la sentencia if el nombre del define ya viene con la libreria que usará.
#define GLM_FORCE_DEPTH_ZERO_TO_ONE //lo mismo que el paso anterior pero para ajustar el depth
#include<glm/glm.hpp> //agregamos nuestra libreria especial de matematicas para graphics library (gl)
#include<glm/mat4x4.hpp>//agregamos matrices 4 para transformaciones.

#include<iostream>

int main()
{
	glfwInit();//literalmente inicializa todo lo que tenga que ver con glfw incluyendo la llamada a vulkan api inicial.

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Test window", nullptr, nullptr);

	//main loop

	while (!glfwWindowShouldClose(window)) // el loop vive mientras la venta exista o no se cierre.
	{
		glfwPollEvents(); //trae todos los eventos de inputs mientrar este loop viva
	}

	glfwDestroyWindow(window);
}


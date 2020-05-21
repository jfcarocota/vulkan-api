#pragma once

//Indices(localicaziones) de la familia de las colas (Queues)

struct QueueFamilyIndices {
	int graphicsFamilty{ -1 }; //si se queda en -1, significa que esa familia no existe

	//Revisa si la Queue de familias es validad (porp el moento solo revisaremos una)
	bool IsValid()
	{
		return graphicsFamilty >= 0;
	}
};
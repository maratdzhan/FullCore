#include "163CommonHeader.h"

CalculatingInterface c_interface;
HWND hwnd_message_1;

void SetGeometry(const Parameters & parameters, std::vector<FuelAssembly_n> & assembly)
{
	//Определяем геометрию
	//Передаем ее в класс FuelAssembly


	// Define FA position
	try {

		// Рисуем кнопочки
		c_interface.PaintButtons();

		// Пока пользователь думает - загружаем координаты ТВС
		for (short number = 0; number < stoi(parameters.Accordance("FUEL_ASSAMBLIES_COUNT")); number++)
		{
			//FuelAssembly_n currentAssembly(number,
			//	AssemblyCoordinates(number + 1, true), AssemblyCoordinates(number + 1, false));
			//assembly.push_back(currentAssembly);
		}

	}
	catch (std::out_of_range & or_init)
	{

		MessageBox(hwnd_message_1, or_init.what(),
			"Error in initializing parameters", MB_OK);
	}

}
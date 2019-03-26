#include "163CommonHeader.h"

// Replace cout with cerr

int main()
{
	CommonParametersHandler handle;

	try {
		// Get states list
		//Start calculating
		for (const auto & content : handle.FilesList())
		{
			try {
				Core CurrentLoad(content);
			}
			catch (std::exception & catching)
			{
				std::cerr << catching.what() << std::endl;
			}
		}
	}
	catch (std::exception & ex_main)
	{
		std::cerr << ex_main.what() << std::endl;
	}

 	return 0;
}
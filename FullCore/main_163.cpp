#include "163CommonHeader.h"

// Replace cout with cerr

int main()
{
   	CommonParametersHandler handle;

		// Get states list
		// Start calculating
	for (const auto & content : handle.FilesList())
	{
		Core CurrentLoad(content);
		CurrentLoad.LoadingAssembly();
	}


	// Last were 300 ms;
 	return 0;
}
 
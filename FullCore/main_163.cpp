#include "163CommonHeader.h"

// Replace cout with cerr

int main()
{
   	CommonParametersHandler handle;

		// Get states list
		// Start calculating
	for (const auto & content : handle.FilesList())
	{
		if (content.IsCalculationInitialized()) {
			Core CurrentLoad(content);
			CurrentLoad.LoadingAssemblies();
			CurrentLoad.PermparMaking();
			CurrentLoad.NewdataMaking();
		}
	}


	// Last were 300 ms;
 	return 0;
}
 
